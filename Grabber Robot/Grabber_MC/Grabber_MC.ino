// Motor 1 Pins  
#define Motor1_Forward 5    //P4_1
#define Motor1_Backward 6   //P4_3

// Motor 2 Pins
#define Motor2_Forward 7    //P1_5
#define Motor2_Backward 8   //P4_6

// Claw Pins  
#define Claw_Open 9         //P6_5
#define Claw_Close 10       //P6_4

// TCS3200 Pins
#define S0 11               //P3_6
#define S1 12               //P5_2
#define S2 13               //P5_0
#define S3 14               //P1_7
#define sensorOut 15        //P1_6

// Variables used to send message
char input;
char Message;

// Store frequency read by the photodiodes
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

// Store RGB color values
int redColor = 0;
int greenColor = 0;
int blueColor = 0;

void config_ColorSensor(){
  // Setting the outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  // Setting the sensorOut as an input
  pinMode(sensorOut, INPUT);
  
  // Setting frequency scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
}

void config_Motors(){
  // Setting the outputs
  pinMode(Motor1_Forward, OUTPUT);
  pinMode(Motor1_Backward, OUTPUT);
  pinMode(Motor2_Forward, OUTPUT);
  pinMode(Motor2_Backward, OUTPUT);
  pinMode(Claw_Open, OUTPUT);
  pinMode(Claw_Close, OUTPUT);
}

bool findColor(char color){
  bool foundColor = false;
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

      // Check if color is found (RED, GREEN, BLUE, ...)
      switch (color){
        case 'R':
          if(redColor > greenColor && redColor > blueColor){
            foundColor = true;
          }
          break;
        case 'G':
          if(greenColor > redColor && greenColor > blueColor){
            foundColor = true;
          }
          break;
        case 'B':
          if(blueColor > redColor && blueColor > greenColor){
            foundColor = true;
          }
          break;
      }
  }
  return foundColor;
}

void moveForward(){
  // Send PWM signal to Motor1_Forward pin (P4_1)
  analogWrite(Motor1_Forward, 255); //0-255
  // Set Motor1_Backward pin LOW (P4_3)
  digitalWrite(Motor1_Backward, LOW);
  
  // Send PWM signal to Motor2_Forward pin (P1_5)
  analogWrite(Motor2_Forward, 255); //0-255
  // Set Motor2_Backward pin LOW (P4_6)
  digitalWrite(Motor2_Backward, LOW);
}

void moveBackward(){
  // Send PWM signal to Motor1_Backward pin (P4_3)
  analogWrite(Motor1_Backward, 255); //0-255
  // Set Motor1_Forward pin LOW (P4_1)
  digitalWrite(Motor1_Forward, LOW);
  
  // Send PWM signal to Motor2_Backward pin (P4_6)
  analogWrite(Motor2_Backward, 255); //0-255
  // Set Motor2_Forward pin LOW (P1_5)
  digitalWrite(Motor2_Forward, LOW);
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
  // Set Claw_Open pin HIGH (P6_5)
  digitalWrite(Claw_Open, HIGH);
  // Set Claw_Closed pin LOW (P6_4)
  digitalWrite(Claw_Close, LOW);
}

void closeClaw(){
  // Set Claw_Open pin LOW (P6_5)
  digitalWrite(Claw_Open, LOW);
  // Set Claw_Closed pin HIGH (P6_4)
  digitalWrite(Claw_Close, HIGH);
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
  
}

void setup() {
  config_ColorSensor(); 
  config_Motors();
  Serial.begin(9600);
  Serial1.begin(115200);
  Message = 'R';
}

void loop() {
  if(Serial1.available() > 0){
    Message = Serial1.read();
    processMessage(Message);   
  }
  
}
