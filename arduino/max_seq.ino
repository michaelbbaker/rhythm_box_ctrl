

#define INPUT_SIZE 20
#define MAX_STEPS 32
#define DRUMS 5

/* A4=bass drum
 * A3=big tom
 * A2=small tom
 * A1=closed hat
 * A0=open hat
 */
bool verb = false; //verbose mode
bool sync = true;  //Serial.write 't' on first step (keep max in sync)
bool go = true;   //advance through steps

unsigned long nextTrig=5000;
int steps = 16;
int trigTime = 10;
int defEnv = 0;                      //default env value
int defPit = 0;                      //default pit value
int trigPins[] = {A0, A1, A2, A3, A4}; //digital pin values for drum triggers
//int envPins[] = {};                 //digital pin values for drum envelopes
//int pitchPins[] = {};               //digital pin values for drum pitches

int tempo = 120;
int waitTime;
int thisStep = 0;
int seq[MAX_STEPS][DRUMS];     
int pit[MAX_STEPS][DRUMS];
int env[MAX_STEPS][DRUMS];

//--------------------------------SETUP()-----------------------

void setup() {

waitTime = 60000/(tempo * 4);
defSeq();
  
  Serial.begin(115200);
  pinMode(trigPins[0], OUTPUT);
  pinMode(trigPins[1], OUTPUT);
  pinMode(trigPins[2], OUTPUT);
  pinMode(trigPins[3], OUTPUT);
  pinMode(trigPins[4], OUTPUT);

  if(verb){
    Serial.println("END OF SETUP()");
  }
}


//--------------------------------LOOP()-----------------------

void loop() {
  if(go){
    //setEnv();     //do these first, allow time for adjustments on chips
    //setPit(); 
    wait(nextTrig);
    trig();
    doStep();
  }
  readSerial();
}

//--------------------------------SERIAL STUFF-----------------------
  /*
   * INCOMING MESSAGES: 
   * p=set pit (p:step:drum:val)
   * e=set env (e:step:drum:val)
   * s=set seq (s:step:drum:val)
   * t=set tempo  (t:tempo)
   * g=go [1=play, 0=pause] (g:1)
   * h=pin high[trig time ms] (h:10)
   * x=trig drum 0 to drums-1 (x:0)
   * z=clear pattern
   * q=save pattern to eeprom
   * r=restore from eeprom, send to max too.


   * OUTGOING MESSAGES:
   * b=bang on triggering first step
   */



void readSerial() {        
  while (Serial.available() > 0) {
    char input[INPUT_SIZE + 1];
    for (int i =0; i<INPUT_SIZE + 1; i++){
      input[i]=0;
    }
    
    Serial.readBytesUntil('\n', input, INPUT_SIZE);
    char* command = strtok(input, ":"); //separate the message char
    char cmd = *command;
    command = strtok(NULL, ":"); //advance to next :
    int i =0;
    int msg[]={0,0,0,0};
    while(command!=0){
      msg[i]=atoi(command);
      i++;
      command = strtok(NULL, ":"); //advance to next :
      }
     routeMessage(cmd, msg);
  }
}
    

void routeMessage(char cmd, int msg[]){
  
  if(cmd == 'p'){
    pit[msg[0]][msg[1]]=msg[2];
  }
    
   else if(cmd == 'e'){
    env[msg[0]][msg[1]]=msg[2];
    }
    else if(cmd == 's'){
    seq[msg[0]][msg[1]]=msg[2];
    }
    else if(cmd == 't'){
      tempo=msg[0];
      waitTime = 60000/(tempo * 4);
      if(verb){
        Serial.print("waitTime: ");
        Serial.print(waitTime);
        Serial.print(" ");
      }
    }
    else if(cmd == 'h'){
    trigTime=msg[0];
      if(verb){
        Serial.print("trigTime: ");
        Serial.print(trigTime);
        Serial.print(" ");
      }    
    }
    else if(cmd == 'g'){
    go=msg[0];
      if(verb){
        Serial.print("go: ");
        Serial.print(go);
        Serial.print(" ");
      }    
    }
    else if(cmd == 'x'){
    int toPlay=msg[0];
    play(toPlay);
      if(verb){
        Serial.print("play: ");
        Serial.print(toPlay);
        Serial.print(" ");
      }    
    }    
    else if(cmd == 'z'){
    defSeq();
      if(verb){
        Serial.print("reset ");
      }    
    }
    else{
      if(verb){
      Serial.print(" unrecognized routing code ");
      }
    }
}


//--------------------------------FUNCTIONS-----------------------


void wait(unsigned long aTrig){
  unsigned long thisTrig = aTrig;
  nextTrig = thisTrig + waitTime;
  unsigned long now=millis();
  while(now<thisTrig){
    now=millis();
  }
}


void trig() {
  for(int i=0; i< DRUMS; i++){
    if(seq[thisStep][i] == 1){
      digitalWrite(trigPins[i], HIGH);
      }     
    }
  delay(trigTime);
  for (int i = 0; i < DRUMS; i++) {
    digitalWrite(trigPins[i], LOW);
  }
  // send sync message to Max right after triggering
  if(sync && thisStep == 0){ Serial.println('b');} 
}


void doStep() {
  thisStep = (thisStep + 1) % steps;
}


void play(int toPlay){
  digitalWrite(trigPins[toPlay], HIGH);
  delay(trigTime);
  digitalWrite(trigPins[toPlay], LOW);
}


void defSeq(){
  for (int i = 0; i < MAX_STEPS; i++) {
    for (int j = 0; j < DRUMS; j++) {
        seq[i][j] = 0;
        pit[i][j] = defPit;
        env[i][j] = defEnv;
      }
    }
}


