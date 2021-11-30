#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define BTNDX   A4
#define BTNDY   A3
#define BTNIX   A1
#define BTNIY   A0
#define BTND    A2
#define BTNI    7
#define TRGD    3
#define TRGI    4
#define LED     A5
#define CE      9
#define CSN     10

RF24 radio(CE, CSN);

//AREÑA -> 61 72 65 F1 61
const uint64_t addr = 0x617265F161LL;

/*
byte XIzq;
byte YIzq;
byte XDer;
byte YDer;
bool BIzq;
bool BDer;
bool BJIzq;
bool BJDer;
*/

struct datosEnviados {
  byte ch1; //XIzq
  byte ch2; //YIzq
  byte ch3; //XDer
  byte ch4; //YDer
  byte ch5; //BIzq
  byte ch6; //BDer
};

datosEnviados datos;

void setup(){
  Serial.begin(115200);
  pinMode(BTND, INPUT);
  pinMode(BTNI, INPUT);
  pinMode(TRGD, INPUT);
  pinMode(TRGI, INPUT);
  pinMode(LED, OUTPUT);

  radio.begin();
  radio.setAutoAck(false);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(addr);
  radio.stopListening();

  datos.ch1=127;
  datos.ch2=127;
  datos.ch3=127;
  datos.ch4=127;
  datos.ch5=1;
  datos.ch6=1;
  
}

void loop(){
  //XIzq=map(analogRead(BTNIX), 0, 1023, 0, 255);
  /*
  YIzq=map(analogRead(BTNIY), 0, 1023, 0, 255);
  XDer=map(analogRead(BTNDX), 0, 1023, 0, 255);
  YDer=map(analogRead(BTNDY), 0, 1023, 0, 255);
  BIzq=digitalRead(TRGI);
  BDer=digitalRead(TRGD);
  BJIzq=digitalRead(BTNI);
  BJDer=digitalRead(BTND);
  */

  //radio.write(&XIzq, sizeof(XIzq));
  
  datos.ch1=map(analogRead(BTNIX), 0, 1023, 0, 255);
  datos.ch2=map(analogRead(BTNIY), 0, 1023, 0, 255);
  datos.ch3=map(analogRead(BTNDX), 0, 1023, 0, 255);
  datos.ch4=map(analogRead(BTNDY), 0, 1023, 0, 255);
  datos.ch5=digitalRead(TRGI);
  datos.ch6=digitalRead(TRGD);

  if(radio.write(&datos, sizeof(datosEnviados))){
    digitalWrite(LED, 1);
  }else{
    digitalWrite(LED, 0);
  }
}
