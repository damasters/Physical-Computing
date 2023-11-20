/*
 * Project itp348_a7_Masters_Daniel_v2
 * Description: Assignment #7: Get temperature readings from your TMP 36 and post them to the cloud. 
 * Import fellow students temperature readings by subscribing to ITP348-ENVIRO. 
 * Display your temp, the average cloud temp and the maximum cloud temp on your OLED. 
 * Create a second display that shows the six most recent temperatures.
 * Use a button to flip between the screens.
 * Author: Daniel Masters
 * Date: 10/6/20
 */
 
#include "SparkFunMicroOLED.h"  // Include MicroOLED library
#include "math.h"
 
//needed variables: 
double temperatures[20] = {};   
 
int currentIndex = 0;
int counter = 0;
//temperature sensor variables:
int tmpPin = A5;
int PIN_BUTTON = D5;
double tempC = 0;
double tempF = 0;
//MicroLED display variables
int avgTemp = 0;
int maxTemp = 0;
int sumOfArray = 0;
MicroOLED oled;
//Button variables
int screenSwitch = 1;
 
 
// setup() runs once, when the device is first turned on.
void setup() {
  pinMode(tmpPin, INPUT);
  pinMode(PIN_BUTTON, INPUT);
  oled.begin();                               // Initialize the OLED
  oled.clear(ALL);                            // Clear the display's internal memory
  oled.display();                             // Display what's in the buffer (splashscreen)
  delay(1000);                                // Delay 1000 ms
  Serial.begin(9600);
  Particle.subscribe("ITP348-ENVIRO", enviroHandler, ALL_DEVICES);
}
 
void enviroHandler(const char *event, const char *data) {
  double eventData = atof(data);              //atof converts string into double
  String eventName = String(event); 
  if (eventName.equals("ITP348-ENVIRO")) {    //if the eventName starts with ITP 348
    temperatures[currentIndex] = eventData;   //fill data in the currentIndex
    currentIndex = currentIndex + 1;          //increase current index by 1
    counter = counter + 1;                    //increase the counter (use for average)
    if (currentIndex == 20){                  //if current index is equal to 20
      currentIndex = 0;                       //restarts the array, rewrites starting from beginning
    }
    if (counter == 20){                       //if the counter is equal to 20
      counter = counter + 0;                  //increase counter by 0, stays at 20 because there will always be 20 values
    }
  }
}
 
void loop() {
  int sensorValue = analogRead(tmpPin);                     //convert the value that the that is read in the sensorValue to voltage, value is between 0-4095
  double voltageValue = sensorValue * 3.3 / 4095;
  tempC = (voltageValue - 0.5) * 100;
  tempF = tempC * (9.0/5.0)  + 32;
  Particle.publish("ITP348-ENVIRO/DAM", String(tempF), PUBLIC);  //publish the farenheit temperatures to the cloud
 
  sumOfArray = 0;
  avgTemp = 0;
  maxTemp = 0;
  for (int i=0; i<20; i++){
    sumOfArray += temperatures[i];                            //find the sum of all elements in the array (make sure this is a double)?                                                           
    avgTemp = sumOfArray / counter;                           //divide the sum of all elements in the array by the counter variable, this gives us the average temperature
    if (temperatures[i] > maxTemp) {                          //iterate over all temperatures and change max each time the new info is greater than max
      maxTemp = temperatures[i];
    }  
 
  }
 
  //button press
  int currButtonVal = digitalRead(PIN_BUTTON);
  if (currButtonVal == HIGH) {
    screenSwitch = screenSwitch + 1;              //adds one to screenSwitch Variable
    if (screenSwitch % 2 == 0){                   //if the screenSwitch is an even number it will display the following
      oled.clear(PAGE);                           //clear page clears the LED screen
      oled.setCursor(0,0);                        // Set cursor to top-left 
      oled.setFontType(0);                       
      oled.println("YourTemp:");                  //display my temperature reading
      oled.println(tempF, 2);                     // x,y coordinates
      oled.println("AvgTemp:");                   //display avg of 20 temperatures
      oled.println(avgTemp, 2);
      oled.println("MaxTemp:");                   //display the 6 most recent cloud temperatures
      oled.println(maxTemp, 2);
      oled.display();                             
    } else {                                      //else when the screenSwitchValue is odd it will display the 6 most recent temperatures that arrive from the cloud
      oled.clear(PAGE);
      oled.setCursor(0,0);
      oled.setFontType(0);
      for (int i = 0; i < 6; i++) {
        oled.println(temperatures[i], 2);
      }
      oled.display();
    }
  }
}
