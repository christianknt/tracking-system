#include <Arduino.h>
#include <SPI.h>
#include <C:\Users\Christian\Desktop\tracking-system\dwm1000.h>

   //Declaracion de variables
    byte length;
    long msg, aux;

    int i=0;
    int variable=15;
    bool leido=false;
    byte control;
    byte auxiliar;

    byte buffer[127];

void setup()
{
    Serial.begin(9600);

	pinMode(MISO, INPUT);
    pinMode(MOSI, OUTPUT);
    pinMode(SCK, OUTPUT);
    pinMode(SS, OUTPUT);
    digitalWrite(SS,HIGH);
    SPI.setDataMode(SPI_MODE0);
    SPI.setClockDivider(SPI_CLOCK_DIV8);
    SPI.setBitOrder(MSBFIRST);
    SPI.begin();

    //Configuro el Dwm1000
    getDevId();
   // getEUI();
}

void loop()
{

    control=readDwm1000(SYS_CTRL,0x01);
    control=control+0x01;
    writeDwm1000(SYS_CTRL,0x01,control);

    leido=false;
    while (leido==false)
    {
        auxiliar=readDwm1000(SYS_STATUS,0x01); //Leo el bit 13 (RXDFR)
        auxiliar=auxiliar&0b00100000;
        if (auxiliar==32)
            leido=true;
    }

    //Apago recepcion y transmision
    control=readDwm1000(SYS_CTRL,0x00);
    control=(control&0b10111111)+0b01000000;
    writeDwm1000(SYS_CTRL,0x00,control);

    //Extension del dato recibido
    length=readDwm1000(RX_FINFO,0x00);
    length=length&0b01111111;
    Serial.print("Extension del dato recibido: ");
    Serial.println(length);

    //Leo el dato del buffer
    Serial.print("Dato recibido: ");

    for (i=0; i<length; i++)
    {
        buffer[i]=readDwm1000(RX_BUFFER,i);
        Serial.print(readDwm1000(RX_BUFFER,i),HEX);
    }
    Serial.println(" ");
    delay(1000);

}

