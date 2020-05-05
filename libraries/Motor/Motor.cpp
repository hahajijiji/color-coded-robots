#include "Energia.h"
#include "Motor.h"

Motor::Motor(int fPin, int bPin)
{
  forwardPin = fPin;
  backwardPin = bPin;
}

void Motor::SETUP()
{
   pinMode(forwardPin, OUTPUT);   
   pinMode(backwardPin, OUTPUT);
}

void Motor::FORWARD(int speed)
{
   analogWrite(forwardPin, speed);
   digitalWrite(backwardPin, LOW);
}

void Motor::BACKWARD(int speed)
{
   analogWrite(backwardPin, speed);
   digitalWrite(forwardPin, LOW);
}

void Motor::BRAKE()
{
   digitalWrite(forwardPin, HIGH);
   digitalWrite(backwardPin, HIGH);
}