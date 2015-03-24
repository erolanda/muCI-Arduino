#include <SoftwareSerial.h>

#define RxD 10
#define TxD 11
#define KEY 4

SoftwareSerial BTSerial(RxD, TxD);

void setup()
{
  pinMode(KEY, OUTPUT);
  digitalWrite(KEY, HIGH); //Modo AT
  
  delay(500);
  
  BTSerial.flush();
  delay(500);
  BTSerial.begin(38400);
  Serial.begin(9600);
  Serial.println("Enter AT commands:");

  BTSerial.print("AT\r\n");
  delay(100);

}

void loop()
{

  if (BTSerial.available())
    Serial.write(BTSerial.read());

  if (Serial.available())
    BTSerial.write(Serial.read());

}
