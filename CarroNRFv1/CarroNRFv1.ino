#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define APWM    3
#define AIN1    A1
#define AIN2    A0
#define BPWM    5
#define BIN2    2
#define BIN1    4
#define BPWM    5
#define CPWM    6
#define CIN1    7
#define CIN2    8
#define LED     A2
#define SRV     A3
#define CE      9
#define CSN     10

RF24 radio(CE, CSN);
//AREÑA -> 61 72 65 F1 61
const uint64_t addr = 0x617265F161LL;

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
  pinMode(APWM, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BPWM, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(CPWM, OUTPUT);
  pinMode(CIN1, OUTPUT);
  pinMode(CIN2, OUTPUT);
  pinMode(LED, OUTPUT);

  iniciarNRF();
}

void loop(){
  if(radio.available()){
    radio.read(&datos, sizeof(datosEnviados));
    digitalWrite(LED, 1);
  }else{
    digitalWrite(LED, 0);
  }
}

//*********** METODOS ***********
void iniciarNRF(){
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

void setMotores(byte a, byte b, byte c){
  puenteH(a, APWM, AIN1, AIN2);
  puenteH(b, BPWM, BIN1, BIN2);
  puenteH(c, CPWM, CIN1, CIN2);
}

void puenteH(byte V, char* PWM, char* IN1, char* IN2){
  if(V>0){
    digitalWrite(IN1, 1);
    digitalWrite(IN2, 0);
    analogWrite(PWM, V);
  }else{
    digitalWrite(IN1, 0);
    digitalWrite(IN2, 1);
    analogWrite(PWM, -V);
  }
}
