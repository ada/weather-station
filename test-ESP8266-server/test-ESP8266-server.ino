#include <ESP8266WiFi.h>
#include <stdio.h>
#include <string.h>

#define Data_Block_Value_Size_Byte 4


struct Data_Block
{
  char Type;
  union
  {
    char Value [4];
    float Value_Float;
    int Value_Int;
  };
};


WiFiServer server (8266);
WiFiClient Client_Database [1];
IPAddress Client_Database_IP;
uint16_t Client_Database_Port = 333;



void Serial_Put_Data_Block (struct Data_Block * Item)
{
  Serial.print ("\nData_Block:\n");
  Serial.print ("Type ");
  Serial.print (Item->Type);
  Serial.println ();
  Serial.print ("Value ");
  Serial.write (Item->Value, Data_Block_Value_Size_Byte);
  Serial.println ();
  Serial.print ("Value_Float ");
  Serial.print (Item->Value_Float);
  Serial.println ();
  Serial.print ("Value_Int ");
  Serial.print (Item->Value_Int);
  Serial.println ();
}

void Serial_Put_WiFIClient (WiFiClient * Item)
{
  Serial.print ("remoteIP: ");
  Serial.println (Item->remoteIP ());
  Serial.print ("remotePort: ");
  Serial.println (Item->remotePort ());
  Serial.print ("localIP: ");
  Serial.println (Item->localIP ());
  Serial.print ("localPort: ");
  Serial.println (Item->localPort ());
}

void WiFi_Connect ()
{
  const char* ssid = "Tenda_27EE90";
  const char* password = "glassspoon";
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




void Serial_Read_Data_Block (struct Data_Block * packet)
{
  Serial.print ("\nRead type begin\n");
  Serial.readBytes ((char *) &(packet->Type), sizeof (char));
  Serial.print ("Read type end\n");
  Serial.print ("Read value begin\n");
  Serial.readBytes ((char *) &(packet->Value), Data_Block_Value_Size_Byte);
  Serial.print ("Read value end\n");
}


int WiFiClient_Read_Buffer (WiFiClient * Item, char * Buffer, int Size, char Terminator)
{
  int I = 0;
  while (true)
  {
    if (I < Size)
    {
      if (Item->available ())
      {
        Buffer [I] = Item->read ();
        if (Buffer [I] == Terminator)
        {
          break;
        }
        I = I + 1;
      }
      else
      {
        break;
      }
    }
  }
  return I;
}


void Read_Command (WiFiClient * Item)
{
  char Message [30];
  int Size;
  Size = WiFiClient_Read_Buffer (Item, Message, 30, '\0');
  Serial.print ("\nMessage ");
  Serial.print (Size);
  Serial.print ("B ");
  Serial.print (Message);
  Serial.println ();
  if (strcmp (Message, "set_ip") == 0)
  {
    Size = WiFiClient_Read_Buffer (Item, Message, 30, '\0');
    if (Size > 0 && Size < 30)
    {
      WiFi.hostByName (Message, Client_Database_IP);
      Item->print ("Connecting to: ");
      Item->print (Client_Database_IP);
      Item->println ();
      Serial.print ("Connecting to: ");
      Serial.print (Client_Database_IP);
      Serial.println ();
      Client_Database->connect (Message, 333);
    }
  }
}


void Connected_Client (WiFiClient * Item)
{
    Serial.print ("\nClient connected.\n");
    Serial_Put_WiFIClient (Item);
    Item->print ("\nHello client\n");
    delay (3000);
    Read_Command (Item);
    Item->stop ();
    delay (1);
    Serial.print ("Client disonnected\n");
}





void Connected_Database ()
{
  if (Serial.available () > 0)
  {
    struct Data_Block Packet [1] = {0};
    Serial_Read_Data_Block (Packet);
    Serial_Put_Data_Block (Packet);
    Client_Database->write ((const uint8_t *) Packet, sizeof (struct Data_Block));
  }
  else
  {
    Serial.println ("Waiting for UART data.");
    Client_Database->print ("Waiting for UART data.\n");
    delay (1000);
  }
}



void WiFi_Connected ()
{

  if (server.status ())
  {
    if (server.hasClient ())
    {
      WiFiClient C = server.available ();
      Connected_Client (&C);
    }
  }
  else
  {
    Serial.println ("Server Begin");
    server.begin ();
  }
  
  if (Client_Database->connected ())
  {
    Connected_Database ();
  }
  else
  {
    Serial.print ("\nReconnect ");
    Serial.print (Client_Database_IP);
    Serial.print (":");
    Serial.println (Client_Database_Port);
    Client_Database->connect (Client_Database_IP, Client_Database_Port);
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
  }
}




