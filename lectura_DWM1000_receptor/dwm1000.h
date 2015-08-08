#ifndef DWM1000_H_INCLUDED
#define DWM1000_H_INCLUDED

#define DEV_ID 0x00 //Identificador del dispositivo
#define EUI 0x01 //Identificador unico del dispositivo
#define TX_FCTRL 0x08 //Configuracion del frame transmitido
#define TX_BUFFER 0x09 //Buffer de transmision
#define SYS_CTRL 0x0D
#define SYS_STATUS 0x0F
#define RX_FINFO 0x10 //Informacion sobre el frame recibido
#define RX_BUFFER 0x11 //Buffer de recepcion
#define CHAN_CTRL 0x1F //Registro de control del canal

//typedef struct wrapper{
//int i;
//char a;
//}wrap;


void writeDwm1000(int dir, word subDir, int data);
byte readDwm1000(int dir, word subDir);

void getDevId();
void getEUI(); //Devuelve el valor del identificador unico del dispositivo

void sendData(byte data, int length);
void receiveData(byte[127] buffer);
#endif // DWM1000_H_INCLUDED
