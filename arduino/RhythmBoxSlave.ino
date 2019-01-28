
#include <SPI.h>
#include <mcp4xxx.h>

MCP4XXX pitchPin[3] = {
                      MCP4XXX(7), MCP4XXX(6), MCP4XXX(5)
                      };
MCP4XXX crvPin2(3);
MCP4XXX crvPin3(4);
#define INPUT_SIZE 20
#define DRUMS 5

/* A0=kick
 * A1=tom
 * A2=clave
 * A3=closed hat
 * A4=open hat
 */

unsigned long now;
unsigned long nextStepTime=5000;
unsigned long unTrigTime[]={-1, -1, -1, -1, -1};
int defEnv = 1;                         //default env value
int defPit = 1;                         //default pit value
int trigPins[] = {A1, A2, A3, A4, A5}; //digital pin values for drum triggers


//--------------------------------SETUP()-----------------------

void setup() {
  
  Serial.begin(115200);
  pinMode(trigPins[0], OUTPUT);
  pinMode(trigPins[1], OUTPUT);
  pinMode(trigPins[2], OUTPUT);
  pinMode(trigPins[3], OUTPUT);
  pinMode(trigPins[4], OUTPUT);

}


//--------------------------------LOOP()-----------------------

void loop() {
  unTrig();
  readSerial();
}

//--------------------------------FUNCTIONS-----------------------


void setPit(int aDrum, int aPit){
    if(aDrum<=2){
    pitchPin[aDrum].set(aPit);
    }
}

void setCrv(int aDrum, int aCrv){
    if(aDrum==2){
    crvPin2.set(aCrv);
    }
    if(aDrum==3||aDrum==4){
    crvPin3.set(aCrv);
    }
}

void trig(int aDrum, int aEnv){  
   digitalWrite(trigPins[aDrum], LOW);       //in case still high, ensure retrigger
   digitalWrite(trigPins[aDrum], HIGH);
   now=millis();
   unTrigTime[aDrum]=now + aEnv;
}

void unTrig(){
  now=millis();
  for(int i=0; i< DRUMS; i++){
      if(unTrigTime[i] != -1 && unTrigTime[i] <= now){
        digitalWrite(trigPins[i], LOW);
        unTrigTime[i] = -1;
        }
  }
}


//--------------------------------SERIAL STUFF-----------------------
  /*
   * INCOMING MESSAGES: 
   * c=set curve on drum 2 or 3 (c:0:0) arg 1= drum id arg 2=curve val
   * p=set pitch on drum 0 to 2 (p:0:0) arg 1= drum id arg 2=pitch val
   * t=trig drum 0 to 4 (t:0)
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

  if(cmd == 't'){
    int drum=msg[0];
    int env=msg[1];
    trig(drum, env);
  }
  else if(cmd == 'p'){
    int drum=msg[0];
    int pit=msg[1];
    setPit(drum, pit);
    }   
  else if(cmd == 'c'){
    int drum=msg[0];
    int crv=msg[1];
    setCrv(drum, crv);
    }           
}




