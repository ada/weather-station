#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
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
  uint16_t Port = 333;
};


ESP8266WebServer Server_Web (8267);
struct Database_Connection My_Database_Connection [1];
struct Control_Server My_Control_Server [1];
struct Data_Block My_Data_Block [1] = {0};



void Database_Connection_Serial_Put_Line_Address (struct Database_Connection * Item)
{
  Serial.print (Item->Address);
  Serial.print (":");
  Serial.println (Item->Port);
}

void Database_Connection_Connect (struct Database_Connection * Item)
{
  My_Database_Connection->WiFi_Client->connect (Item->Address, Item->Port);
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
    if (I >= Size)
    {
      break;
    }
    if (!Item->available ())
    {
      break;
    }
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
  if (Serial.available () > 0)
  {
    Serial_Read_Data_Block (My_Data_Block);
    Serial_Put_Data_Block (My_Data_Block);
    Item->WiFi_Client->write ((const uint8_t *) My_Data_Block, sizeof (struct Data_Block));
  }
  else
  {
    Serial.println ("Waiting for UART data.");
    Item->WiFi_Client->print ("Waiting for UART data.\n");
    delay (1000);
  }
}


void Control_Server_Read_Message (struct Control_Server * Item)
{
  int Size;
  Size = WiFiClient_Read_Buffer (Item->WiFi_Client, Item->Message, Control_Server_Message_Length, '\0');
  Serial.print ("\nMessage ");
  Serial.print (Size);
  Serial.print ("Bytes:\n");
  Serial.print (Item->Message);
  Serial.println ();
}


void Control_Server_Read_Command (Control_Server * Item)
{
  Control_Server_Read_Message (Item);
  if (strcmp (Item->Message, "set_ip") == 0)
  {
    Serial.println ("command set_ip");
    Control_Server_Read_Message (Item);
    WiFi.hostByName (Item->Message, My_Database_Connection->Address);
    My_Database_Connection->WiFi_Client->stop ();
  }

  if (strcmp (Item->Message, "set_port") == 0)
  {
    Serial.println ("command set_port");
    Control_Server_Read_Message (Item);
    My_Database_Connection->Port = String (Item->Message).toInt ();
    Serial.println (My_Database_Connection->Port);
    My_Database_Connection->WiFi_Client->stop ();
  }
}





void WiFi_Connected ()
{
  Server_Web.handleClient();

  if (My_Control_Server->WiFi_Server.status ())
  {
    if (My_Control_Server->WiFi_Server.hasClient ())
    {
      Serial.print ("\nClient connected.\n");
      My_Control_Server->WiFi_Client [0] = My_Control_Server->WiFi_Server.available ();
      Serial_Put_WiFIClient (My_Control_Server->WiFi_Client);
      My_Control_Server->WiFi_Client->print ("\nHello client\n");
      delay (3000);
      Control_Server_Read_Command (My_Control_Server);
      My_Control_Server->WiFi_Client->stop ();
      delay (1);
      Serial.print ("Client disonnected\n");
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
    Database_Connection_Serial_Put_Line_Address (My_Database_Connection);
    Database_Connection_Connect (My_Database_Connection);
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
      
      html += "Last Data_Block: ";
      html += "\n";
      
      html += "Type: ";
      html += (int) My_Data_Block->Type;
      html += "\n";
      
      html += "Value: ";
      for (int I = 0; I < Data_Block_Value_Size_Byte; ++I)
      {
        html += (int) My_Data_Block->Value [I];
        html += " ";
      }
      html += "\n";
      
      html += "Value_Float: ";
      html += My_Data_Block->Value_Float;
      html += "\n";
      
      html += "Value_Int: ";
      html += My_Data_Block->Value_Int;
      html += "\n";
      
      Server_Web.send (200, "text/plain", html);
    });
    
    Serial.println ("Server_Web Begin");
    Server_Web.begin ();
  }
}




