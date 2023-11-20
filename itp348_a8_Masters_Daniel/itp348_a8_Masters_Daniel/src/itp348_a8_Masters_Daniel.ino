/*
 * Project itp348_a8_Masters_Daniel
 * Description: Assignment #8 Alarm clock that connects to the cloud, plays tones and can snooze/stop alarm.
 * Author: Daniel Masters
 * Date: 10/12/20
 */
#include "pitches.h"
//Stage 1 Variables (used first/second Melody and first/second duration instead of just melodyNotes and melodyNotesDuration (Radhika said this was okay in office hours))
int firstMelody[] = {NOTE_E6, NOTE_D6, NOTE_C6, NOTE_D6, NOTE_E6, NOTE_E6, NOTE_E6, NOTE_D6, NOTE_D6, NOTE_D6, NOTE_E6, NOTE_G6, NOTE_G6, NOTE_E6, NOTE_D6, NOTE_C6, NOTE_D6, NOTE_E6, NOTE_E6, NOTE_E6, NOTE_E6, NOTE_D6, NOTE_D6, NOTE_E6, NOTE_D6, NOTE_C6};
int firstDuration[] = {4, 4, 4, 4, 4, 4, 2, 4, 4, 2, 4, 4, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2};
int secondMelody[] = {NOTE_G6, NOTE_G6, NOTE_D6, NOTE_D6, NOTE_E6, NOTE_E6, NOTE_D6, NOTE_C6, NOTE_C6, NOTE_B6, NOTE_B6, NOTE_A6, NOTE_A6, NOTE_G6, NOTE_D6, NOTE_D6, NOTE_C6, NOTE_C6, NOTE_B6, NOTE_B6, NOTE_A6, NOTE_D6, NOTE_D6, NOTE_C6, NOTE_C6, NOTE_B6, NOTE_B6, NOTE_A6, NOTE_G6, NOTE_G6, NOTE_D6, NOTE_D6, NOTE_E6, NOTE_E6, NOTE_D6, NOTE_C6, NOTE_C6, NOTE_B6, NOTE_B6, NOTE_A6, NOTE_A6, NOTE_G6};
int secondDuration[] = {4, 4, 4, 4, 4, 4, 2, 4, 4, 4, 4, 4, 4, 2, 4, 4, 4, 4, 4, 4, 2, 4, 4, 4, 4, 4, 4, 2, 4, 4, 4, 4, 4, 4, 2, 4, 4, 4, 4, 4, 4, 2};
const unsigned long eventInterval = 500; //interval for millis for LED
unsigned long prevTime = 0; //variable for millis for LED
unsigned long secondPrevTime = 0; //variable for millis for LED
int LEDstate = LOW; //for LED
//Stage 2 Variables
int buttonPress = 1;  //variable for enable/disable
int alarmChoice = 1; //variable for switching melodies
//Stage 3 variables
int playAlarm = 0; //variable for date/time widget for IFTTT
//Stage 4 variables
bool currButtonState = LOW;  //variables for monitoring the button state for both stopping the alarm and snoozing the alarm
bool prevButtonState = LOW;
bool isButtonPressed = false;
bool currSnoozeState = LOW;
bool prevSnoozeState = LOW;

const int PIN_BUZZER = D2; 
const int PIN_BUTTON1 = D3;
const int PIN_BUTTON2 = D4;
const int PIN_LED = D5;

