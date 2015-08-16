#ifndef DWM1000_H_INCLUDED
#define DWM1000_H_INCLUDED

#define DEV_ID 0x00 //Identificador del dispositivo
#define EUI 0x01 //Identificador unico del dispositivo
#define PANADR 0x03//PAN identifier and short address
#define TX_FCTRL 0x08 //Configuracion del frame transmitido
#define TX_BUFFER 0x09 //Transmission buffer
#define SYS_CTRL 0x0D
#define SYS_STATUS 0x0F
#define RX_FINFO 0x10 //Informacion sobre el frame recibido
#define RX_BUFFER 0x11 //Buffer de recepcion
#define RX_TIME 0x15 //Tiempo de recepcion
#define TX_TIME 0x17 //Tiempo de envio
#define CHAN_CTRL 0x1F //Registro de control del canal
#define AON_WCFG 0x2C //AON wakeup configuration register
#define OTP_IF 0x2D
#define PMSC 0x36 //Power Management and System control
//Sub-registers
#define SHORT_ADDR 0
#define PAN_ID 2


void writeDwm1000(uint8_t dir, uint16_t subDir, uint8_t data);
byte readDwm1000(uint8_t dir, uint16_t subDir);

void getDevId();
void getEUI(); //Return the EUI value
uint16_t getAddress(byte type); //Return PAN Adress or Short Adress, depending on the argument choosed
void setAddress(byte type, uint16_t address);

void sendData(byte buffer[], int length);
void receiveData(byte buffer[]);
#endif // DWM1000_H_INCLUDED
