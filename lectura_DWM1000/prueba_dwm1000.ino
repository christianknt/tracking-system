#include <Arduino.h>
#include <SPI.h>
#include <C:\Users\Christian\Desktop\tracking-system\dwm1000.h>

long msg, aux;

int i=0;
int variable=15;
bool configuracion=false, transmitido=false;
byte control, auxiliar;
byte length;

void setup()
{
    Serial.begin(9600);

	pinMode(50, INPUT);
    pinMode(51, OUTPUT);
    pinMode(52, OUTPUT);
    pinMode(SS, OUTPUT);
    digitalWrite(SS,HIGH);
    SPI.setDataMode(SPI_MODE0);
    SPI.setClockDivider(SPI_CLOCK_DIV128);
    SPI.setBitOrder(MSBFIRST);
    SPI.begin();
}

void loop()
{
    if (configuracion==false)
    {
        //Imprimo en pantalla la identificacion del modulo
        configuracion=true;
        Serial.println("--");
        getDevId();
       // getEUI();

    }

    //Dato a enviar
    writeDwm1000(TX_BUFFER,0x00,0x0D);
    writeDwm1000(TX_BUFFER,0x01,0x0E);
    writeDwm1000(TX_BUFFER,0x02,0x0C);
    writeDwm1000(TX_BUFFER,0x03,0x0A);
    writeDwm1000(TX_BUFFER,0x04,0x02);
    writeDwm1000(TX_BUFFER,0x05,0x00);
    writeDwm1000(TX_BUFFER,0x06,0x01);
    writeDwm1000(TX_BUFFER,0x07,0x05);

    //Largo del dato a enviar
    length=10;
    writeDwm1000(TX_FCTRL,0x00,length);

    //Inicio transmision
    control=readDwm1000(SYS_CTRL,0x00);
    control=control+0x02;
    writeDwm1000(SYS_CTRL,0x00,control);

    transmitido=false;
    while (transmitido==false)
    {
        auxiliar=readDwm1000(SYS_STATUS,0x00); //Leo el bit 13 (TXFRS)
        auxiliar=auxiliar&0b10000000;
        if (auxiliar==128)
        {
            transmitido=true;
        }
    }

    //Apago recepcion y transmision
    control=readDwm1000(SYS_CTRL,0x00);
    control=(control&0b10111111)+0b01000000;
    writeDwm1000(SYS_CTRL,0x00,control);
    ///////////////////////////////////////

    Serial.println("Data sent: OK");
    while (transmitido==true)
    {
        transmitido=true;
        delay(100);
    }
}


