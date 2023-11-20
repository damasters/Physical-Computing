/*
 * Project Assignment #2
 * Description: Created code that turns LEDs on, makes them blink, then turns them off. 
 * Author: Daniel Masters
 * Date: 08/31/20
 */

// setup() runs once, when the device is first turned on.
// setup global variables for each pin
const int PIN_1 = D2;
const int PIN_2 = D3;
const int PIN_3 = D4;
const int PIN_4 = D5;
const int PIN_5 = D6;

void setup() { //initialized each pin as output
  pinMode(PIN_1, OUTPUT);
  pinMode(PIN_2, OUTPUT);
  pinMode(PIN_3, OUTPUT);
  pinMode(PIN_4, OUTPUT);
  pinMode(PIN_5, OUTPUT);
  // Put initialization like pinMode and begin functions here.

}

// loop() runs over and over again, as quickly as it can execute.
void loop() { //starts off by turning on all LEDs
  digitalWrite(PIN_1, HIGH);
  digitalWrite(PIN_2, HIGH);
  digitalWrite(PIN_3, HIGH);
  digitalWrite(PIN_4, HIGH);
  digitalWrite(PIN_5, HIGH);
  delay(5000); //holds LEDs on for 5 seconds
  int i = 0;
  while (i < 10) { //created while loop which turns off the LED's for 500 milliseconds, after this time is up he LED will turn on for 500 milliseconds. This will loop for as long as i is less than 10
    digitalWrite(PIN_1, LOW);
    digitalWrite(PIN_2, LOW);
    digitalWrite(PIN_3, LOW);
    digitalWrite(PIN_4, LOW);
    digitalWrite(PIN_5, LOW);
    delay(500);
    digitalWrite(PIN_1, HIGH);
    digitalWrite(PIN_2, HIGH);
    digitalWrite(PIN_3, HIGH);
    digitalWrite(PIN_4, HIGH);
    digitalWrite(PIN_5, HIGH);
    delay(500);
    i++; //adds one to i
  } //after the while loop is broken, i > 10, then the LEDs will turn off for 5 seconds
  digitalWrite(PIN_1, LOW);
  digitalWrite(PIN_2, LOW);
  digitalWrite(PIN_3, LOW);
  digitalWrite(PIN_4, LOW);
  digitalWrite(PIN_5, LOW);
  delay(5000);

}