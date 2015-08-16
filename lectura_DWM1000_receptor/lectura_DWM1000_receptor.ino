#include <Arduino.h>
#include <SPI.h>
#include <C:\Users\Christian\Desktop\tracking-system\dwm1000.h>

   //Declaracion de variables

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
    //Cargo desde la memoria OTP la informacion a la RAM
    writeDwm1000(OTP_IF,0x07,0b10000000); //Cargo info desde la memoria OTP

    //Configuro el Dwm1000
    setAddress(SHORT_ADDR,0x0B);

    //Imprimo algunos parametros
    getDevId();
    Serial.print("Short Address - ");
    Serial.println(getAddress(SHORT_ADDR),HEX);

    // Apago recepcion y transmision
    writeDwm1000(SYS_CTRL,0x00,0b01000000);
    /////////////////////////////////////
}

void loop()
{
    int i;

    receiveData(buffer);
    Serial.print("Dato recibido: ");
     for (i=0; i<7; i++)
    {
        Serial.print(buffer[i],HEX);
    }
    Serial.println(" ");
    buffer[4]=buffer[4]+1;
    delay(1000);
    sendData(buffer,7);
    Serial.println("\n");

}

