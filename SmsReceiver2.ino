
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Message.h"
#include "EventController.h"



boolean startSystem = false; 
boolean waitingValue = false;

int releSystem = 7;
int startButton = 6;
int systemLed = 12;

boolean SMB = true; //define if its using a SMB GSM Module


char controlCharBuff;

//GSMController gsm(systemLed, startButton, SMB); //set status led to 5 (default is 13)

EventController controller;

void setup()
{ 
  
  Serial.begin(115200);  //setup Local Serial
  
  Serial.println("SARMen: Start");
  controller.registerGSMController(&Serial1, SMB);
}

Message aMess;

void loop(){


  if (controlCharBuff = controller.listener()) { //listen the GSM Module
    Serial.print(controlCharBuff);
  }

  if(Serial.available() > 0) {  //caso algum dado seja encontrado na serial LOCAL
    char buffSerie = Serial.read(); //ler o dado recebido
    controller.callFunction(buffSerie);
  } //fim-se Serial.avail... (entrada de dados por serial)
  
}


