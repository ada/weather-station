#ifndef Packets_H
#define Packets_H


#define Packet_Kind_Message 'M'
#define Packet_Kind_Response 'R'
#define Packet_Kind_Float 'f'
#define Packet_Kind_Int 'i'
#define Packet_Kind_A0 6
#define Packet_Kind_A1 7

#define Packet_Size 16
#define Packet_Message_Size 15

struct Packet
{
  union
  {
    char Data [Packet_Size];
    struct
    {
      char Kind;
      union
      {
        char Message [Packet_Message_Size];
        int Value_Int;
        float Value_Float;
      };
    };
  };
};



#endif
