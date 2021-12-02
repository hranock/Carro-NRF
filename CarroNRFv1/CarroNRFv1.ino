#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define APWM    3
#define AIN1    14
#define AIN2    15
#define BPWM    5
#define BIN2    2
#define BIN1    4
#define BPWM    5
#define CPWM    6
#define CIN1    7
#define CIN2    8
#define LED     16
#define SRV     17
#define STB1    18
#define STB2    19
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

int nDato[6];

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
  pinMode(STB1, OUTPUT);
  pinMode(STB2, OUTPUT);
  
  digitalWrite(LED, 1);
  iniciarNRF();

  digitalWrite(STB1, 1);
  digitalWrite(STB2, 1);
}

void loop(){
  if(radio.available()){
    radio.read(&datos, sizeof(datosEnviados));
    ajustarDatos();
    verDatos();
    setMotores(nDato[1], nDato[1], nDato[3]);
    digitalWrite(LED, 0);
    delay(50);
  }else{
    digitalWrite(LED, 1);
  }
}

//*********** METODOS ***********

void iniciarNRF(){
  radio.begin();
  radio.setAutoAck(false);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(1, addr);
  radio.startListening();
  reiniciarDatos();
}

void setMotores(int a, int b, int c){
  puenteH(a, APWM, AIN1, AIN2);
  puenteH(b, BPWM, BIN1, BIN2);
  puenteH(c, CPWM, CIN1, CIN2);
}

void puenteH(int val, byte pwm, byte in1, byte in2){
  if(val>0){
    digitalWrite(in1, 1);
    digitalWrite(in2, 0);
    analogWrite(pwm, val);
  }else{
    digitalWrite(in1, 0);
    digitalWrite(in2, 1);
    analogWrite(pwm, -val);
  }
}

void reiniciarDatos(){
  datos.ch1=127;
  datos.ch2=127;
  datos.ch3=127;
  datos.ch4=127;
  datos.ch5=1;
  datos.ch6=1;
}

void ajustarDatos(){
  nDato[0]=map(datos.ch1, 0, 255, 255, -255);
  nDato[1]=map(datos.ch2, 0, 255, -255, 255);
  nDato[2]=map(datos.ch3, 0, 255, -255, 255);
  nDato[3]=map(datos.ch4, 0, 255, 255, -255);
  nDato[4]=datos.ch5;
  nDato[5]=datos.ch6;
}

void verDatosRaw(){
  Serial.print(datos.ch1);
  Serial.print(F(" || "));
  Serial.print(datos.ch2);
  Serial.print(F(" || "));
  Serial.print(datos.ch3);
  Serial.print(F(" || "));
  Serial.print(datos.ch4);
  Serial.print(F(" || "));
  Serial.print(datos.ch5);
  Serial.print(F(" || "));
  Serial.println(datos.ch6);
}

void verDatos(){
  Serial.print(nDato[0]);
  Serial.print(F(" || "));
  Serial.print(nDato[1]);
  Serial.print(F(" || "));
  Serial.print(nDato[2]);
  Serial.print(F(" || "));
  Serial.print(nDato[3]);
  Serial.print(F(" || "));
  Serial.print(nDato[4]);
  Serial.print(F(" || "));
  Serial.println(nDato[5]);
}
