#ifndef ColorSensor_h
#define ColorSensor_h

#include "Energia.h"

class ColorSensor
{
   public:
     ColorSensor(int _S0, int _S1, int _S2, int _S3, int _sensorOut, int _OE);
     void SETUP();
     void findColor(char color);
   private:
     int S0;
     int S1;
     int S2;
     int S3;
     int sensorOut;
     int OE;
};

#endif