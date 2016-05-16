//http://aprs.gids.nl/nmea/#vtg

#include "TinyGPS++.h"
#define DEFAULT_TIMEOUT 1000

TinyGPSPlus gps;
char c;

void setup()
{
  Serial.begin(9600); //PC
  Serial3.begin(9600);
  Serial3.setTimeout(DEFAULT_TIMEOUT);
  
  while(!Serial || !Serial3);

  Serial.print("GY-NEO6MV2 Test. Library version: ");
  Serial.println(TinyGPSPlus::libraryVersion());
  delay(1000);
}

void loop()
{
  
  while (Serial3.available() > 0)
  {
    c = Serial3.read(); 
    //Serial.write(c);
    
    if (gps.encode(c))
    {
      Serial.println();
      displayInfo();
      delay(3000);
    }
    
    if (millis() > 5000 && gps.charsProcessed() < 10)
    {
      Serial.println(F("No GPS detected"));
      while(true);
    }
    
  }

  //Serial.println("---");
  
  
}

void displayInfo()
{
  Serial.print("Location: "); 
  if (1 && gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(",");
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print("INVALID");
  }

  Serial.print(" Date/Time: ");
  if (1 && gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print("INVALID");
  }

  Serial.print(" ");
  if (1 && gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print("0");
    Serial.print(gps.time.hour());
    Serial.print(":");
    if (gps.time.minute() < 10) Serial.print("0");
    Serial.print(gps.time.minute());
    Serial.print(":");
    if (gps.time.second() < 10) Serial.print("0");
    Serial.print(gps.time.second());
    Serial.print(".");
    if (gps.time.centisecond() < 10) Serial.print("0");
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print("INVALID");
  }
  
  Serial.println();
  Serial.print("Sattelites in use: ");
  Serial.println(gps.satellites.value());
  
}

