#ifndef Packets_H
#define Packets_H


#define Packet_Kind_A0 2
#define Packet_Kind_A1 3
#define Packet_Kind_RSSI 4

#define Packet_Kind_UART_Ready 5
#define Packet_Kind_UART_Resend 6

#define Packet_Kind_Reconnect 12
#define Packet_Kind_WiFi_Connect 10

#define Packet_Kind_Test 255

#define Packet_Header_Flag_1 255
#define Packet_Header_Flag_2 254

#define Packet_Size 12
#define Packet_Message_Size 8

struct Packet
{
  union
  {
    uint8_t Data [Packet_Size];
    struct
    {
      uint8_t Header_Flag_1;
      uint8_t Header_Flag_2;
      uint8_t CRC;
      uint8_t Kind_1;
      union
      {
        uint8_t Message [Packet_Message_Size];
        int32_t Value_Int;
      };
    };
  };
};


const char * Packets_Get_Kind_Name (uint8_t Value)
{
  switch (Value)
  {
    case Packet_Kind_A0:
    return "Kind A0";
    case Packet_Kind_A1:
    return "Kind A1";
  }
}


#endif
