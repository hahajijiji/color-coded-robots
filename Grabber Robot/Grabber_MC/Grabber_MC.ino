#include <Motor.h>
#include <ColorSensor.h>
#include <Servo.h>
  
#define clawPin 40         //P2_7

/* 
 *  Create Motor Objects
 *  Motor 1 Forward       Pin 34 (P2_3)
 *  Motor 1 Backward      Pin 35 (P6_7)
 *  Motor 2 Forward       Pin 31 (P3_7)
 *  Motor 2 Backward      Pin 32 (P3_5)
 *  Extend Motor Forward  Pin 37 (P5_6)
 *  Extend Motor Backward Pin 36 (P6_6)
 */
Motor motor1(34, 35);
Motor motor2(31, 32);
Motor extend(37, 36);

/*
 * Create Color Sensor Object
 * S0         Pin 11 (P3_6)
 * S1         Pin 12 (P5_2)
 * S2         Pin 13 (P5_0)
 * S3         Pin 14 (P1_7)
 * sensorOut  Pin 15 (P1_6)
 * OE         Pin 39 (P2_6)
 */
ColorSensor colorSensor(11, 12, 13, 14, 15, 39);

Servo Claw;

int pos = 0;

// Variables used to send message
char input;
char Message;
char Color;

void setup() {
  colorSensor.SETUP(); 
  motor1.SETUP();
  motor2.SETUP();
  extend.SETUP();
  Claw.attach(clawPin);
  closeClaw();
  Serial.begin(9600);
  Serial1.begin(115200); 
}

void loop() {
//  if(Serial1.available() > 0){
//    Message = Serial1.read();
//    processMessage(Message);   
//  }
  openClaw();
  closeClaw();
}

void openClaw(){
  for(pos = 0; pos <= 180; pos++){
    Claw.write(pos);
    delay(5);
  }
}

void closeClaw(){
  for(pos = 180; pos >= 0; pos--){
    Claw.write(pos);
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
  motor1.FORWARD(32);
  motor2.FORWARD(32);
  colorSensor.findColor(color);
  motor1.BRAKE();
  motor2.BRAKE();
  openClaw();
  extend.FORWARD(32);
  delay(500);
  extend.BRAKE();
  closeClaw();
  extend.BACKWARD(32);
  delay(500);
  extend.BRAKE();
  sendMessage('G');
}

void givetoDeliver(){
  motor1.FORWARD(32);
  motor2.FORWARD(32);
  colorSensor.findColor('G');   // Change Color 
  motor1.BRAKE();
  motor2.BRAKE();
  extend.FORWARD(32);
  delay(500);
  extend.BRAKE();
  openClaw();
  extend.BACKWARD(32);
  delay(500);
  extend.BRAKE();
  closeClaw();
}

void goHome(){
  motor1.BACKWARD(32);
  motor2.BACKWARD(32);
  colorSensor.findColor('H');   // Change Color 
  motor1.BRAKE();
  motor2.BRAKE();  
  sendMessage('H');
}
