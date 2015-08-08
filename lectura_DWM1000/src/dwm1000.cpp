#include "dwm1000.h"
#include <Arduino.h>
#include <SPI.h>

dwm1000::dwm1000()
{
    //ctor
}

long dwm1000::leerTag()
{
    digitalWrite(53,LOW);
    SPI.transfer(0x00);
    msg1 = SPI.transfer(0x00);
    msg2 = SPI.transfer(0x00);
    msg3 = SPI.transfer(0x00);
    msg4 = SPI.transfer(0x00);
    digitalWrite(53,HIGH);
    msg=msg4;
    msg=(msg<<8)+msg3;
    msg=(msg<<8)+msg2;
    msg=(msg<<8)+msg1;
    return msg;
}

dwm1000::~dwm1000()
{
    //dtor
}
