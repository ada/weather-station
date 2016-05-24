#ifndef WEATHER_H

struct Weather
{
    int32_t DHT11_Humidity[2];
    int32_t DHT11_Temperature[2];
    float BMP085_Temperature; 
    float BMP085_Altitude;
    int32_t BMP085_Pressure; 
    int32_t BMP085_Sea_Pressure;
    int32_t BMP085_Pressure_Raw;
    int32_t BMP085_Temperature_Raw;
    int32_t MQ2_Raw[2];
    int32_t MQ4_Raw[2];
    int32_t Bluesky_Rain_Drop[2];
    int32_t Bluesky_Rain_Drop_Delta[2];
    int32_t Neo6mGPS_Time_Centisecond;
    int32_t Neo6mGPS_Time_Second;
    int32_t Neo6mGPS_Time_Minute;
    int32_t Neo6mGPS_Time_Hour;
    int32_t Neo6mGPS_Time_Day;
    int32_t Neo6mGPS_Time_Month;
    int32_t Neo6mGPS_Time_Year;
    double Neo6mGPS_Location_Latidude;
    double Neo6mGPS_Location_Longitude;
    int32_t Neo6mGPS_Info_Satelite_Count;
    double Neo6mGPS_Altitude_Meter;
};

String CSV(Weather w){
  return String(w.DHT11_Humidity[0], DEC) + ',';
}

#endif
