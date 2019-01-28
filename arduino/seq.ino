int drumPins[] = {A1,A2,A3,A4,A5}; //digital pin values for drums
int drumState[] = {0, 0, 0, 0, 0}; //array holding on/off drum states
int tempo = 90;
int thisSequence = 0;
int thisStep = 0;
int sequence[][5] = {
  {1, 0, 0, 0, 0},
  {0, 0, 0, 0, 1},
  {0, 1, 0, 1, 0},
  {0, 0, 0, 0, 1},
  {0, 0, 1, 0, 0},
  {0, 0, 0, 0, 1},
  {0, 1, 0, 1, 0},
  {0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0},
  {0, 0, 0, 0, 1},
  {0, 1, 0, 1, 0},
  {0, 0, 0, 0, 1},
  {0, 0, 1, 0, 0},
  {0, 0, 0, 0, 1},
  {0, 1, 0, 1, 0},
  {0, 0, 0, 0, 1}};

void setup() {
  // initialize digital pin 13 as an output.
  Serial.begin(19200);
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
    Serial.println(thisStep);
    Serial.println(60000/(tempo*4));
    delay(60000/(tempo*4));

}

