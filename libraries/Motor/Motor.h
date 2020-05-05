#ifndef Motor_h
#define Motor_h

#include "Energia.h"

class Motor
{
   public:
     Motor();
     void SETUP();
     void FORWARD(int speed);
     void BACKWARD(int speed);
     void BRAKE();
   private:
};

#endif