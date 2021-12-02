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
  byte ch1; //Y Izq
  byte ch2; //Y Der
  byte ch3; //Trig Izq
  byte ch4; //Trig Der
  //byte ch5; //X Izq
  //byte ch6; //X Der
};

int nDato[4];

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
}

void loop(){
  if(radio.available()){
    radio.read(&datos, sizeof(datosEnviados));
    ajustarDatos(150);
    verDatos();
    setMotores(nDato[1], nDato[1], nDato[0]);
    digitalWrite(STB1, nDato[2]);
    digitalWrite(STB2, nDato[3]);
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
  datos.ch3=1;
  datos.ch4=1;
  //datos.ch5=127;
  //datos.ch6=127;
}

void ajustarDatos(byte a){
  nDato[0]=map(datos.ch1, 0, 255, a, -a);
  nDato[1]=map(datos.ch2, 0, 255, -a, a);
  nDato[2]=datos.ch3;
  nDato[3]=datos.ch4;
  //nDato[4]=map(datos.ch5, 0, 255, -a, a);
  //nDato[5]=map(datos.ch6, 0, 255, a, -a);
  if(nDato[0]<30 && nDato[0]>-30) nDato[0]=0;
  if(nDato[1]<30 && nDato[1]>-30) nDato[1]=0;
}

void verDatosRaw(){
  Serial.print(datos.ch1);
  Serial.print(F(" || "));
  Serial.print(datos.ch2);
  Serial.print(F(" || "));
  Serial.print(datos.ch3);
  Serial.print(F(" || "));
  Serial.println(datos.ch4);
  /*
  Serial.print(F(" || "));
  Serial.print(datos.ch5);
  Serial.print(F(" || "));
  Serial.println(datos.ch6);
  */
}

void verDatos(){
  Serial.print(nDato[0]);
  Serial.print(F(" || "));
  Serial.print(nDato[1]);
  Serial.print(F(" || "));
  Serial.print(nDato[2]);
  Serial.print(F(" || "));
  Serial.println(nDato[3]);
  /*
  Serial.print(F(" || "));
  Serial.print(nDato[4]);
  Serial.print(F(" || "));
  Serial.println(nDato[5]);
  */
}
