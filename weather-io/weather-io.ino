#include <Wire.h>
#include <Adafruit_BMP085.h>
#include "weather.h"
#include "dht11.h"
#include "TinyGPS++.h"
#define DEFAULT_TIMEOUT 1000
TinyGPSPlus gps;


#define DEBUG 1
#define PIN_DHT11_0 30
#define PIN_DHT11_1 31
#define PIN_MQ2_0 A0
#define PIN_MQ2_1 A1
#define PIN_MQ4_0 A2
#define PIN_MQ4_1 A3
#define PIN_Bluesky_Rain_Drop_0 A4
#define PIN_Bluesky_Rain_Drop_1 A5

Weather w; 
dht11 DHT11;
Adafruit_BMP085 BMP180;

void setup() {
  Serial.begin (115200); //PC
  Serial3.begin(9600); //GPS
  Serial.setTimeout (DEFAULT_TIMEOUT);
  Serial3.setTimeout (DEFAULT_TIMEOUT);
  while(!Serial || !Serial3);
  if (!BMP180.begin()) 
  {
    Serial.println("BMP180: Error communicating to device.");
    while(1);
  }
  delay(1000);
}

void loop() {
  // DHT11_1
  switch (DHT11.read(PIN_DHT11_0))
  {
    case DHTLIB_OK:
      w.DHT11_Temperature[0] = DHT11.temperature; 
      w.DHT11_Humidity[0] = DHT11.humidity;
    break;
    case DHTLIB_ERROR_CHECKSUM:
      Serial.println("DHT11_1 Checksum error");
    break;
    case DHTLIB_ERROR_TIMEOUT:
      Serial.println("DHT11_1 Time out error");
    break;
    default:
      Serial.println("DHT11_1 Unknown error");
    break;
  }

  // DHT11_2
  switch (DHT11.read(PIN_DHT11_1))
  {
    case DHTLIB_OK:
      w.DHT11_Temperature[1] = DHT11.temperature; 
      w.DHT11_Humidity[1] = DHT11.humidity;
    break;
    case DHTLIB_ERROR_CHECKSUM:
      Serial.println("DHT11_2 Checksum error");
    break;
    case DHTLIB_ERROR_TIMEOUT:
      Serial.println("DHT11_2 Time out error");
    break;
    default:
      Serial.println("DHT11_2 Unknown error");
    break;
  }

  // BMP085
  w.BMP085_Pressure = BMP180.readPressure(); 
  w.BMP085_Temperature = BMP180.readTemperature();
  w.BMP085_Altitude = BMP180.readAltitude(102300); 
  w.BMP085_Pressure_Raw = BMP180.readRawPressure();
  w.BMP085_Temperature_Raw = BMP180.readRawTemperature();

  // MQ2
  w.MQ2_Raw[0] = analogRead(PIN_MQ2_0);
  w.MQ2_Raw[1] = analogRead(PIN_MQ2_1);
  w.MQ4_Raw[0] = analogRead(PIN_MQ4_0);
  w.MQ4_Raw[1] = analogRead(PIN_MQ4_1);

  //Bluesky
  w.Bluesky_Rain_Drop_Delta [0] = w.Bluesky_Rain_Drop [0] - analogRead(PIN_Bluesky_Rain_Drop_0);
  w.Bluesky_Rain_Drop_Delta [1] = w.Bluesky_Rain_Drop [1] - analogRead(PIN_Bluesky_Rain_Drop_1);
  w.Bluesky_Rain_Drop [0] = analogRead(PIN_Bluesky_Rain_Drop_0);
  w.Bluesky_Rain_Drop [1] = analogRead(PIN_Bluesky_Rain_Drop_1);



  //Neo
  while (Serial3.available() > 0)
  {
    uint8_t c = Serial3.read(); 
    if (gps.encode (c))
    {
      if (1 && gps.location.isValid())
      {
        w.Neo6mGPS_Location_Latidude = gps.location.lat();
        w.Neo6mGPS_Location_Longitude = gps.location.lng();
      }
      if (1 && gps.date.isValid())
      {
        w.Neo6mGPS_Time_Centisecond = gps.time.centisecond();
        w.Neo6mGPS_Time_Second = gps.time.second();
        w.Neo6mGPS_Time_Minute = gps.time.minute();
        w.Neo6mGPS_Time_Hour = gps.time.hour();
      }
      if (1 && gps.date.isValid())
      {
        w.Neo6mGPS_Time_Day = gps.date.day();
        w.Neo6mGPS_Time_Month = gps.date.month();
        w.Neo6mGPS_Time_Year = gps.date.year();
      }
      if (1 && gps.altitude.isValid())
      {
        w.Neo6mGPS_Altitude_Meter = gps.altitude.meters();
      }
      w.Neo6mGPS_Info_Satelite_Count = gps.satellites.value();
    }
  }





  
  #ifdef DEBUG
  Serial.print("DHT11_0_Temperature: ");    Serial.println(w.DHT11_Temperature[0]);
  Serial.print("DHT11_0_Humidity: ");       Serial.println(w.DHT11_Humidity[0]);
  Serial.print("DHT11_1_Temperature: ");    Serial.println(w.DHT11_Temperature[1]);
  Serial.print("DHT11_1_Humidity: ");       Serial.println(w.DHT11_Humidity[1]);
  Serial.print("BMP085_Pressure: ");        Serial.println(w.BMP085_Pressure);
  Serial.print("BMP085_Temperature: ");     Serial.println(w.BMP085_Temperature);
  Serial.print("BMP085_Altitude: ");        Serial.println(w.BMP085_Altitude);
  Serial.print("BMP085_Pressure_Raw: ");    Serial.println(w.BMP085_Pressure_Raw);
  Serial.print("BMP085_Temperature_Raw: "); Serial.println(w.BMP085_Temperature_Raw);
  Serial.print("MQ2_0_Raw: ");              Serial.println(w.MQ2_Raw[0]);
  Serial.print("MQ2_1_Raw: ");              Serial.println(w.MQ2_Raw[1]);
  Serial.print("MQ4_0_Raw: ");              Serial.println(w.MQ4_Raw[0]);
  Serial.print("MQ4_1_Raw: ");                   Serial.println(w.MQ4_Raw[1]);
  Serial.print("Bluesky_Rain_Drop_0: ");         Serial.println(w.Bluesky_Rain_Drop[0]);
  Serial.print("Bluesky_Rain_Drop_1: ");         Serial.println(w.Bluesky_Rain_Drop[1]);
  Serial.print("Bluesky_Rain_Drop_0_Delta: ");   Serial.println(w.Bluesky_Rain_Drop_Delta[0]);
  Serial.print("Bluesky_Rain_Drop_1_Delta: ");   Serial.println(w.Bluesky_Rain_Drop_Delta[1]);
  Serial.print("Neo6mGPS_Info_Satelite_Count: ");Serial.println (w.Neo6mGPS_Info_Satelite_Count);
  Serial.print("Neo6mGPS_Time_Centisecond: ");   Serial.println (w.Neo6mGPS_Time_Centisecond);
  Serial.print("Neo6mGPS_Time_Second: ");        Serial.println (w.0
  );
  Serial.print("Neo6mGPS_Time_Minute: ");        Serial.println (w.Neo6mGPS_Time_Minute);
  Serial.print("Neo6mGPS_Time_Hour: ");          Serial.println (w.Neo6mGPS_Time_Hour);
  Serial.print("Neo6mGPS_Time_Day: ");           Serial.println (w.Neo6mGPS_Time_Day);
  Serial.print("Neo6mGPS_Time_Month: ");         Serial.println (w.Neo6mGPS_Time_Month);
  Serial.print("Neo6mGPS_Time_Year: ");          Serial.println (w.Neo6mGPS_Time_Year);
  Serial.print("Neo6mGPS_Location_Latidude: ");  Serial.println (w.Neo6mGPS_Location_Latidude);
  Serial.print("Neo6mGPS_Location_Longitude: "); Serial.println (w.Neo6mGPS_Location_Longitude);
  Serial.print("Neo6mGPS_Altitude_Meter: ");     Serial.println (w.Neo6mGPS_Altitude_Meter);
  Serial.println("---");
  #endif
  
  delay(1000);
}
