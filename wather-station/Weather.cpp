#include "Arduino.h"
#include "Weather.h"


void Weather::SerialPrint()
{
  Serial.print("Humidity:\t");
  Serial.println(humidity);
  Serial.print("Temperature:\t");
  Serial.println(temperature);
  Serial.println();
}
