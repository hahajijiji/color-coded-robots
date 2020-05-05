#include "Energia.h"
#include "ColorSensor.h"

// Store frequency read by the photodiodes
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

// Store RGB color values
int redColor = 0;
int greenColor = 0;
int blueColor = 0;

bool foundColor = false;

int S0, S1, S2, S3, sensorOut, OE;

ColorSensor::ColorSensor()
{

}

void ColorSensor::SETUP(int S0_1, int S1_1, int S2_1, int S3_1, int sensorOut_1, int OE_1)
{
  
  S0 = S0_1;  
  S1 = S1_1;
  S2 = S2_1;
  S3 = S3_1;
  sensorOut = sensorOut_1;
  OE = OE_1;

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

void ColorSensor::findColor(char color)
{
  while(!foundColor){
      // Measure RED value
      digitalWrite(S2, LOW);
      digitalWrite(S3, LOW);
      redFrequency = pulseIn(sensorOut, LOW);
      redColor = map(redFrequency, 8, 48, 255, 0);      // Replace values after calibration (8, 48)
      Serial.print("R = ");
      Serial.print(redColor);

      // Measure BLUE value
      digitalWrite(S2, LOW);
      digitalWrite(S3, HIGH); 
      blueFrequency = pulseIn(sensorOut, LOW);
      blueColor = map(blueFrequency, 11, 44, 255, 0);   // Replace values after calibration (11, 44)
      Serial.print(" B = ");
      Serial.print(blueColor);

      // Measure GREEN value
      digitalWrite(S2, HIGH);
      digitalWrite(S3, HIGH);
      greenFrequency = pulseIn(sensorOut, LOW);
      greenColor = map(greenFrequency, 11, 80, 255, 0); // Replace values after calibration (11, 80)
      Serial.print(" G = ");
      Serial.println(greenColor);

      // Check if color is found (RED, GREEN, BLUE, YELLOW, WHITE)
      switch (color){
        case 'R':
          if(redColor > greenColor && redColor > blueColor){
            if(redColor > 0 && greenColor > 0 && blueColor > 0){
                foundColor = true;
		Serial.println("RED Detected!");
            }
          }
          break;
        case 'G':
          if(greenColor > redColor && greenColor > blueColor){
            if(redColor > 0 && greenColor > 0 && blueColor > 0){
              foundColor = true;
	      Serial.println("GREEN Detected!");
            }
          }
          break;
        case 'B':
          if(blueColor > redColor && blueColor > greenColor){
            if(redColor > 0 && greenColor > 0 && blueColor > 0){
                foundColor = true;
		Serial.println("BLUE Detected!");
            }
          }
          break;
      }
  }
  
  foundColor = false;
}