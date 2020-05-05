#include <Servo.h>

//Servo Claw;
Servo Extend;

int pos = 0;

// Motor 1 Pins  
#define Motor1_Forward 34    //P2_3
#define Motor1_Backward 35   //P6_7

// Motor 2 Pins
#define Motor2_Forward 31    //P3_7
#define Motor2_Backward 32   //P3_5

// Claw Pins  
#define Claw_Open 40         //P6_5, P2_7
#define Claw_Extend 37      //P6_4, P5_6

// TCS3200 Pins
#define S0 11               //P3_6
#define S1 12               //P5_2
#define S2 13               //P5_0
#define S3 14               //P1_7
#define sensorOut 15        //P1_6
#define OE 39               //P2_6

// Variables used to send message
char input;
char Message;
char Color;

// Store frequency read by the photodiodes
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

// Store RGB color values
int redColor = 0;
int greenColor = 0;
int blueColor = 0;

bool foundColor = false;

void config_ColorSensor(){
  // Setting the outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OE, OUTPUT);
  
  // Setting the sensorOut as an input
  pinMode(sensorOut, INPUT);
  
  // Setting frequency scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  digitalWrite(OE,LOW);
}

void config_Motors(){
  // Setting the outputs
  pinMode(Motor1_Forward, OUTPUT);
  pinMode(Motor1_Backward, OUTPUT);
  pinMode(Motor2_Forward, OUTPUT);
  pinMode(Motor2_Backward, OUTPUT);
}

void findColor(char color){
  while(!foundColor){
      // Measure RED value
      digitalWrite(S2, LOW);
      digitalWrite(S3, LOW);
      redFrequency = pulseIn(sensorOut, LOW);
      redColor = map(redFrequency, 8, 48, 255, 0);      // Replace values after calibration (8, 48)

      // Measure BLUE value
      digitalWrite(S2, LOW);
      digitalWrite(S3, HIGH); 
      blueFrequency = pulseIn(sensorOut, LOW);
      blueColor = map(blueFrequency, 11, 44, 255, 0);   // Replace values after calibration (11, 44)

      // Measure GREEN value
      digitalWrite(S2, HIGH);
      digitalWrite(S3, HIGH);
      greenFrequency = pulseIn(sensorOut, LOW);
      greenColor = map(greenFrequency, 11, 80, 255, 0); // Replace values after calibration (11, 80)

      // Check if color is found (RED, GREEN, BLUE, YELLOW, WHITE)
      switch (color){
        case 'R':
          if(redColor > greenColor && redColor > blueColor){
            if(redColor > 0 && greenColor > 0 && blueColor > 0){
                foundColor = true;
            }
          }
          break;
        case 'G':
          if(greenColor > redColor && greenColor > blueColor){
            if(redColor > 0 && greenColor > 0 && blueColor > 0){
              foundColor = true;
            }
          }
          break;
        case 'B':
          if(blueColor > redColor && blueColor > greenColor){
            if(redColor > 0 && greenColor > 0 && blueColor > 0){
                foundColor = true;
            }
          }
          break;
      }
  }
}

void moveForward(){
  // Send PWM signal to Motor1_Forward pin (P2_3)
  analogWrite(Motor1_Forward, 32); //0-255
  // Set Motor1_Backward pin LOW (P6_7)
  analogWrite(Motor1_Backward, 0);
  
  // Send PWM signal to Motor2_Forward pin (P3_7)
  analogWrite(Motor2_Forward, 32); //0-255
  // Set Motor2_Backward pin LOW (P3_5)
  analogWrite(Motor2_Backward, 0);
}

void moveBackward(){
  // Send PWM signal to Motor1_Backward pin (P4_3)
  analogWrite(Motor1_Backward, 32); //0-255
  // Set Motor1_Forward pin LOW (P4_1)
  analogWrite(Motor1_Forward, 0);
  
  // Send PWM signal to Motor2_Backward pin (P4_6)
  analogWrite(Motor2_Backward, 32); //0-255
  // Set Motor2_Forward pin LOW (P1_5)
  analogWrite(Motor2_Forward, 0);
}

void brake(){
  // Set Motor1_Forward and Motor1_Backward HIGH
  digitalWrite(Motor1_Forward, HIGH);
  digitalWrite(Motor1_Backward, HIGH);
  // Set Motor2_Forward and Motor2_Backward HIGH
  digitalWrite(Motor2_Forward, HIGH);
  digitalWrite(Motor2_Backward, HIGH);
}

void openClaw(){
  for(pos = 0; pos < 180; pos++){
    //Claw.write(pos);
    delay(5);
  }
}

void closeClaw(){
  for(pos = 180; pos >= 1 ; pos--){
    //Claw.write(pos);
    delay(5);
  }
}

void extendClaw(){
  for(pos = 180; pos >= 0; pos--){
    //Extend.write(pos);
    delay(5);
  }
}

void retractClaw(){
  for(pos = 0; pos < 180 ; pos++){
    //Extend.write(pos);
    delay(5);
  }
}

void sendMessage(char Data){
  // Notify WiFi module Data is available to send
  while(Serial1.available() <= 0){
    Serial1.write('x');
    // Break loop when WiFi module responds
    if(Serial1.available() > 0){
      break;
    }
    delay(1000);
  }
  // Clear buffer 
  input = Serial1.read();
  // Send Message Data to WiFi module
  Serial1.write(Data);
}

void processMessage(char Data){
  if(Data == 'R' || Data == 'G' || Data == 'B'){
    Data = Color;
    getItem(Color);
  }
  else if(Data == 'r' || Data == 'g' || Data == 'b'){
    Data = Data - 0x20;
    if(Data == Color){
      sendMessage('M');
      goHome();
    }
    else{
      goHome();
    }
  }
  else if(Data == 'x'){
    input = Serial1.read();
  }
}

void getItem(char color){
  moveForward();
  findColor(color);
  foundColor = false;
  brake();
  for(pos = 255; pos >= 0; pos--){
    analogWrite(Claw_Extend, pos);
    delay(5);
  }
    delay(5000);
  for(pos = 0; pos < 255; pos++){
    analogWrite(Claw_Extend, pos);
    delay(5);
  }

  //openClaw();
  //extendClaw();
  //closeClaw();
  //retractClaw();
  //sendMessage('G');
}

void givetoDeliver(){
  moveForward();
  findColor('G');   // Change Color 
  foundColor = false;
  brake();
  extendClaw();
  openClaw();
  retractClaw();
  closeClaw();
}

void goHome(){
  moveBackward();
  findColor('H');   // Change Color 
  foundColor = false;
  brake();
  sendMessage('H');
}

void setup() {
  config_ColorSensor(); 
  config_Motors();
//  Claw.attach(Claw_Open);
  Extend.attach(Claw_Extend);
  //pinMode(Claw_Extend, OUTPUT);
  pinMode(Claw_Open,OUTPUT);
  Serial.begin(9600);
  Serial1.begin(115200); 
}

void loop() {
//  if(Serial1.available() > 0){
//    Message = Serial1.read();
//    processMessage(Message);   
//  }
//  getItem('R');
//  delay(2000);
//  getItem('B');
//  moveBackward();
//  delay(5000);  
//  getItem('G');
//  delay(2000);


}
