/*
 * Project itp348_a4_Masters_Daniel
 * Description: Assignment 4: Constructing a dice game with random number generator that corresponds to a pattern that the LEDs will flash
 * Author: Daniel Masters
 * Date: 09/13/20
 */
long randNumber; //command for random number generator
const int PIN_BUTTON = A1; //initialize all the LEDs and button
const int PIN_1 = A2;
const int PIN_2 = D7;
const int PIN_3 = D6;
const int PIN_4 = D5;
const int PIN_5 = D4;
const int PIN_6 = D3;
const int PIN_7 = D2;
const int PIN_8 = A3;
const int PIN_9 = A4;
int firstPattern [9] = { 0, 0, 0, 0, 1, 0, 0, 0, 0 }; //six arrays; each match the pattern that the LEDs will light up
int secondPattern [9] = { 1, 0, 0, 0, 0, 0, 0, 0, 1 };
int thirdPattern [9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
int fourthPattern [9] = { 1, 0, 1, 0, 0, 0, 1, 0, 1 };
int fifthPattern [9] = { 1, 0, 1, 0, 1, 0, 1, 0, 1 };
int sixthPattern [9] = { 1, 1, 1, 0, 0, 0, 1, 1, 1 };
int ledArray [9] = { PIN_1, PIN_2, PIN_3, PIN_4, PIN_5, PIN_6, PIN_7, PIN_8, PIN_9 }; // big array used in the for loop
int curButton; //global

void setup() {
  pinMode(PIN_BUTTON, INPUT); //setup all leds to output and button to input
  pinMode(PIN_1, OUTPUT);
  pinMode(PIN_2, OUTPUT);
  pinMode(PIN_3, OUTPUT);
  pinMode(PIN_4, OUTPUT);
  pinMode(PIN_5, OUTPUT);
  pinMode(PIN_6, OUTPUT);
  pinMode(PIN_7, OUTPUT);
  pinMode(PIN_8, OUTPUT);
  pinMode(PIN_9, OUTPUT);
  // Put initialization like pinMode and begin functions here.
}


// loop() runs over and over again, as quickly as it can execute.
void loop() { 
 
  int* array; //initialize array to equal the pattern
  int curButton = analogRead(PIN_BUTTON); 

  if (curButton >= 2047) {  //divided 4095 by 2 to get 2047 and anything equal to or greater than that means the button is HIGH, anything lower than that is LOW
    randNumber = random(1, 7); //randNumber generates a number from 1 to 6
    if (randNumber == 1) {
      array = firstPattern; //if statements for each of the six pattern options
    }
    if (randNumber == 2) {
      array = secondPattern;
    }
    if (randNumber == 3) {
      array = thirdPattern;
    }
    if (randNumber == 4) {
      array = fourthPattern;
    }
    if (randNumber == 5) {
      array = fifthPattern;
    }
    if (randNumber == 6) {
      array = sixthPattern; 
    }
    for (int i= 0; i<9; i++) { //for loop loops over the pattern and will turn on the LED with the value 1 and LEDs with the value 0 will remain off
      int PIN = ledArray[i];
      if (array[i] == 0) {
        digitalWrite(PIN, LOW);
      }
      else {
        digitalWrite(PIN, HIGH);
      }
    }
    delay(2000); //holds the pattern for two seconds
  } 
  else { //keeps the LED off if button is not pushed
    digitalWrite(PIN_9, LOW);
    digitalWrite(PIN_5, LOW);
    digitalWrite(PIN_6, LOW);
    digitalWrite(PIN_1, LOW);
    digitalWrite(PIN_4, LOW);
    digitalWrite(PIN_3, LOW);
    digitalWrite(PIN_2, LOW);
    digitalWrite(PIN_7, LOW);
    digitalWrite(PIN_8, LOW);
  }
}