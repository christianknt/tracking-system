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

    loadLDE();

    //Configuro el Dwm1000
    setAddress(SHORT_ADDR,0x000B);
    setAddress(PAN_ID,0xDECA);

    //Imprimo algunos parametros
    getDevId();
    Serial.print("Short Address - ");
    Serial.println(getAddress(PAN_ID),HEX);

    // Apago recepcion y transmision
    writeDwm1000(SYS_CTRL,0x00,0b01000000);
    /////////////////////////////////////
}

void loop()
{
    int i;
    //uint16_t framecontrol=buildFrameControl(DATA, DESTINATION_SHORT, SOURCE_SHORT);

//    receiveData(buffer);
//    Serial.print("Dato recibido: ");
//     for (i=0; i<7; i++)
//    {
//        Serial.print(buffer[i],HEX);
//    }
//    Serial.println(" ");
//    buffer[4]=buffer[4]+1;
//    delay(1000);
//    sendData(buffer,7);
//    Serial.println("\n");

    delay(10);
    buildMACMessage(buffer);
    sendData(buffer,13);
    delay(1000);

}

