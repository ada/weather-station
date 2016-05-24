#include "C:\Users\Alex\Documents\GitHub\ws\packets.h"


#define isascii(c)  ((c & ~0x7F) == 0)


void Send (struct Packet * Item)
{
  switch (Item->Kind_1)
  {
    case Packet_Kind_A0:
      Item->Value_Int = analogRead (A0);
      Serial1.write ((uint8_t *) Item, sizeof (struct Packet));
    break;

    case Packet_Kind_A1:
      Item->Value_Int = analogRead (A1);
      Serial1.write ((uint8_t *) Item, sizeof (struct Packet));
    break;

    /*
    case 255:
      Item->Kind = 0;
    break;
    */
  }
  Item->Kind_1 = Item->Kind_1 + 1;
}





void setup()
{
  Serial.begin (115200);
  Serial1.begin (115200);
}


struct Packet My_Packet [1] = {Packet_Header_Flag_1, Packet_Header_Flag_2, 0, 0, 0};

uint32_t D = 0;

void loop()
{
 uint8_t Header [4];
 while (Serial1.available () >= 4)
 {
  Serial1.readBytes (Header, 4);
  if (Header [0] == Packet_Header_Flag_1 && Header [1] == Packet_Header_Flag_2 && Header [3] == Packet_Kind_UART_Ready)
  {
    Serial.println ("Packet recieved by ESP8266.");
    while (Serial1.available ()){char C = Serial1.read ();}
    Send (My_Packet);
  }
  else if (Header [0] == Packet_Header_Flag_1 && Header [1] == Packet_Header_Flag_2 && Header [3] == Packet_Kind_UART_Resend)
  {
    Serial.println ("Packet not recieved by ESP8266.");
    while (Serial1.available ()){char C = Serial1.read ();}
    Send (My_Packet);
  }
  else
  {
    Serial.write (Header, 4);
    while (Serial1.available ())
    {
      char C = Serial1.read ();
      Serial.write (C);
    }
  }
 }

 D = D + 1;
 if (D > 500000)
 {
  D = 0;
  Send (My_Packet);
  Serial.println ("Send");
 }
/*
 while (Serial1.available () > 0)
 {
  Serial.write (Serial1.read());
 }
 */
 
 
}
