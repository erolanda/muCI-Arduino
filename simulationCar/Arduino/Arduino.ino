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

void setup()
{ 
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
  Serial.print(nState);
}
