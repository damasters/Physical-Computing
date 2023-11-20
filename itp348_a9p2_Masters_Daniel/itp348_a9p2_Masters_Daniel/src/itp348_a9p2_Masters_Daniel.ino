/*
 * Project itp348_a9p2_Masters_Daniel
 * Description: bluetooth car
 * Author: Daniel Masters
 * Date: 10/30
 */
// based on
// https://learn.sparkfun.com/tutorials/sparkfun-inventors-kit-experiment-guide---v40

#include "argon_ble_setup.h"
/*
  Use Adafruit Bluefruit app
  (https://learn.adafruit.com/bluefruit-le-connect/controller)
  Connect to Argon -> controller

  CONFIGURATION
  Motor Controller
    PWMA  - D8
    AIN2  - D7
    AIN1  - D6
    BIN1  - D5
    BIN2  - D4
    PWMB  - D3



    ?? Questions
    VCC
    Standby
    VM
    enable
    
*/

// SYSTEM_MODE(MANUAL);  //uncomment to disable wifi and use BT only

// PIN VARIABLES
// the motor will be controlled by the motor A pins on the motor driver
const int AIN1 = D6;  // control pin 1 on the motor driver for the right motor
const int AIN2 = D7;  // control pin 2 on the motor driver for the right motor
const int PWMA = D8;  // speed control pin on the motor driver for the right motor
//controls speed from 0-255
// the left motor will be controlled by the motor B pins on the motor driver
const int PWMB = D3;  // speed control pin on the motor driver for the left
                      // motor
const int BIN2 = D4;  // control pin 2 on the motor driver for the left motor
const int BIN1 = D5;  // control pin 1 on the motor driver for the left motor



// VARIABLES
int motorSpeed = 125;      // starting speed for the motor
int motorLOWSpeed = 125;   // starting speed for the motor
int motorHIGHSpeed = 255;  // starting speed for the motor
bool isLowSpeed = true;




// onDataReceived() is event handler for incoming data via RX characteristic
// When the RXCharacteristic receives data, the event handler is called

// Note: uint8_t is a byte ("unsigned integer of length 8 bits")
void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer,
                    void* context) {
  /* === START DEBUG PRINTING ===
           uncomment following loop to see commands being sent by app ==
   */
  for (size_t ii = 0; ii < len; ii++) {
    Serial.print(data[ii]);
    Serial.print(" ");
  }
  Serial.println();
  for (size_t ii = 0; ii < len; ii++) {
    Serial.write(data[ii]);
    Serial.print(" ");
  }
  Serial.println();
  /* === END DEBUG PRINTING === */

  if (len > 4 ) {  // make sure there at least four bytes
    // the command scheme is from the Adafruit Bluefruit app
    // ex: [‘!’] [‘B’] [‘4’] [‘1’] [CRC]
    // https://learn.adafruit.com/bluefruit-le-connect/controller
    if (data[0] == '!' && data[1] == 'B' && data[2] == '5' && data[3] == '1') {
      carForward();
    }
    if (data[0] == '!' && data[1] == 'B' && data[2] == '5' && data[3] == '0') {
      carStop();
    }
    if (data[0] == '!' && data[1] == 'B' && data[2] == '6' && data[3] == '1') {
      carBackward();
    }
    if (data[0] == '!' && data[1] == 'B' && data[2] == '6' && data[3] == '0') {
      carStop();
    }
    if (data[0] == '!' && data[1] == 'B' && data[2] == '7' && data[3] == '1') {
      carLeft();
    }
    if (data[0] == '!' && data[1] == 'B' && data[2] == '7' && data[3] == '0') {
      carStop();
    }
    if (data[0] == '!' && data[1] == 'B' && data[2] == '8' && data[3] == '1') {
      carRight();
    }
    if (data[0] == '!' && data[1] == 'B' && data[2] == '8' && data[3] == '0') {
      carStop();
    }
    
  }
}

/*
some functions
carForward:
  leftmotor in forward direction
  rightmotor in forward direction
carBackward:
  swap directions, same as above
carRight:
  leftmotor moves by some amount
  rightmotor moves by some amount
carLeft:
  similar to carRight
carStop:
  any button is released than car stops
leftMotor "forward" is some amount
  "backward" is some amount
*/

void setup() {
  argon_ble_setup();
  // set the motor control pins as outputs
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);

  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  Serial.begin(9600);  // begin serial communication with the computer

  // the serial monitor.
}

void carForward(){
  // carForward:
  // leftmotor in forward direction
  // rightmotor in forward direction
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, motorSpeed);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, motorSpeed);
}
void carBackward(){
  // carBackward
  // leftmotor in opposite direction
  // right motor in opposite direction
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMA, motorSpeed);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  analogWrite(PWMB, motorSpeed);

}

void carRight(){
  // carRight:
  // leftmotor moves by some amount
  // rightmotor moves by some amount
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, motorSpeed);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, 0);
}

void carLeft(){
  // leftmotor moves by some amount
  // rightmotor moves by some amount
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, 0);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, motorSpeed);
}

void carStop(){
  // carStop:
  // any button is released than car stops
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMA, 0);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  analogWrite(PWMB, 0);
}

/********************************************************************************/
void loop() {}

