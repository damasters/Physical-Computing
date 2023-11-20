/*
 * Project itp348_a5_Masters_Daniel
 * Description: Assignment 5 Dice Game that involves two players
 * Author: Daniel Masters
 * Date: 9/19/20
 */
long randNumber; //used this to initialize the randNumber variable
const int button1 = A2; //initialized all buttons and RGBLED
const int button2 = A3;
const int button3 = A4;
const int button4 = A5;
const int redPin = D2;
const int greenPin = D3;
const int bluePin = D4;
//unsigned long prevMillisRed = 0;


int player1; //this stores the player times
int player2;
int player1Counter = 0; //this sotres the player scors
int player2Counter = 0;
int player1Flag = -1; //at -1; this means the player has not pressed any buttons. at 0; this means the player pressed the wrong button. at 1; this means the player pressed the correct button
int player2Flag = -1;
int flashFlag = 1;


// setup() runs once, when the device is first turned on.
void setup() {
  pinMode(button1, INPUT); //setup all buttons as input and LEDs as outputs
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  pinMode(button4, INPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  Serial.begin(9600);
  // Put initialization like pinMode and begin functions here.

}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  int value1 = digitalRead(button1); //reading the values of each button
  int value2 = digitalRead(button2);
  int value3 = digitalRead(button3);
  int value4 = digitalRead(button4);
  if (flashFlag == 1) {
    int randVal = random(500, 1001); //code for blinking and delaying for randomized periods of time
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, HIGH);
    digitalWrite(bluePin, HIGH);
    delay(randVal);
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);
    delay(randVal);
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, HIGH);
    digitalWrite(bluePin, HIGH);
    delay(randVal);
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);
    delay(randVal);
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, HIGH);
    digitalWrite(bluePin, HIGH);
    delay(randVal);
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);
    delay(randVal);
    randNumber = random(1, 3); //rand number picking a value that is either 1 or 2
    if (randNumber == 1) { //if the rand number is 1 it is red
      digitalWrite(redPin, HIGH); //flashes for half a second
      delay(500);
      digitalWrite(redPin, LOW);
    }
    if (randNumber == 2) {
      digitalWrite(bluePin, HIGH);
      delay(500);
      digitalWrite(bluePin, LOW);
    }
    flashFlag = 0; //making flashflag 0 means that the LED is done flashing
  }
  if (randNumber == 1) { 
    unsigned long flashMillis = millis(); //flashmillis marks the start time
    if (value1 == HIGH) { //value1 = the value from the red button: player 1
      int player1Time = millis(); //player1time measures the current time from the flashmillis to when the button is pressed
      player1 = (player1Time - flashMillis); //player1 measures the time from the flashmillis to when the button is pressed represented by player1Time
      player1Flag = 1; //playerflag is set to 1 to show that it is correct
    } 
    else if (value2 == HIGH) { //value2 = the value from the blue button: player 1
      player1Counter = player1Counter - 100; //incorrect answer the player gets minus 100 points
      player1Flag = 0; //playerflag is set to 0 to show that it is incorrect
    }
    else if (value3 == HIGH) { //value3 = the value from the red button (on my argon it is yellow): player 2
      player2Counter = player2Counter - 100;
      player2Flag = 0;
    }
    else if (value4 == HIGH) { //value4= the value from the blue button (on my argon it is green); player 2
      int player2Time = millis();
      player2 = (player2Time - flashMillis);
      player2Flag = 1; 
    //if the player does not press a button the player flag will remain -1
    }
    if (player1Flag == 1 && player2Flag == 1){ //goes through the case of if the both players are correct
      if (player1 > player2) { //if player 2 time is less than player 1 time that means player2 wins
        Serial.println("Player 2 wins. They are rewarded with 100pts."); 
        player2Counter = player2Counter + 100;
        Serial.println("Player 2 Cummulative Score: " +String(player2Counter)); //shows cumulative score
        Serial.println("Player 1 Cummulative Score: " +String(player1Counter));
        Serial.println('\n');
        player1Flag = -1; //playerflags are reset 
        player2Flag = -1; 
        flashFlag = 1; //flashflag is reset which marks the end of a round and the device can start blinking again
      }
      else {    //else player1 wins
        Serial.println("Player 1 wins. They are rewarded with 100pts.");
        player1Counter = player1Counter + 100;
        Serial.println("Player 1 Cummulative Score: " +String(player1Counter));
        Serial.println("Player 2 Cummulative Score: " +String(player2Counter));
        Serial.println('\n');
        flashFlag = 1;
        player1Flag = -1;
        player2Flag = -1;
      } 
    }
    else if (player1Flag == 1 && player2Flag == 0){ //when player1 wins and player2 presses the wrong button
      Serial.println("Player 1 wins. They are rewarded with 100pts.");
      player1Counter = player1Counter + 100;
      Serial.println("Player 1 Cummulative Score: " +String(player1Counter));
      Serial.println("Player 2 Cummulative Score: " +String(player2Counter));
      Serial.println('\n');
      flashFlag = 1;
      player1Flag = -1;
      player2Flag = -1;
    }
    else if (player1Flag == 0 && player2Flag == 1){ //when player1 presses the wrong button and player2 wins
      Serial.println("Player 2 wins. They are rewarded with 100pts.");
      player2Counter = player2Counter + 100;
      Serial.println("Player 2 Cummulative Score: " +String(player2Counter));
      Serial.println("Player 1 Cummulative Score: " +String(player1Counter));
      Serial.println('\n');
      flashFlag = 1;
      player1Flag = -1;
      player2Flag = -1;
    }
    else if (player1Flag == 0 && player2Flag == 0){ //when neither of the two players presses the correct button
      Serial.println("Neither player pushed the correct button. Both players were deducted 100pts");
      Serial.println("Player 1 Cummulative Score: " +String(player1Counter));
      Serial.println("Player 2 Cummulative Score: " +String(player2Counter));
      Serial.println('\n');
      flashFlag = 1;
      player1Flag = -1;
      player2Flag = -1;
    }
    else if (player1Flag == -1 && player2Flag == 0){ //when player1 does not press a button and player2 presses the incorrect one
      player1Counter = player1Counter - 100;
      Serial.println("Player 1 did not press a button: -100pts.");
      Serial.println("Player 2 pressed the incorrect button: -100pts.");
      Serial.println("Player 1 Cummulative Score: " +String(player1Counter));
      Serial.println("Player 2 Cummulative Score: " +String(player2Counter));
      Serial.println('\n');
      flashFlag = 1;
      player1Flag = -1;
      player2Flag = -1;
    }
    else if (player1Flag == -1 && player2Flag == 1){ //when player1 does not press a button and player2 presses the correct one
      player1Counter = player1Counter - 100;
      player2Counter = player2Counter + 100;
      Serial.println("Player 1 did not press a button: -100pts.");
      Serial.println("Player 2 pressed the correct button: +100pts.");
      Serial.println("Player 1 Cummulative Score: " +String(player1Counter));
      Serial.println("Player 2 Cummulative Score: " +String(player2Counter));
      Serial.println('\n');
      flashFlag = 1;
      player1Flag = -1;
      player2Flag = -1;
    }
    else if (player1Flag == 0 && player2Flag == -1){ //when player1 presses the wrong button and player2 does not press a button
      player2Counter = player2Counter - 100; 
      Serial.println("Player 1 pressed the incorrect button: -100pts.");
      Serial.println("Player 2 did not press a button: -100pts.");
      Serial.println("Player 1 Cummulative Score: " +String(player1Counter));
      Serial.println("Player 2 Cummulative Score: " +String(player2Counter));
      Serial.println('\n');
      flashFlag = 1;
      player1Flag = -1;
      player2Flag = -1;
    }
    else if (player1Flag == 1 && player2Flag == -1){ //when player1 presses the correct button and player2 does not press a button
      player1Counter = player1Counter + 100;
      player2Counter = player2Counter - 100;
      Serial.println("Player 1 pressed the correct button: +100pts.");
      Serial.println("Player 2 did not press a button: -100pts.");
      Serial.println("Player 1 Cummulative Score: " +String(player1Counter));
      Serial.println("Player 2 Cummulative Score: " +String(player2Counter));
      Serial.println('\n');
      flashFlag = 1;
      player1Flag = -1;
      player2Flag = -1;
    }   
  } else { //else the LED flashes blue. same process as the when it is red except the when the blue button is pressed playerFlags will be 1 and if the red button is pressed the playerFlags will be 0
    unsigned long flashMillis = millis();
    if (value1 == HIGH) {
      player1Counter = player1Counter - 100;
      player1Flag = 0;
    }
    if (value2 == HIGH) {
      int player1Time = millis();
      player1 = (player1Time - flashMillis);
      player1Flag = 1;
    }
    if (value3 == HIGH) {
      int player2Time = millis();
      player2 = (player2Time - flashMillis);
      player2Flag = 1;
    }
    if (value4 == HIGH) {
      player2Counter = player2Counter - 100;
      player2Flag = 0;
    }
    if (player1Flag == 1 && player2Flag == 1){
      if (player1 > player2) {
        Serial.println("Player 2 wins. They are rewarded with 100pts.");
        player2Counter = player2Counter + 100;
        Serial.println("Player 2 Cummulative Score: " +String(player2Counter));
        Serial.println("Player 1 Cummulative Score: " +String(player1Counter));
        Serial.println('\n');
        flashFlag = 1;
        player1Flag = -1;
        player2Flag = -1;
      }
      else {
        Serial.println("Player 1 wins. They are rewarded with 100pts.");
        player1Counter = player1Counter + 100;
        Serial.println("Player 1 Cummulative Score: " +String(player1Counter));
        Serial.println("Player 2 Cummulative Score: " +String(player2Counter));
        Serial.println('\n');
        flashFlag = 1;
        player1Flag = -1;
        player2Flag = -1;
      } 
    }
    else if (player1Flag == 1 && player2Flag == 0){
      Serial.println("Player 1 wins. They are rewarded with 100pts.");
      player1Counter = player1Counter + 100;
      Serial.println("Player 1 Cummulative Score: " +String(player1Counter));
      Serial.println("Player 2 Cummulative Score: " +String(player2Counter));
      Serial.println('\n');
      flashFlag = 1;
      player1Flag = -1;
      player2Flag = -1;
    }
    else if (player1Flag == 0 && player2Flag == 1){
      Serial.println("Player 2 wins. They are rewarded with 100pts.");
      player2Counter = player2Counter + 100;
      Serial.println("Player 2 Cummulative Score: " +String(player2Counter));
      Serial.println("Player 1 Cummulative Score: " +String(player1Counter));
      Serial.println('\n');
      flashFlag = 1;
      player1Flag = -1;
      player2Flag = -1;
    }
    else if (player1Flag == 0 && player2Flag == 0){
      Serial.println("Neither player pushed the correct button. No points were rewarded");
      Serial.println("Player 1 Cummulative Score: " +String(player1Counter));
      Serial.println("Player 2 Cummulative Score: " +String(player2Counter));
      Serial.println('\n');
      flashFlag = 1;
      player1Flag = -1;
      player2Flag = -1;
    }
    else if (player1Flag == -1 && player2Flag == 0){
      player1Counter = player1Counter - 100;
      Serial.println("Player 1 did not press a button: -100pts.");
      Serial.println("Player 2 pressed the incorrect button: -100pts.");
      Serial.println("Player 1 Cummulative Score: " +String(player1Counter));
      Serial.println("Player 2 Cummulative Score: " +String(player2Counter));
      Serial.println('\n');
      flashFlag = 1;
      player1Flag = -1;
      player2Flag = -1;
    }
    else if (player1Flag == -1 && player2Flag == 1){
      player1Counter = player1Counter - 100;
      player2Counter = player2Counter + 100;
      Serial.println("Player 1 did not press a button: -100pts.");
      Serial.println("Player 2 pressed the correct button: +100pts.");
      Serial.println("Player 1 Cummulative Score: " +String(player1Counter));
      Serial.println("Player 2 Cummulative Score: " +String(player2Counter));
      Serial.println('\n');
      flashFlag = 1;
      player1Flag = -1;
      player2Flag = -1;
    }
    else if (player1Flag == 0 && player2Flag == -1){
      player2Counter = player2Counter - 100;
      Serial.println("Player 1 pressed the incorrect button: -100pts.");
      Serial.println("Player 2 did not press a button: -100pts.");
      Serial.println("Player 1 Cummulative Score: " +String(player1Counter));
      Serial.println("Player 2 Cummulative Score: " +String(player2Counter));
      Serial.println('\n');
      flashFlag = 1;
      player1Flag = -1;
      player2Flag = -1;
    }
    else if (player1Flag == 1 && player2Flag == -1){
      player1Counter = player1Counter + 100;
      player2Counter = player2Counter - 100;
      Serial.println("Player 1 pressed the correct button: +100pts.");
      Serial.println("Player 2 did not press a button: -100pts.");
      Serial.println("Player 1 Cummulative Score: " +String(player1Counter));
      Serial.println("Player 2 Cummulative Score: " +String(player2Counter));
      Serial.println('\n');
      flashFlag = 1; 
      player1Flag = -1;
      player2Flag = -1;
    }
  }
}



