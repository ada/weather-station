#ifndef Weather_h
#define Weather_h

#include "Arduino.h"

class Weather
{
  public: 
    double temperature; 
    double humidity;  
    void SerialPrint();
};

#endif
