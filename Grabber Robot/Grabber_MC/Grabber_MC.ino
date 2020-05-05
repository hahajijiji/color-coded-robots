#include <Motor.h>
#include <ColorSensor.h>
#include <Servo.h>

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

/* 
 *  Create Motor Object
 *  Motor 1 Forward   Pin 34 (P2_3)
 *  Motor 1 Backward  Pin 35 (P6_7)
 *  Motor 2 Forward   Pin 31 (P3_7)
 *  Motor 2 Backward  Pin 32 (P3_5)
 */
Motor motor;

/*
 * Create Color Sensor Object
 * S0         Pin 11 (P3_6)
 * S1         Pin 12 (P5_2)
 * S2         Pin 13 (P5_0)
 * S3         Pin 14 (P1_7)
 * sensorOut  Pin 15 (P1_6)
 * OE         Pin 39 (P2_6)
 */
 ColorSensor colorSensor;

Servo Claw;
Servo Extend;

int pos = 0;

// Variables used to send message
char input;
char Message;
char Color;

void openClaw(){
  analogWrite(Claw_Open, 0);
  delay(15);
  analogWrite(Claw_Open, 255);
  delay(15);
}

void closeClaw(){
  analogWrite(Claw_Open, 0);
  delay(1000);
  analogWrite(Claw_Open, 255);
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
  motor.FORWARD(32);
  colorSensor.findColor(color);
  motor.BRAKE();
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
  motor.FORWARD(32);
  colorSensor.findColor('G');   // Change Color 
  motor.BRAKE();
  extendClaw();
  openClaw();
  retractClaw();
  closeClaw();
}

void goHome(){
  motor.BACKWARD(32);
  colorSensor.findColor('H');   // Change Color 
  motor.BRAKE();
  sendMessage('H');
}

void setup() {
  colorSensor.SETUP(S0, S1, S2, S3, sensorOut, OE); 
  motor.SETUP();
  //Claw.attach(Claw_Open);
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
//  motor.BACKWARD(32);
//  delay(5000);  
//  getItem('G');
//  delay(2000);
  closeClaw();
}
