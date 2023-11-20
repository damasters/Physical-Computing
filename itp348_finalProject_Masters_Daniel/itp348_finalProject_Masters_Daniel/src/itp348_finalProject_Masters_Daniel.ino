/*
 * Project itp348_finalProject_Masters_Daniel
 * Description: Final Project: building a fluid consumption tracker and reminder
 * Author: Daniel Masters
 * Date: 11/17/20
 */

#include "SparkFunMMA8452Q.h"
#include "pitches.h"
MMA8452Q accel;
const int PIN_ECHO = D2;                            //sensor sends to argon input
const int PIN_TRIGGER = D3;                         //argon output sends to sensor
const int PIN_BUTTON = D5;                          //button
const int PIN_LED = D6;                             //LED
const int PIN_BUZZER = D4;                          //alarm for the device
double SPEED_SOUND_CM = 0.03444;                    //temperature dependent
int melody[] = {NOTE_E6, NOTE_D6, NOTE_C6, NOTE_D6, NOTE_E6, NOTE_E6, NOTE_E6, NOTE_D6, NOTE_D6, NOTE_D6, NOTE_E6, NOTE_G6, NOTE_G6, NOTE_E6, NOTE_D6, NOTE_C6, NOTE_D6, NOTE_E6, NOTE_E6, NOTE_E6, NOTE_E6, NOTE_D6, NOTE_D6, NOTE_E6, NOTE_D6, NOTE_C6};
int duration[] = {4, 4, 4, 4, 4, 4, 2, 4, 4, 2, 4, 4, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2};
const unsigned long eventInterval = 500;            //interval for millis for LED
unsigned long prevTime = 0;                         //variable for millis for LED
unsigned long secondPrevTime = 0;                   //variable for millis for LED
int LEDstate = LOW;                                 //state of LED
                                   
int counter = 1;                                    //controls the amount of times device was on its side (accelerometer sees this)
int heightOfCup = 9;                                //the height of the cup in cm

int intervalForUser = 0;                            //interval for the user begins at 0
int amountForUser = 0;                              //goal amount for user begins at 0
bool startMeasuring = false;                        //StartMeasuring is controlled by user in particle app
int intervalForUserInMilliseconds = 0;              //calculation based on the user's input
int prevInterval = 0;                               //for millis 

int currentButtonState = LOW;                       //these two variables for when button is pressed
int lastButtonState = LOW;

double currentSip = 0;                              //these variables for when user takes a sip and calculating the amount they drank
double previousSip = 0;
double sumOfDrunk = 0;
double totalSum = 0;

unsigned long startTimer;                           //millis timer

// setup() runs once, when the device is first turned on.
void setup() {
  pinMode(PIN_TRIGGER, OUTPUT);                    //pinModes for my components
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_BUTTON, INPUT);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  accel.begin(SCALE_2G, ODR_1);                    //starting the accelerometer
  Particle.function("User Interval", setInterval);                    //for Particle app recognition
  Particle.function("User Amount", setAmount);
  Particle.function("User Start", setStart);
}

int setInterval(String cmd){
  intervalForUser = cmd.toInt();                                      //takes in the command from user
  intervalForUserInMilliseconds = intervalForUser * 60000;            //convert minutes to milliseconds
  return intervalForUserInMilliseconds;                                //returns a number
}

int setAmount(String cmd){                                            //takes in the command from user
  amountForUser = cmd.toInt();                  
  return amountForUser;                                               //returns number
}

