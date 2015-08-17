#include <Arduino.h>
#include <SPI.h>
#include <C:\Users\Christian\Desktop\tracking-system\dwm1000.h>


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
    uint64_t EUI_leido=0, aux2=0;
    int aux=0;

    Serial.print("EUI: ");
    for (i=0; i<8; i++)
    {
        aux=readDwm1000(EUI,i);
        aux2=aux<<(i*8);
        EUI_leido=EUI_leido+aux2;
        Serial.print(aux);
        Serial.print(".");
    }
    Serial.println("\n");

}


uint16_t getAddress(byte type)
{
    uint16_t address=0;
    uint16_t auxiliar=0; //Auxiliar variable

    address=readDwm1000(PANADR,type); //Read first 8 bits of address 'type'
    auxiliar=readDwm1000(PANADR,type+1);//Read last 8 bits of address 'type'
    address=address+(auxiliar<<8);
    return address;
}

void setAddress(byte type, uint16_t address)
{
    uint16_t auxiliar=0; //Auxiliar variable
    auxiliar=0x00FF&address; //Write first 8 bits of address on the 'type' register
    writeDwm1000(PANADR,type,auxiliar);
    auxiliar=(0xFF00&address)>>8; //Write last 8 bits of address on the 'type' register
    writeDwm1000(PANADR,type+1,auxiliar);
}


void writeDwm1000(uint8_t dir, uint16_t subDir, uint8_t data)
{
    int byte1=0, byte2=0, byte3=0;
    uint16_t aux;

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

        byte2=0b0000000001111111&subDir;
        byte2=byte2+0b10000000; //Indica que hay tercer octeto

        aux=0b0111111110000000&subDir;
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


byte readDwm1000(uint8_t dir, uint16_t subDir)
{
    int byte1=0, byte2=0, byte3=0;
    uint16_t aux;
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
        aux=0b0111111110000000&subDir;
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


void sendData(byte buffer[], int length)
{
    bool transmitido=false;
    byte auxiliar, auxiliar2;
    byte control;
    int i;

// Apago recepcion y transmision
    writeDwm1000(SYS_CTRL,0x00,0b01000000);
    /////////////////////////////////////

//    Dato a enviar
    for (i=0; i<length; i++)
    {
        writeDwm1000(TX_BUFFER,i,buffer[i]); //Copio los datos a enviar en el buffer de salida.
    }
//    Largo del dato a enviar
    writeDwm1000(TX_FCTRL,0x00,length+2);

//    Inicio transmision
    writeDwm1000(SYS_CTRL,0x00,0b00000010);

    transmitido=false;
    while (transmitido==false)
    {
        auxiliar=readDwm1000(SYS_STATUS,0x00); //Leo el bit 7 (TXFRS)
        if ((auxiliar&0b10000000)==128)
        {
            Serial.println("Data sent: OK");
            writeDwm1000(SYS_STATUS,0x00,0b11111110); //Limpio las variables informativas
            transmitido=true;
        }
        else
        {
            auxiliar=readDwm1000(SYS_STATUS,0x03);
            if ((auxiliar&0b00010000)==16)
            {
                Serial.println("Error: Transmit Buffer Error");
                writeDwm1000(SYS_STATUS,0x00,0b11111110); //Limpio las variables informativas
                transmitido=false;
                break;
            }
        }
    }

   // Apago recepcion y transmision
    writeDwm1000(SYS_CTRL,0x00,0b01000000);
    /////////////////////////////////////


}

void receiveData(byte buffer[])
{
    byte control;
    bool leido=false;
    bool error=false;
    byte auxiliar, auxiliar2;
    int i=0;
    byte length;
    uint32_t TOF=0, auxiliar3=0;
    float distance;

// Apago recepcion y transmision
    writeDwm1000(SYS_CTRL,0x00,0b01000000);
    /////////////////////////////////////


    //Habilito la recepcion
    control=readDwm1000(SYS_CTRL,0x01);
    control=(control&0b11111110)+0x01;
    writeDwm1000(SYS_CTRL,0x01,control);

    //Espero a recibir algun dato
    leido=false;
    while (leido==false)
    {
        auxiliar=readDwm1000(SYS_STATUS,0x01);
        auxiliar2=readDwm1000(SYS_STATUS,0x02);

        if ((auxiliar&0b00100000)==32) //Leo el bit 13 (RXDFR)
        {
            Serial.println("Data received: OK");
            leido=true; //Si recibo dato, salgo del while
        }
        else
        {
            if ((auxiliar&0b00010000)==16) //RXPHE
            {
                Serial.println("Error: Receiver PHY header Error");
                leido=false;
                break; //Si recibo dato incorrecto, salgo del while
            }
            else
            {
                if ((auxiliar2&0b00010000)==16) //RXOVRR Receiver Overrun
                {
                    Serial.println("Error: Receiver Overrun");
                    leido=false;
                    break; //Si recibo dato incorrecto, salgo del while
                }
            }
        }
    }

   // Apago recepcion y transmision
    control=0b01000000;
    writeDwm1000(SYS_CTRL,0x00,control);
    /////////////////////////////////////

    //Leo extension del dato recibido
    length=readDwm1000(RX_FINFO,0x00);
    length=length&0b01111111;

    for (i=0; i<length; i++)
    {
        buffer[i]=readDwm1000(RX_BUFFER,i); //Leo el byte i, del buffer de recepcion
    }

//Calculo la distancia entre los dos moviles
    TOF=0;
    auxiliar3=0;
    for (i=0; i<3; i++)
    {
        auxiliar=readDwm1000(RX_TIME,i);
        auxiliar3=auxiliar<<(i*8);
        TOF=TOF+auxiliar3;
    }
    distance=TIME_RES*TOF;
    distance=distance*SPEED_OF_LIGHT;
//    Serial.print("Distance: ");
//    Serial.println(distance);

}

void loadLDE()
{
     //Cargo desde la memoria OTP la informacion a la RAM
    writeDwm1000(PMSC,0x00,0x01);
    writeDwm1000(OTP_IF,0x07,0b10000000); //Cargo info desde la memoria OTP
    delayMicroseconds(200);
    writeDwm1000(PMSC,0x00,0x00);
}
