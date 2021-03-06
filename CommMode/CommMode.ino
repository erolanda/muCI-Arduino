#include <SoftwareSerial.h>

#define RxD 10
#define TxD 11
#define KEY 4
#define STATES 4   //sets states
#define MAX 10      //maximum posible reading
#define SAMPLING 50

int reading[SAMPLING];
int finalReading;
byte nState = 0;
byte multiplier = 1;

SoftwareSerial BTSerial(RxD, TxD);
byte moveFordward [] = {0x0C,0x00,0x80,0x04,0x01,0x32,0x05,0x02,0x00,0x20,0x00,0x00,0x00,0x00,0x0C,0x00,0x80,0x04,0x02,0x32,0x05,0x02,0x00,0x20,0x00,0x00,0x00,0x00};
byte moveStop [] = {0x0C,0x00,0x80,0x04,0x01,0x00,0x05,0x02,0x00,0x20,0x00,0x00,0x00,0x00,0x0C,0x00,0x80,0x04,0x02,0x00,0x05,0x02,0x00,0x20,0x00,0x00,0x00,0x00};
byte moveLeft [] = {0x0C,0x00,0x80,0x04,0x01,0x32,0x05,0x02,0x00,0x20,0x00,0x00,0x00,0x00,0x0C,0x00,0x80,0x04,0x02,0x19,0x05,0x02,0x00,0x20,0x00,0x00,0x00,0x00};
byte moveRight [] = {0x0C,0x00,0x80,0x04,0x01,0x19,0x05,0x02,0x00,0x20,0x00,0x00,0x00,0x00,0x0C,0x00,0x80,0x04,0x02,0x32,0x05,0x02,0x00,0x20,0x00,0x00,0x00,0x00};

void setup()
{
  pinMode(KEY, OUTPUT);
  digitalWrite(KEY, LOW);   // Communication mode
  
  BTSerial.begin(38400);
  BTSerial.flush();
  delay(500);
  
  Serial.begin(9600);
}

void loop()
{
  for(int i = 0; i < SAMPLING; i++){    //take ten readings in ~0.02 seconds
    reading[i] = analogRead(A0) * multiplier;
    delay(2);
  }
  for(int i = 0; i < SAMPLING; i++){   //average the ten readings
    finalReading += reading[i];
  }
  finalReading /= SAMPLING;
  finalReading = constrain(finalReading, 0, MAX);
  nState = map(finalReading, 0, MAX, 0, STATES);
  Serial.println(nState);
  if (nState == 0){
    Serial.println("Detenido");
    BTSerial.write(moveStop,sizeof(moveStop));
    delay(100);
  }else if (nState == 1){
    Serial.println("Adelante");
    BTSerial.write(moveFordward,sizeof(moveFordward));
    delay(100);
  }else if ((nState>1) && (nState<3)){
    Serial.println("Izquierda");
    BTSerial.write(moveLeft,sizeof(moveLeft));
    delay(100);
  }else {
    Serial.println("Derecha");
    BTSerial.write(moveRight,sizeof(moveRight));
    delay(100);
  }
}
