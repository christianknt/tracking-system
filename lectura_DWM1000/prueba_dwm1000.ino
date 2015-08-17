#include <Arduino.h>
#include <SPI.h>
#include <C:\Users\Christian\Desktop\tracking-system\dwm1000.h>
#include <math.h>

byte buffer[127];

void setup()
{
    Serial.begin(9600);

	pinMode(50, INPUT);
    pinMode(51, OUTPUT);
    pinMode(52, OUTPUT);
    pinMode(53, OUTPUT);
    digitalWrite(SS,HIGH);
    SPI.setDataMode(SPI_MODE0);
    SPI.setClockDivider(SPI_CLOCK_DIV128);
    SPI.setBitOrder(MSBFIRST);
    SPI.begin();

    //Cargo desde la memoria OTP la informacion a la RAM
    loadLDE();

    getDevId();
    buffer[0]=0x0D;
    buffer[1]=0x0E;
    buffer[2]=0x0C;
    buffer[3]=0x0A;
    buffer[4]=0x00;
    buffer[5]=0x00;
    buffer[6]=0x00;
    setAddress(SHORT_ADDR,0x000A);
    Serial.print("Short Address - ");
    Serial.println(getAddress(SHORT_ADDR),HEX);

    setAddress(PAN_ID,0xDECC);

    // Apago recepcion y transmision
    writeDwm1000(SYS_CTRL,0x00,0b01000000);
    /////////////////////////////////////
}

void loop()
{
    int i=0;

    sendData(buffer,7);
    receiveData(buffer);

    Serial.print("Dato recibido: ");
    for (i=0; i<7; i++)
    {
        Serial.print(buffer[i],HEX);
    }
    Serial.println(" ");
    delay(1000);
}


