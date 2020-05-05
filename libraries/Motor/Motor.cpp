#include "Energia.h"
#include "Motor.h"

Motor::Motor()
{

}

void Motor::SETUP()
{
   pinMode(34, OUTPUT);   //P2_3 Motor 1 Forward Pin
   pinMode(35, OUTPUT);   //P6_7 Motor 1 Backward Pin
   pinMode(31, OUTPUT);   //P3_7 Motor 2 Forward Pin
   pinMode(32, OUTPUT);   //P3_5 Motor 2 Backward Pin
}

void Motor::FORWARD(int speed)
{
   analogWrite(34, speed);
   digitalWrite(35, LOW);
   analogWrite(31, speed);
   digitalWrite(32, LOW);
}

void Motor::BACKWARD(int speed)
{
   analogWrite(35, speed);
   digitalWrite(34, LOW);
   analogWrite(32, speed);
   digitalWrite(31, LOW);
}

void Motor::BRAKE()
{
   digitalWrite(34, HIGH);
   digitalWrite(35, HIGH);
   digitalWrite(31, HIGH);
   digitalWrite(32, HIGH);
}