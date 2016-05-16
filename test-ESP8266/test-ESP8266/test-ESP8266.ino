#define DEFAULT_TIMEOUT 1000
#define DEBUG 1
String cmd;

void setup()
{
  //USB
  Serial.begin(115200);
  Serial2.begin(9600); 
  Serial2.setTimeout(DEFAULT_TIMEOUT);
  
  while(!Serial || !Serial2);
  
  sendCommand("AT+RST", DEFAULT_TIMEOUT, DEBUG); // Reset
  sendCommand("AT+CWMODE=1", DEFAULT_TIMEOUT, DEBUG); // Station mode
  sendCommand("AT+CIPMUX=0", DEFAULT_TIMEOUT, DEBUG); // Single connection
  sendCommand("AT+CWJAP=\"home\",\"mynameis\"", DEFAULT_TIMEOUT, DEBUG); // Join AP
  sendCommand("AT+CIFSR", DEFAULT_TIMEOUT, DEBUG); // Get IP and MAC address
  sendCommand("AT+CIPSTART=\"TCP\",\"192.168.0.15\",7777", DEFAULT_TIMEOUT, DEBUG); //Create TCP 
  sendCommand("AT+CIPSTATUS", DEFAULT_TIMEOUT, DEBUG); //Status
  delay(3000);
  //cmd = "GET /api/weather/post HTTP/1.1\r\n";
  //sendCommand("AT+CIPSEND="+cmd.length()+2, DEFAULT_TIMEOUT, DEBUG);
  //sendCommand(cmd, DEFAULT_TIMEOUT, DEBUG);
}

void loop()
{
  
  if(Serial2.available())
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

