#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <stdio.h>
#include <string.h>

#define Data_Block_Value_Size_Byte 4
struct Data_Block
{
  uint8_t Signature = 'q';
  uint8_t Type = 0;
  uint8_t A = 0;
  uint8_t B = 0;
  union
  {
    uint8_t Value [4] = {0};
    float Value_Float;
    int32_t Value_Int;
  };
};

#define Control_Server_Message_Length 30
struct Control_Server
{
  WiFiServer WiFi_Server = WiFiServer (8266);
  WiFiClient WiFi_Client [1];
  char Message [Control_Server_Message_Length + 1] = {'\0'};
};

struct Database_Connection
{
  WiFiClient WiFi_Client [1];
  IPAddress Address;
  uint16_t Port;
  struct Data_Block UART_Block [1];
  struct Data_Block Info_Block [1];
};


ESP8266WebServer Server_Web (8267);
struct Database_Connection My_Database_Connection [1];
struct Control_Server My_Control_Server [1];



void WiFi_Connect ()
{
  const char* ssid = "home";
  const char* password = "mynameis";
  WiFi.begin (ssid, password);
  Serial.print ("\nConnecting to ");
  Serial.print (ssid);
  Serial.println ();
  while (WiFi.status () != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print ("\nWiFi connected\n");
  Serial.print ("IP address: ");
  Serial.print (WiFi.localIP());
  Serial.println ();
  Serial.print ("subnetMask: ");
  Serial.print (WiFi.subnetMask());
  Serial.println ();
  Serial.print ("gatewayIP: ");
  Serial.print (WiFi.gatewayIP());
  Serial.println ();
  /*
  if (MDNS.begin ("esp8266"))
  {
    Serial.println ("MDNS responder started");
  }
  */
}


int WiFiClient_Read_Buffer (WiFiClient * Item, char * Buffer, int Size, char Terminator)
{
  int I = 0;
  while (true)
  {
    if (I >= Size)
    {
      break;
    }
    while (!Item->available ())
    {}
    Buffer [I] = Item->read ();
    if (Buffer [I] == Terminator)
    {
      break;
    }
    I = I + 1;
  }
  return I;
}

void Database_Connection_Send_Data_Block (struct Database_Connection * Item)
{
  static int Frame = 0;

  Item->WiFi_Client->flush ();
  
  if (Serial.available () == sizeof (struct Data_Block))
  {
    Serial.readBytes ((char *) (Item->UART_Block), sizeof (struct Data_Block));
    while (Serial.available() > 0)
    {
      char t = Serial.read();
    }
  }
  else if (Serial.available () > sizeof (struct Data_Block))
  {
    while (Serial.available() > 0)
    {
      char t = Serial.read();
    }
  }
  
  switch (Frame)
  {
    case 1:
      Item->WiFi_Client->write ((const uint8_t *) Item->UART_Block, sizeof (struct Data_Block));
    break;
    case 2:
      Item->Info_Block->Type = 'x';
      Item->Info_Block->Value_Int = WiFi.RSSI();
      Item->WiFi_Client->write ((const uint8_t *) Item->Info_Block, sizeof (struct Data_Block));
    break;
    case 3:
      Item->Info_Block->Type = 1;
      Item->Info_Block->Value_Int = 123;
      Item->WiFi_Client->write ((const uint8_t *) Item->Info_Block, sizeof (struct Data_Block));
    break;
    case 100000:
      Frame = 0;
    break;
  }
  Frame = Frame + 1;
}


void Control_Server_Read_Command (Control_Server * Item)
{
  WiFiClient_Read_Buffer (Item->WiFi_Client, Item->Message, Control_Server_Message_Length, '\0');
  if (strcmp (Item->Message, "set_ip") == 0)
  {
    WiFiClient_Read_Buffer (Item->WiFi_Client, Item->Message, Control_Server_Message_Length, '\0');
    WiFi.hostByName (Item->Message, My_Database_Connection->Address);
    My_Database_Connection->WiFi_Client->stop ();
  }
  else if (strcmp (Item->Message, "set_port") == 0)
  {
    WiFiClient_Read_Buffer (Item->WiFi_Client, Item->Message, Control_Server_Message_Length, '\0');
    My_Database_Connection->Port = String (Item->Message).toInt ();
    My_Database_Connection->WiFi_Client->stop ();
  }
  else
  {
    
  }
}


void WiFi_Connected ()
{
  Server_Web.handleClient();

  if (My_Control_Server->WiFi_Server.status ())
  {
    if (My_Control_Server->WiFi_Server.hasClient ())
    {
      My_Control_Server->WiFi_Client [0] = My_Control_Server->WiFi_Server.available ();
      My_Control_Server->WiFi_Client->print ("\nHello client says server\n");
      Control_Server_Read_Command (My_Control_Server);
      My_Control_Server->WiFi_Client->stop ();
    }
  }
  else
  {
    Serial.println ("Server_Control Begin");
    My_Control_Server->WiFi_Server.begin ();
  }
  
  if (My_Database_Connection->WiFi_Client->connected ())
  {
    Database_Connection_Send_Data_Block (My_Database_Connection);
  }
  else
  {
    Serial.print ("\nReconnect ");
    Serial.print (My_Database_Connection->Address);
    Serial.print (":");
    Serial.println (My_Database_Connection->Port);
    My_Database_Connection->WiFi_Client->connect (My_Database_Connection->Address, My_Database_Connection->Port);
  }
  
}


void setup ()
{
  Serial.setTimeout (20000);
  Serial.begin (115200);
  delay (10);
}


void loop ()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    WiFi_Connected ();
  }
  else
  {
    Serial.println ("Connect_WiFi");
    WiFi_Connect ();
    delay (10);
    
    Server_Web.on("/", []()
    {
      String html;
      html += "My_Database_Connection->WiFi_Client->status: ";
      html += My_Database_Connection->WiFi_Client->status () ? "Online" : "Offline";
      html += "\n";
      
      html += "My_Database_Connection->Address: ";
      html += My_Database_Connection->Address.toString ();
      html += "\n";
      
      html += "My_Database_Connection->Port: ";
      html += My_Database_Connection->Port;
      html += "\n";
      
      html += "Last Server_Control_Message: ";
      html += My_Control_Server->Message;
      html += "\n";
      
      html += "RSSI: ";
      html += (int) WiFi.RSSI ();
      html += "\n";
      
      Server_Web.send (200, "text/plain", html);
    });
    
    Serial.println ("Server_Web Begin");
    Server_Web.begin ();
  }
}

