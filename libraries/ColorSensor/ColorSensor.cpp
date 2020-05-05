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

ColorSensor::ColorSensor(int _S0, int _S1, int _S2, int _S3, int _sensorOut, int _OE)
{
  S0 = _S0;
  S1 = _S1;
  S2 = _S2;
  S3 = _S3;
  sensorOut = _sensorOut;
  OE = _OE;
}

void ColorSensor::SETUP()
{
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