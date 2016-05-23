#ifndef Packets_H
#define Packets_H

#define Packet_Kind_Test 1
#define Packet_Kind_A0 2
#define Packet_Kind_A1 3
#define Packet_Kind_RSSI 4
#define Packet_Kind_UART_Ready 5
#define Packet_Kind_Reconnect 6
#define Packet_Kind_WiFi_Connect 7

#define Packet_Size 12
#define Packet_Message_Size 8

struct Packet
{
  union
  {
    uint8_t Data [Packet_Size];
    struct
    {
      uint8_t Kind_1;
      uint8_t Kind_2;
      uint8_t Kind_3;
      uint8_t Kind_4;
      union
      {
        uint8_t Message [Packet_Message_Size];
        int32_t Value_Int;
      };
    };
  };
};



#endif
