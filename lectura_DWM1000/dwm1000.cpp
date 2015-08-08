#include <Arduino.h>
#include <SPI.h>
#include <dwm1000.h>
//hola

void getDevId()
{
    uint16_t RIDTAG=0;
    uint8_t MODEL=0, VERSION=0;
    uint16_t aux=0;

//Lectura del DEV_ID
    aux=readDwm1000(DEV_ID,0x03);
    aux=aux<<8;
    RIDTAG=aux+readDwm1000(DEV_ID,0x02);
    MODEL=readDwm1000(DEV_ID,1); //Leo el model del dispositivo
    VERSION=readDwm1000(DEV_ID,0); //Leo la version del dispositivo

    //Imprimo los datos en pantalla por puerto serie.
    Serial.print("Device: ");
    Serial.print(RIDTAG,HEX);
    Serial.print(" Ver. ");
    Serial.print(MODEL,HEX);
    Serial.print(".");
    Serial.println(VERSION,HEX);

}

void getEUI()
{
    int i=0;
    uint64_t EUI_leido=0;
    int aux=0;

    Serial.print("EUI: ");
    for (i=0; i<8; i++)
    {
        aux=readDwm1000(EUI,i);
        Serial.print(aux,DEC);
        Serial.print(".");
        //aux=aux<<(i*8);
        //EUI_leido=EUI_leido+aux;
    }
    Serial.println(".");
}

void writeDwm1000(int dir, word subDir, int data)
{
    int byte1=0, byte2=0, byte3=0;
    word aux;

    if ((subDir>=0)&&(subDir<=0x7F))
    {
        byte1=0b11000000+dir; //Indica escritura y que hay segundo octeto

        byte2=0b000000001111111&subDir; //Indica que no hay tercer octeto

        digitalWrite(SS,LOW);
        SPI.transfer(byte1);
        SPI.transfer(byte2);
        SPI.transfer(data);
        digitalWrite(SS,HIGH);
    }
    else
    if ((subDir>0x7F)&&(subDir<=0x7FFF))
    {
        byte1=0b11000000+dir; //Indica escritura y que hay segundo octeto

        byte2=0b000000001111111&subDir;
        byte2=byte2+0b10000000; //Indica que hay tercer octeto

        aux=0b111111110000000&subDir;
        aux=aux>>7;
        byte3=aux;

        digitalWrite(SS,LOW);
        SPI.transfer(byte1);
        SPI.transfer(byte2);
        SPI.transfer(byte3);
        SPI.transfer(data);
        digitalWrite(SS,HIGH);
    }
}


byte readDwm1000(int dir, word subDir)
{
    int byte1=0, byte2=0, byte3=0;
    word aux;
    byte msg=0;

    if ((subDir>=0)&&(subDir<=0x7F))
    {
        byte1=0b01000000+dir;
        byte2=0b000000001111111&subDir;

        digitalWrite(SS,LOW);
        SPI.transfer(byte1);
        SPI.transfer(byte2);
        msg=SPI.transfer(0x00);
        digitalWrite(SS,HIGH);
    }
    else
    if ((subDir>0x7F)&&(subDir<=0x7FFF))
    {
        byte1=0b01000000+dir;
        byte2=0b000000001111111&subDir;
        byte2=byte2+0b10000000; //Indica que hay proximo octeto
        aux=0b111111110000000&subDir;
        aux=aux>>7;
        byte3=aux;


        digitalWrite(SS,LOW);
        SPI.transfer(byte1);
        SPI.transfer(byte2);
        SPI.transfer(byte3);
        msg=SPI.transfer(0x00);
        digitalWrite(SS,HIGH);
    }

    return msg;
}


void sendData(byte data[], byte length)
{

}

void receiveData()
{

}
