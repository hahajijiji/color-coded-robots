#ifndef ColorSensor_h
#define ColorSensor_h

#include "Energia.h"

class ColorSensor
{
   public:
     ColorSensor();
     void SETUP(int S0, int S1, int S2, int S3, int sensorOut, int OE);
     void findColor(char color);
   private:
};

#endif