#include <SPI.h>
#include <mcp4xxx.h>
MCP4XXX pot(10);
int drumPins[] = {7,6,5,4,3}; //digital pin values for drums
int drumState[] = {0, 0, 0, 0, 0}; //array holding on/off drum states
int tempo = 40;
int thisSequence = 0;
int thisStep = 0;
int serialVal=0;
int sequence[][5] = {
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0}};



void setup() {
  // initialize digital pin 13 as an output.
  Serial.begin(115200);
  pinMode(drumPins[0], OUTPUT);
  pinMode(drumPins[1], OUTPUT);
  pinMode(drumPins[2], OUTPUT);
  pinMode(drumPins[3], OUTPUT);
  pinMode(drumPins[4], OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  for(int i=0; i <5; i++){
    drumState[i]=sequence[thisStep][i];
  }
  for (int i = 0; i < 5; i++) {
    if (drumState[i] == 1) {
      digitalWrite(drumPins[i], HIGH);
      //      Serial.print(" TRIG: ");
      //      Serial.print(drumPins[i]);
    }
  }
  //Serial.println("");
  delay(10);
  for (int i = 0; i < 5; i++) {
    drumState[i] = 0;
    digitalWrite(drumPins[i], LOW);
  }
  thisStep=(thisStep+1)%16;
    delay(60000/(tempo*4));

}

void serialEvent() {
  while (Serial.available()) {
    serialVal= Serial.parseInt();
    pot.set(serialVal);
    Serial.read();
    delay(5);
  } 
}


