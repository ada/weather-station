#include "dht11.h"
#define DHT11PIN 30 //DIO 30
dht11 DHT11;

void setup() {
  Serial.begin(9600);
}

void loop() {

  int chk = DHT11.read(DHT11PIN);
  switch (chk)
  {
    case DHTLIB_OK: 
      Serial.println(DHT11.temperature);
      Serial.println(DHT11.humidity);
    break;
    case DHTLIB_ERROR_CHECKSUM: 
      Serial.println("Checksum error"); 
    break;
    case DHTLIB_ERROR_TIMEOUT: 
      Serial.println("Time out error"); 
    break;
    default: 
      Serial.println("Unknown error"); 
    break;
  }
  
  delay(500);
}
