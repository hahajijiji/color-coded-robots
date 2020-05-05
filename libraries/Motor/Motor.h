#ifndef Motor_h
#define Motor_h

#include "Energia.h"

class Motor
{
   public:
     Motor(int fPin, int bPin);
     void SETUP();
     void FORWARD(int speed);
     void BACKWARD(int speed);
     void BRAKE();
   private:
     int forwardPin;
     int backwardPin;
};

#endif