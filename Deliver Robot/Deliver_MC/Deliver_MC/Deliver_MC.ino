// Motor 1 Pins  
#define Motor1_Forward 34    //P2_3
#define Motor1_Backward 35   //P6_7

// Motor 2 Pins
#define Motor2_Forward 31    //P3_7
#define Motor2_Backward 32   //P3_5

// TCS3200 Pins Color Sensor 1
#define S0_1 11               //P3_6
#define S1_1 12               //P5_2
#define S2_1 13               //P5_0
#define S3_1 14               //P1_7
#define sensorOut_1 15        //P1_6
#define OE_1 39               //P2_6

// TCS3200 Pins Color Sensor 2
#define S0_2 5                //P4_1
#define S1_2 6                //P4_3
#define S2_2 7                //P1_5
#define S3_2 8                //P4_6
#define sensorOut_2 9         //P6_5
#define OE_2 10               //P6_4

// Store frequency read by the photodiodes
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

// Store RGB color values
int redColor = 0;
int greenColor = 0;
int blueColor = 0;

bool foundColor = false;

void config_Motors(){
  // Setting the outputs
  pinMode(Motor1_Forward, OUTPUT);
  pinMode(Motor1_Backward, OUTPUT);
  pinMode(Motor2_Forward, OUTPUT);
  pinMode(Motor2_Backward, OUTPUT);
}

void config_ColorSensor(){
  // Setting the outputs
  pinMode(S0_1, OUTPUT);
  pinMode(S1_1, OUTPUT);
  pinMode(S2_1, OUTPUT);
  pinMode(S3_1, OUTPUT);
  pinMode(OE_1, OUTPUT);
  
  // Setting the sensorOut as an input
  pinMode(sensorOut_1, INPUT);
  
  // Setting frequency scaling to 20%
  digitalWrite(S0_1,HIGH);
  digitalWrite(S1_1,LOW);
  digitalWrite(OE_1,LOW);

  // Setting the outputs
  pinMode(S0_2, OUTPUT);
  pinMode(S1_2, OUTPUT);
  pinMode(S2_2, OUTPUT);
  pinMode(S3_2, OUTPUT);
  pinMode(OE_2, OUTPUT);
  
  // Setting the sensorOut as an input
  pinMode(sensorOut_2, INPUT);
  
  // Setting frequency scaling to 20%
  digitalWrite(S0_2,HIGH);
  digitalWrite(S1_2,LOW);
  digitalWrite(OE_2,LOW);
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

void findColor(char color){
  while(!foundColor){
      // Measure RED value
      digitalWrite(S2_1, LOW);
      digitalWrite(S3_1, LOW);
      redFrequency = pulseIn(sensorOut_1, LOW);
      redColor = map(redFrequency, 8, 48, 255, 0);      // Replace values after calibration (8, 48)

      // Measure BLUE value
      digitalWrite(S2_1, LOW);
      digitalWrite(S3_1, HIGH); 
      blueFrequency = pulseIn(sensorOut_1, LOW);
      blueColor = map(blueFrequency, 11, 44, 255, 0);   // Replace values after calibration (11, 44)

      // Measure GREEN value
      digitalWrite(S2_1, HIGH);
      digitalWrite(S3_1, HIGH);
      greenFrequency = pulseIn(sensorOut_1, LOW);
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

void setup() {
  config_Motors();
  config_ColorSensor();
  Serial.begin(9600);
  Serial1.begin(115200);
}

void loop() {
//  if(Serial1.available() > 0){
//    Message = Serial1.read();
//    processMessage(Message);   
//  }
}
