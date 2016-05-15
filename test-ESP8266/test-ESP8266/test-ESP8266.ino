#define DEFAULT_TIMEOUT 1000
#define DEBUG 1
String cmd;

void setup()
{
  //USB
  Serial.begin(115200);
  Serial2.begin(115200); //115200, 
  Serial2.setTimeout(DEFAULT_TIMEOUT);
  
  while(!Serial || !Serial2); 

  //Access point
  /*
  sendCommand("AT+RST", DEFAULT_TIMEOUT, DEBUG); // rst
  sendCommand("AT+CWMODE=3", DEFAULT_TIMEOUT, DEBUG); //  access point
  sendCommand("AT+CIFSR", DEFAULT_TIMEOUT, DEBUG); // get ip address
  sendCommand("AT+CIPMUX=1", DEFAULT_TIMEOUT, DEBUG); // configure for multiple connections
  sendCommand("AT+CIPSERVER=1,80", DEFAULT_TIMEOUT, DEBUG); // turn on server on port 80
  */

  //Station
  sendCommand("AT+RST", DEFAULT_TIMEOUT, DEBUG); // rst
  sendCommand("AT+CWMODE=1", DEFAULT_TIMEOUT, DEBUG); // Station
  //sendCommand("AT+CWLAP", DEFAULT_TIMEOUT, DEBUG); // List access points
  sendCommand("AT+CWJAP=\"alex\",\"mynameis\"", DEFAULT_TIMEOUT, DEBUG); // Station
  sendCommand("AT+CIPMODE=0", DEFAULT_TIMEOUT, DEBUG); // Not data mode
  sendCommand("AT+CIPMUX=1", DEFAULT_TIMEOUT, DEBUG); 
  sendCommand("AT+CIPSTATUS", DEFAULT_TIMEOUT, DEBUG); // Not data mode
  //AT+CIPMUX=1
  //AT+CIPSTART=0,"TCP","192.168.0.15",7777
  //AT+CIPSTART=0,"TCP","superuser.com",80
}

void loop()
{
  
  if(Serial2.available())
  {
    while(Serial2.available())
    {
      Serial.write(Serial2.read());
    }  

    /*int connectionId = Serial2.read()-48;
     String webpage = "<head><meta http-equiv=""refresh"" content=""10""></head>";
     webpage+= millis();
     String cipSend = "AT+CIPSEND=";
     cipSend += connectionId;
     cipSend += ",";
     cipSend +=webpage.length();
     
     sendCommand(cipSend, DEFAULT_TIMEOUT, DEBUG);
     sendCommand(webpage, DEFAULT_TIMEOUT, DEBUG);
     
  
     String closeCommand = "AT+CIPCLOSE="; 
     closeCommand+=connectionId;
     sendCommand(closeCommand, DEFAULT_TIMEOUT*3, DEBUG);*/
  }
  
  if(Serial.available())
  {
    //sendData("11","12");
    delay(50);
    cmd = "";
    while(Serial.available()){
      cmd += (char)Serial.read();
    }
      
    sendCommand(cmd, DEFAULT_TIMEOUT, DEBUG);
  }
}

String sendCommand(String command, const int timeout, boolean debug)
{
    String response = "";
    Serial2.println(command);
    Serial2.flush();
    long int time = millis();
    
    while( (time+timeout) > millis())
    {
      while(Serial2.available())
      {
        char c = Serial2.read(); 
        response += c;
      }  
    }
    
    if(debug)
    {
      Serial.print(response);
      Serial.println("---");
    }
    delay(2000);
    return response;
}


void sendData(String temperature ,String humidity){
  // Set up TCP connection. 
  cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "192.168.0.15";
  cmd += "\",7777";
  sendCommand(cmd, DEFAULT_TIMEOUT, DEBUG);
  sendCommand("AT+CIPSTATUS", DEFAULT_TIMEOUT, DEBUG);


  // Send data.
  cmd = "GET /api/weather/post?device=";
  cmd += "ws1";
  cmd += "&temperature=";
  cmd += "22";
  cmd += "&humidity=";
  cmd += "23";
  sendCommand("AT+CIPSEND="+cmd.length(), DEFAULT_TIMEOUT, DEBUG);
  sendCommand(cmd, DEFAULT_TIMEOUT, DEBUG);
}
