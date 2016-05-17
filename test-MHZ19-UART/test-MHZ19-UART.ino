void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200);
 Serial2.begin(9600); 
 while(!Serial || !Serial2);
}

void loop() {
  Serial.println(readCO2());
  /*if(Serial2.available())
  {
    while(Serial2.available())
    {
      Serial.write(Serial2.read());
    }  
  }

  if(Serial.available())
  {
    delay(50);
    cmd = "";
    while(Serial.available()){
      cmd += (char)Serial.read();
    }
      Serial2.println(command);
    Serial2.flush();
  }*/
  
}

int readCO2()
{

  byte cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
  // command to ask for data
  char response[9] ={0}; // for answer

  Serial2.write(cmd, 9); //request PPM CO2
  Serial2.readBytes(response, 9);

  for(int i=0; i<9; i++)
  {
    Serial.print(response[i],HEX);
    Serial.print(" ");
  }
  Serial.println();
  /*if (response[0] != 0xFF)
  {
    Serial.println("Wrong starting byte from co2 sensor!");
    return -1;
  }

  if (response[1] != 0x86)
  {
    Serial.println("Wrong command from co2 sensor!");
    return -1;
  }*/

  int responseHigh = (int) response[2];
  int responseLow = (int) response[3];
  int ppm = (256 * responseHigh) + responseLow;
  return ppm;
}
