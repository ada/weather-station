#include "C:\Users\Kenth Johan\Documents\GitHub\ws\packets.h"


struct Packet My_Packet [1];


void Send (struct Packet * Item)
{
  switch (Item->Kind)
  {
    case Packet_Kind_A0:
      Item->Value_Int = analogRead (A0);
      Serial.write ((uint8_t *) Item, sizeof (struct Packet));
    break;

    case Packet_Kind_A1:
      Item->Value_Int = analogRead (A1);
      Serial.write ((uint8_t *) Item, sizeof (struct Packet));
    break;

    /*
    case 255:
      Item->Kind = 0;
    break;
    */
  }
  Item->Kind = Item->Kind + 1;
}





void setup()
{
  Serial.begin (115200);
}

void loop()
{
 
}
