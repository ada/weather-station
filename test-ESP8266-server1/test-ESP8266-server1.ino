#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <stdio.h>
#include <string.h>
#include <ArduinoJson.h>
#include "C:\Users\Alex\Documents\GitHub\ws\packets.h"

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
};

WiFiServer Server_Web (8267);
struct Database_Connection My_Database_Connection [1];
struct Control_Server My_Control_Server [1];
struct Packet Response_Packet [1] = {0};

#define Packet_Array_Count 255
struct Packet Packet_Array [Packet_Array_Count] = {0};

String HTML_Response;

void Read_UART ()
{
  uint8_t Kind_1, Kind_2, Kind_3, Kind_4;
  if (Serial.available () == Packet_Size)
  {
    Kind_1 = Serial.read ();
    Kind_2 = Serial.read ();
    Kind_3 = Serial.read ();
    Kind_4 = Serial.read ();
    Serial.readBytes ((uint8_t *) &Packet_Array[Kind_1].Message, Packet_Message_Size);
  }
  else if (Serial.available () > Packet_Size)
  {}
}

void Read_RSSI ()
{
  Packet_Array [Packet_Kind_RSSI].Value_Int = WiFi.RSSI ();
}


void WiFi_Connect ()
{
  const char* ssid = "home";
  const char* password = "mynameis";
  WiFi.begin (ssid, password);
  //Serial.print ("\nConnecting to ");
  //Serial.print (ssid);
  //Serial.println ();
  while (WiFi.status () != WL_CONNECTED)
  {
    delay(500);
    Response_Packet->Kind_1 = Packet_Kind_WiFi_Connect;
    memcpy (Response_Packet->Message, "WiFiCon\n", Packet_Message_Size);
    Serial.write (Response_Packet->Data, Packet_Size);
  }
  //Serial.print ("\nWiFi connected\n");
  //Serial.print ("IP address: ");
  //Serial.print (WiFi.localIP());
  //Serial.println ();
  //Serial.print ("subnetMask: ");
  //Serial.print (WiFi.subnetMask());
  //Serial.println ();
  //Serial.print ("gatewayIP: ");
  //Serial.print (WiFi.gatewayIP());
  //Serial.println ();
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

  WiFiClient Server_Web_Client = Server_Web.available();
  Server_Web_Client.flush ();
  if (Server_Web_Client)
  {
    HTML_Response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n\r\n";
    HTML_Response += "<html><head><title>";
    HTML_Response += "ESP8266";
    HTML_Response += "</title></head><body><pre>";
    for (int I = 0; I < Packet_Array_Count; I = I + 1)
    {
      HTML_Response += Packet_Array [I].Kind_1;
      HTML_Response += ',';
      HTML_Response += Packet_Array [I].Kind_2;
      HTML_Response += ',';
      HTML_Response+= Packet_Array [I].Kind_3;
      HTML_Response += ',';
      HTML_Response += Packet_Array [I].Kind_4;
      HTML_Response += ',';
      HTML_Response += Packet_Array [I].Value_Int;
      HTML_Response += "\r\n";
    }
    HTML_Response += "</pre></body></html>";
    Server_Web_Client.print (HTML_Response);
    Server_Web_Client.flush ();
    Server_Web_Client.stop ();
  }

  
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
    My_Control_Server->WiFi_Server.begin ();
  }
  
  if (My_Database_Connection->WiFi_Client->connected ())
  {
    My_Database_Connection->WiFi_Client->flush ();
    static uint8_t Kind_Iterator = 0;
    static int D = 0;
    D = D + 1;
    if (D > 1000)
    {
      D = 0;
      My_Database_Connection->WiFi_Client->write ((uint8_t *) (Packet_Array + Kind_Iterator), Packet_Size);
      Kind_Iterator = Kind_Iterator + 1;
      if (Kind_Iterator >= Packet_Array_Count)
      {
        Kind_Iterator = 0;
      }
    }
    My_Database_Connection->WiFi_Client->flush ();
  }
  else
  {
    Response_Packet->Kind_1 = Packet_Kind_Reconnect;
    memcpy (Response_Packet->Message, "DB_CON.\n", Packet_Message_Size);
    Serial.write (Response_Packet->Data, Packet_Size);
    My_Database_Connection->WiFi_Client->connect (My_Database_Connection->Address, My_Database_Connection->Port);
  }

  Read_UART ();
  Read_RSSI ();
  
}


void setup ()
{
  Serial.setTimeout (20000);
  Serial.begin (115200);
  delay (10);
  for (int I = 0; I < Packet_Array_Count; I = I + 1)
  {
    Packet_Array [I].Kind_1 = I;
  }
  Packet_Array [255].Kind_1 = 255;
  Packet_Array [255].Kind_2 = 255;
  Packet_Array [255].Kind_3 = 255;
  Packet_Array [255].Kind_4 = 255;
  Packet_Array [255].Value_Int = 255;
  HTML_Response.reserve (10000);
}


void loop ()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    WiFi_Connected ();
  }
  else
  {
    WiFi_Connect ();
    delay (10);
    Server_Web.begin ();
  }
}