// setup() runs once, when the device is first turned on.
void setup() {
  pinMode(PIN_BUTTON1, INPUT);
  pinMode(PIN_BUTTON2, INPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(9600);
  Particle.function("triggerEorD", triggerEorD); //functions used for communication with IFTTT
  Particle.function("cycleAlarms", cycleAlarms);
  Particle.function("dateAndTime", dateAndTime);
  
}

//STAGE 2 functions
int triggerEorD(String cmd) { 
  buttonPress += 1;                     //buttonPress always increases by 1 when the triggerEorD function is called
  int remainder = buttonPress % 2;      
  if (remainder == 0) {                  //if the remainder is zero, this means the buttonPress number is even
    buttonPress = 0;
  } else {                                //else the buttonPress number is odd
    buttonPress = 1;
  }
  return 0;
}

int cycleAlarms(String cmd) {
  alarmChoice += 1;                     //alarmChoice is increased by 1 each time IFTTT calls the function
  if (alarmChoice == 3) {               //if the alarmChoice is greater than 2 the alarmChoice goes back down to 1
    alarmChoice = 1;
  }
  return 0;
}

//STAGE 3 function
int dateAndTime(String cmd) {             //when the hour is reached, IFTTT will call this function
  playAlarm += 1;                         //alarm always increase by one when IFTTT calls the function
  if (playAlarm == 2) {
    playAlarm = 1;                        //if it is greater than 1 it goes back down to 1
  }
  return 0;
}


void loop() {
  //STAGE 2
  if (buttonPress == 0) {                                 // if the buttonPress == 0 that means the alarm is enabled
    if (alarmChoice == 1) {                                //alarmChoice defaults to 1 so first song is played
      for (int i = 0; i < arraySize(firstMelody); i++) {    //loops over the first melody and plays the alarm song
        unsigned long currentTime = millis();               //used millis to control function of the flashing light when the alarm is playing
        int noteTime = 1000 / firstDuration[i];       //each note is some fraction of one sec (quarter is 1/4 sec = .25 sec)
        tone(PIN_BUZZER, firstMelody[i], noteTime);
        int pauseTime = noteTime * 1.3;
        delay(pauseTime);
        if (currentTime - prevTime > eventInterval) {       //if the currtime minus the prevtime exceeds the interval than
          prevTime = currentTime;                           //prevTime will now equal the currtime
          if (LEDstate == LOW){                             //if the LEDstate is low than it is turned to high
            LEDstate = HIGH;
          }
          else {                                            //else if its high it is now low
            LEDstate = LOW;
          }
          digitalWrite(PIN_LED, LEDstate);                  //have the LED reflect the state that is chosen
        }
      }
      digitalWrite(PIN_LED, LOW);                            //turning off the LED and delaying for 5 seconds after all this is done
      delay(5000);
      buttonPress = 1;                                       //making sure buttonPress is one which disables it
    } else if (alarmChoice == 2) {
      for (int i = 0; i < arraySize(secondMelody); i++) {
        unsigned long currentTime = millis();
        int noteTime = 1000 / secondDuration[i];       
        tone(PIN_BUZZER, secondMelody[i], noteTime);
        int pauseTime = noteTime * 1.3;
        delay(pauseTime);
        Serial.println("play note");
        if (currentTime - prevTime > eventInterval) {
          prevTime = currentTime;
          if (LEDstate == LOW){
            LEDstate = HIGH;
          }
          else {
            LEDstate = LOW;
          }
          digitalWrite(PIN_LED, LEDstate);
        }
      }
      digitalWrite(PIN_LED, LOW);
      delay(5000);
      buttonPress = 1;
    }
  //STAGE 3 & 4
  } else if (playAlarm == 1) {                                                          //triggered when date and time is set off by IFTTT
    if (alarmChoice == 1) {
      for (int i = 0; i < arraySize(firstMelody) && isButtonPressed == false; i++) {    //for loop checks for isButtonPressed which is initialized as false
        int currButtonState = digitalRead(PIN_BUTTON2);
        int currSnoozeState = digitalRead(PIN_BUTTON1);
        unsigned long currentTime = millis();
        int noteTime = 1000 / firstDuration[i];       
        tone(PIN_BUZZER, firstMelody[i], noteTime);
        int pauseTime = noteTime * 1.3;
        delay(pauseTime);
        if (currButtonState != prevButtonState) {                                         //if the currButtonState and the prevButtonState are not the same
          if (currButtonState == HIGH) {                                                  //and the currButtonState is HIGH, this means that the button was pressed
            isButtonPressed = true;                                                       //isButtonPressed = true which breaks out of for loop
            playAlarm = 0;                                                                //playArarm = 0 stops it from repeating
          }
        }
        if (currSnoozeState != prevSnoozeState) {                                         //same initial logic as currButtonState and prevButtonState
          if (currSnoozeState == HIGH){                                                   //instead of breaking out of loop, just used delay
            delay(300000);                                                                  //delay for 5 minutes
          }
        }
        if (currentTime - prevTime > eventInterval) {
          prevTime = currentTime;
          if (LEDstate == LOW){
            LEDstate = HIGH;
          }
          else {
            LEDstate = LOW;
          }
          digitalWrite(PIN_LED, LEDstate);
        } 
      }
      digitalWrite(PIN_LED, LOW);
      delay(5000);
      isButtonPressed = false;                                                              //put isButtonPressed back to false
    } else if (alarmChoice == 2) {                                                          //same logic and code but for when the alarmChoice is 2
      for (int i = 0; i < arraySize(secondMelody) && isButtonPressed == false; i++) {
        int currButtonState = digitalRead(PIN_BUTTON2);
        int currSnoozeState = digitalRead(PIN_BUTTON1);
        unsigned long currentTime = millis();
        int noteTime = 1000 / secondDuration[i];       
        tone(PIN_BUZZER, secondMelody[i], noteTime);
        int pauseTime = noteTime * 1.3;
        delay(pauseTime);
        if (currButtonState != prevButtonState) {
          if (currButtonState == HIGH) {
            isButtonPressed = true;
            playAlarm = 0;
          }
        }
        if (currSnoozeState != prevSnoozeState) {
          if (currSnoozeState == HIGH){
            delay(300000);
          }
        }
        if (currentTime - prevTime > eventInterval) {
          prevTime = currentTime;
          if (LEDstate == LOW){
            LEDstate = HIGH;
          }
          else {
            LEDstate = LOW;
          }
          digitalWrite(PIN_LED, LEDstate);
        }
      }
      digitalWrite(PIN_LED, LOW);
      delay(5000);
      isButtonPressed = false;
    }
  }
}  
    
