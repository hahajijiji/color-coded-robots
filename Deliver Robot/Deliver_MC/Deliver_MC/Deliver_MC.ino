#include <Motor.h>
#include <ColorSensor.h>

/* 
 *  Create Motor Objects
 *  Motor 1 Forward       Pin 34 (P2_3)
 *  Motor 1 Backward      Pin 35 (P6_7)
 *  Motor 2 Forward       Pin 31 (P3_7)
 *  Motor 2 Backward      Pin 32 (P3_5)
 */
Motor motor1(34, 35);
Motor motor2(31, 32);

/*
 * Create Color Sensor Objects
 * S0_1         Pin 11 (P3_6)
 * S1_1         Pin 12 (P5_2)
 * S2_1         Pin 13 (P5_0)
 * S3_1         Pin 14 (P1_7)
 * sensorOut_1  Pin 15 (P1_6)
 * OE_1         Pin 39 (P2_6)
 * 
 * S0_2         Pin 5  (P4_1)
 * S1_2         Pin 6  (P4_3)
 * S2_2         Pin 7  (P1_5)
 * S3_2         Pin 8  (P4_6)
 * sensorOut_2  Pin 9  (P6_5)
 * OE_2         Pin 10 (P6_4)
 */
ColorSensor colorSensor1(11, 12, 13, 14, 15, 39);
ColorSensor colorSensor2(5, 6, 7, 8, 9, 10);

// Variables used to send message
char input;
char Message;
char Color;

void setup() {
  motor1.SETUP();
  motor2.SETUP();
  colorSensor1.SETUP();
  colorSensor2.SETUP();
  Serial.begin(9600);
  Serial1.begin(115200);
}

void loop() {
//  if(Serial1.available() > 0){
//    Message = Serial1.read();
//    processMessage(Message);   
//  }
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
  }
  else if(Data == 'r' || Data == 'g' || Data == 'b'){
    Data = Data - 0x20;
    if(Data == Color){
      sendMessage('M');
    }
    else{
    }
  }
  else if(Data == 'x'){
    input = Serial1.read();
  }
}
