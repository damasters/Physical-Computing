/*
 * Project itp348_a3_Masters_Daniel
 * Description: Create 5 scanning/oscillating LEDs that get brighter and scan/oscillate faster as the potentiometer is turned clockwise
 * Author: Daniel Masters
 * Date: 9/7/20
 */
// created global variables for the potentiometer and LEDs
const int PIN_POT = A0;
const int LED_1 = D3;
const int LED_2 = D4;
const int LED_3 = D5;
const int LED_4 = D6;
const int LED_5 = D7;
// setup() runs once, when the device is first turned on.

void setup() { //setup the LEDs to output and the potentiometer to input
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
  pinMode(LED_5, OUTPUT);
  pinMode(PIN_POT, INPUT);


  // Put initialization like pinMode and begin functions here.

}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  int value = analogRead(PIN_POT);
  delay(500);
  // convert out potVal to a PWM val for LED
  // potentiometer value: 0-4095
  // pwm value: 0-255
  // int pwmValue = (value * 255) / 4095; this makes LED's turn off when the value is 0
  int pwmValue = ((value + 20) * 255) / 4095; //added 20 to the value so that LED would not shut off when the value is 0
  // delayTime based on the pwmValue
  int delayTime = (255 / pwmValue) * 50;
  // analogwrite to control the brightness of the LED based on the pwm value
  // digitalwrite to turn off the LED 
  // delay based on the brightness of the potentiometer 
  // brighter = faster. dimmer = slower
  analogWrite(LED_1, pwmValue);
  delay(delayTime);
  digitalWrite(LED_1, LOW);
  analogWrite(LED_2, pwmValue);
  delay(delayTime);
  digitalWrite(LED_2, LOW);
  analogWrite(LED_3, pwmValue);
  delay(delayTime);
  digitalWrite(LED_3, LOW);
  analogWrite(LED_4, pwmValue);
  delay(delayTime);
  digitalWrite(LED_4, LOW);
  analogWrite(LED_5, pwmValue);
  delay(delayTime);
  digitalWrite(LED_5, LOW);
  analogWrite(LED_4, pwmValue);
  delay(delayTime);
  digitalWrite(LED_4, LOW);
  analogWrite(LED_3, pwmValue);
  delay(delayTime);
  digitalWrite(LED_3, LOW);
  analogWrite(LED_2, pwmValue);
  delay(delayTime);
  digitalWrite(LED_2, LOW);
  analogWrite(LED_1, pwmValue);
  delay(delayTime);
  digitalWrite(LED_1, LOW);
}