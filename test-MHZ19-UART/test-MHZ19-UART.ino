void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200);
 Serial1.begin(9600); 
 while(!Serial || !Serial1);
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

  delay(500);
}

int readCO2()
{

  uint8_t cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
  // command to ask for data
  uint8_t response[9] ={0}; // for answer

  //while (Serial1.available ()) {char C = Serial1.read ();}
  //Serial1.flush ();
  Serial1.write (cmd, 9); //request PPM CO2
  Serial1.readBytes(response, 9);

  for(int i=0; i<9; i++)
  {
    Serial.print(response[i],HEX);
    Serial.print(" ");
  }
  Serial.println();

  if (response[0] == 0xFF && response[1] == 0x86)
  {
    //Gas concentration= high level * 256 + low level
    return ((int)response[2] * 256) + (int)response[3];
  }
  
  return -1;
}
