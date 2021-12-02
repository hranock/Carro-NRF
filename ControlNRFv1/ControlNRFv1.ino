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
  byte ch1; //Y Izq
  byte ch2; //Y Der
  byte ch3; //Trig Izq
  byte ch4; //Trig Der
  //byte ch5; //X Izq
  //byte ch6; //X Der
};

datosEnviados datos;

void setup(){
  Serial.begin(115200);
  pinMode(TRGD, INPUT);
  pinMode(TRGI, INPUT);
  pinMode(LED, OUTPUT);
  //pinMode(BTND, INPUT);
  //pinMode(BTNI, INPUT);

  digitalWrite(LED, 1);
  iniciarNRF();
}

void loop(){
  datos.ch1=map(analogRead(BTNIY), 0, 1023, 0, 255);
  datos.ch2=map(analogRead(BTNDY), 0, 1023, 0, 255);
  datos.ch3=digitalRead(TRGI);
  datos.ch4=digitalRead(TRGD);
  //datos.ch5=map(analogRead(BTNIX), 0, 1023, 0, 255);
  //datos.ch6=map(analogRead(BTNDX), 0, 1023, 0, 255);
  //verDatosRaw();

  if(radio.write(&datos, sizeof(datosEnviados))){
    digitalWrite(LED, 0);
  }else{
    digitalWrite(LED, 1);
  }
}

void iniciarNRF(){
  radio.begin();
  radio.setAutoAck(false);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(addr);
  radio.stopListening();
  reiniciarDatos();
}

void reiniciarDatos(){
  datos.ch1=127;
  datos.ch2=127;
  datos.ch3=1;
  datos.ch4=1;
  //datos.ch5=127;
  //datos.ch6=127;
}

void verDatosRaw(){
  Serial.print(datos.ch1);
  Serial.print(F(" || "));
  Serial.print(datos.ch2);
  Serial.print(F(" || "));
  Serial.print(datos.ch3);
  Serial.print(F(" || "));
  Serial.println(datos.ch4);
}