int setStart(String cmd){
  if (cmd == "start" || cmd == "Start" || cmd == "START") {           //user can type these variants of the word start to start the device
    startMeasuring = true;                                            //startMeasuring = true so now the device can start measuring
    prevInterval = millis();                                          //prevInterval is noted here
    return 2;
  }
  else if (cmd == "pause" || cmd == "Pause" || cmd == "PAUSE") {      //user can type these variants of pause in the particle app
    startMeasuring = false;                                           //start measuring will remain false until the user types start again
    return 1;
  }
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  if (startMeasuring == true){                                        //when user types start
    if (accel.available()){                                           // To update acceleration values from the accelerometer, call accel.read();
      accel.read();
      byte orientation = accel.readPL();                              // gives a measurment that determines direction
      if (orientation == LOCKOUT) {                                   //LOCKOUT is when the device is laying horizontally
        startTimer = millis();                                        //begin the millis timer
        digitalWrite(PIN_TRIGGER, LOW);                               //send low for 2 microseconds, high for 10 microseconds, low (sequence)
        delayMicroseconds(2);                                         //this is the process to start the ultrasonic distance sensor
        digitalWrite(PIN_TRIGGER, HIGH);
        delayMicroseconds(10);
        digitalWrite(PIN_TRIGGER, LOW);
        if (counter == 1) {                                           //counter is intialized to 1 so this will automatically begin
          int roundTripTime = pulseIn(PIN_ECHO, HIGH);                //listen when pin is HIGH, stop listening when pin is low
          double distanceCm = roundTripTime * SPEED_SOUND_CM / 2;
          double fluidLevel = distanceCm - 3;                         //to compensate for the ultrasonic distance sensors position of about 3 cm above the top of the cup
          double heightOfContents = heightOfCup - fluidLevel;         //determining the current height based on the overall height of cup
          double volumeOfCup = 23.39 * heightOfContents;              // 23.39 = pi/3(height)(radius of top squared)(radius of bottom squared)(radius of top times radius of bottom)
          double convertToOunces = volumeOfCup * 0.0338;              //1 cubic centimeter = 0.0338 US fluid ounces
          currentSip = convertToOunces;                               //level of the water
          String data = "{ \"key\":\"Fluid Level\"," "\"value\":" + String(currentSip)+"}";         //sending data to InitialState
          // Trigger the integration
          Particle.publish("waterTracker", data, PRIVATE);
          counter = 0;                                                //counter = 0 to stop ultrasonic distance sensor measuring
        }
        else if (counter == 3){                                       //the the device is not LOCKOUT, registers the next sip
          prevInterval = startTimer;                                  //millis 
          startTimer = millis();
          int roundTripTime = pulseIn(PIN_ECHO, HIGH);                //repeat measuring process
          double distanceCm = roundTripTime * SPEED_SOUND_CM / 2;
          double fluidLevel = distanceCm - 3;                      
          double heightOfContents = heightOfCup - fluidLevel;      
          double volumeOfCup = 23.39 * heightOfContents;           
          double convertToOunces = volumeOfCup * 0.0338;             
          previousSip = convertToOunces;                              //new measurement is the previousSip
          String data = "{ \"key\":\"Fluid Level\"," "\"value\":" + String(previousSip)+"}";
          // Trigger the integration
          Particle.publish("waterTracker", data, PRIVATE);            //this new information is published
          sumOfDrunk = currentSip - previousSip;                      //determines the amount that was drunk
          totalSum = totalSum + sumOfDrunk;                           //total amount that was drunk
          if (totalSum < 0){                                          //if there was an error in the distance sensor reading or otherwise, the total be 0 instead reading a negative number
            totalSum = 0;
          }
          String data2 = "{ \"key\":\"Fluid Drank\"," "\"value\":" + String(totalSum)+"}";
          Particle.publish("waterTracker", data2, PRIVATE);           //publish the total amount

          if (totalSum >= amountForUser) {                            //registers when the user has met their goal amount
            exit(0);                                                  //restarts the device
          }
          currentSip = previousSip;                                   //made the currSip to prevSip and counter to 0 to stop the ultrasonic distance measuring
          counter = 0;
        }
        if (startTimer - prevInterval > intervalForUserInMilliseconds){     //when the interval the user has put is reached while the device is horizontal the alarm will play
          // prevInterval = startTimer;
          for (int i = 0; i < arraySize(melody); i++) {               //loops over the first melody and plays the alarm song
            unsigned long currentTime = millis();                     //used millis to control function of the flashing light when the alarm is playing
            int noteTime = 1000 / duration[i];                        //each note is some fraction of one sec (quarter is 1/4 sec = .25 sec)
            tone(PIN_BUZZER, melody[i], noteTime);
            int pauseTime = noteTime * 1.3;
            delay(pauseTime);
            if (currentTime - prevTime > eventInterval) {             //if the currtime minus the prevtime exceeds the interval than
              prevTime = currentTime;                                 //prevTime will now equal the currtime
              if (LEDstate == LOW){                                   //if the LEDstate is low than it is turned to high
                LEDstate = HIGH;
              }
              else {                                                  //else if its high it is now low
                LEDstate = LOW;
              }
              digitalWrite(PIN_LED, LEDstate);                        //have the LED reflect the state that is chosen
            }
          }
          digitalWrite(PIN_LED, LOW);                                 //turning off the LED and delaying for 5 seconds after all this is done
          delay(5000);
          prevInterval = startTimer;                                  //adjusting the millis
        }  
        
      }
      else {
        int currentButtonState = digitalRead(PIN_BUTTON);             //when the device is not LOCKOUT
        if (currentButtonState == HIGH && lastButtonState == LOW){    //if there is a button press, meaning user wants to refill cup
          currentSip = 0;                                             //reset all the currentSip, PreviousSip and counter back to its original initialized values
          previousSip = 0;
          counter = 1;
          startTimer = millis();                                      //restart millis
          prevInterval = startTimer;
        }
        lastButtonState = currentButtonState;                         
        if (counter != 1) {                                           //if the button was not pressed then the counter changes to 3
          counter = 3;
        }
      }
    }      
  }
}
