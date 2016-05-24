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

#define Packet_Array_Count 256
struct Packet Packet_Array [Packet_Array_Count] = {0};

String HTML_Response;

void Send_Message (char * Value)
{
  
}

void Read_UART ()
{
  if (Serial.available () >= 4)
  {
    uint8_t Flag_1, Flag_2, CRC, Kind_1;
    Flag_1 = Serial.read ();
    Flag_2 = Serial.read ();
    CRC = Serial.read ();
    Kind_1 = Serial.read ();
    if (Flag_1 == Packet_Header_Flag_1 && Flag_2 == Packet_Header_Flag_2)
    {
      Serial.readBytes ((uint8_t *) &Packet_Array[Kind_1].Message, Packet_Message_Size);
      Serial.println ("Packet recieved");
    }
    else
    {
      while (Serial.available ()) {char C = Serial.read ();}
      Serial.println ("Recognised no packet.");
      Serial.write ((uint8_t *) Packet_Array + Packet_Kind_UART_Resend, Packet_Size);
    }
  }
  else
  {
    Serial.write ((uint8_t *) Packet_Array + Packet_Kind_UART_Ready, Packet_Size);
  }
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
  Serial.print ("\nConnecting to ");
  Serial.print (ssid);
  Serial.println ();
  while (WiFi.status () != WL_CONNECTED)
  {
    delay (500);
    Serial.print (".");
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
      My_Control_Server->WiFi_Client->flush ();
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
    Serial.print ("Connecting to database: ");
    Serial.print (My_Database_Connection->Address);
    Serial.println ();
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
    Packet_Array [I].Header_Flag_1 = Packet_Header_Flag_1;
    Packet_Array [I].Header_Flag_2 = Packet_Header_Flag_2;
    Packet_Array [I].Kind_1 = I;
    Packet_Array [I].Value_Int = 0;
  }
  Packet_Array [Packet_Kind_Test].Value_Int = 255;
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

