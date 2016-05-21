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
  Serial.println ("Data_Block");
  Serial.print ("Type ");
  Serial.println (Item->Type);
  Serial.print ("Value ");
  Serial.write (Item->Value, Data_Block_Value_Size_Byte);
  Serial.println ();
  Serial.print ("Value_Float ");
  Serial.println (Item->Value_Float);
  Serial.print ("Value_Int ");
  Serial.println (Item->Value_Int);
}

void Put_Client (WiFiClient * Item)
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

void Connect_WiFi ()
{
  const char* ssid = "Tenda_27EE90";
  const char* password = "glassspoon";
  WiFi.begin(ssid, password);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print ("IP address: ");
  Serial.println (WiFi.localIP());
  Serial.print ("subnetMask: ");
  Serial.println (WiFi.subnetMask());
  Serial.print ("gatewayIP: ");
  Serial.println (WiFi.gatewayIP());
  /*
  if (MDNS.begin ("esp8266"))
  {
    Serial.println ("MDNS responder started");
  }
  */
}




void Serial_Read_Data_Block (struct Data_Block * packet)
{
  Serial.readBytes ((char *) &(packet->Type), sizeof (char));
  Serial.println ("Read type");
  Serial.readBytes ((char *) &(packet->Value), Data_Block_Value_Size_Byte);
  Serial.println ("Read value");
}

void setup()
{
  Serial.setTimeout (20000);
  Serial.begin (115200);
  delay (10);
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
  Serial.print ("Message ");
  Serial.print (Size);
  Serial.print (" ");
  Serial.print (Message);
  Serial.println ();
  if (strcmp (Message, "set_ip") == 0)
  {
    WiFiClient_Read_Buffer (Item, Message, 30, '\0');
    WiFi.hostByName (Message, Client_Database_IP);
    Serial.println ("Connecting to: ");
    Serial.println (Client_Database_IP);
    Client_Database->connect (Message, 333);
    Item->print ("\n");
  }
}


void Connected_Client (WiFiClient * Item)
{
    Serial.println ("Client connected.");
    Put_Client (Item);
    Item->print ("\nHello client\n");
    delay (3000);
    Read_Command (Item);
    Item->stop ();
    delay (1);
    Serial.println ("Client disonnected");
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
    Serial.println ("No data");
    Client_Database->print ("No data\n");
    delay (1000);
  }
}



void Connected ()
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
    Serial.print ("Reconnect ");
    Serial.print (Client_Database_IP);
    Serial.print (":");
    Serial.println (Client_Database_Port);
    Client_Database->connect (Client_Database_IP, Client_Database_Port);
  }
}




void loop ()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    Connected ();
  }
  else
  {
    Serial.println ("Connect_WiFi");
    Connect_WiFi ();
  }
}




