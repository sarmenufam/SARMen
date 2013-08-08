/*
 GSMController.cpp - Library for controll events from SARMen System.
 Created by Juscelino Tanaka, September 12, 2012.
 All Rights Reserved to J.Tanaka ® juscelinotanaka.com.br
 */

#include "Arduino.h"
#include "EventController.h"
#include "GSMController.h"
#include "Message.h"

const unsigned int buffsize = 500;

unsigned long blinkLed; //pega o tempo da ultima execucao do blink

unsigned long time, next = 500, nextExec = 0; //variaveis de tempo
char rcv[buffsize];  //variavel de armazenamento do texto
int cnt_rcv=0;      //contador de indice da variavel de recebimento

char charBuff;

//temporary message to use with GSM Controller
Message tempMessage;


/* _systemStatus Available

0  = nothing
1  = no sim inserted
11 = gsmStarted
12 = gsmReady

*/

EventController::EventController(int systemLed, int releSystem) {
  _systemStatus = 0;
  _systemLed = systemLed;
  _releSystem = releSystem;
  
  pinMode(_systemLed, OUTPUT);
  pinMode(_releSystem, OUTPUT);
}

int EventController::registerGSMController(HardwareSerial *GSMController, boolean  SMB) {
  _gsm.init(SMB); //set status led to 5 (default is 13)
  _systemStatus = 1;
  
  _gsm.begin(GSMController);
  
}


boolean EventController::setupGSM() {
  //variaveis de controle de start
  unsigned long time, startTime = 0;
  
  byte startStep = 0;
  boolean dk = true, qk = true, sk = true, fk = true;
  
  time = millis();
  startTime = time + 3000; //30 secs
  
  Serial.print(time);
  Serial.print(" < - > ");
  Serial.println(startTime + 10000L);
  
  while (time = millis() < startTime + 10000) {
    if (_gsm.available()) {
      Serial.print((char)_gsm.read());
    }
    
    if (time > startTime && fk) {
      startStep = 1;
      fk = false;
      Serial.print("SS1");
    } 
    if (time > startTime + 2000 && dk) {
      startStep = 2;
      dk = false;
      Serial.print("SS2");
    } 
    else if (time > startTime + 4000 && qk) {
      startStep = 3;
      qk = false;
      Serial.print("SS3");
    } 
    else if (time > startTime + 6000 && sk) {
      startStep = 4;
      sk = false;
      Serial.print("SS4");
    }

    switch (startStep) {
    case 1 :
      if(_gsm.isSMB()) {
        _gsm.enableSMSStatusReport();
      } else {
        _gsm.setFormatMessage(1);
      }
      Serial.println("s1");
      startStep = 0;
      break;
    case 2 :
      if(_gsm.isSMB()) {
        _gsm.setFormatMessage(1);
      } else {
        _gsm.setDisplayAvailable();
      }
      Serial.println("s2");
      startStep = 0;
      break;
    case 3 :
      _gsm.setSuportedMessageService();
      startStep = 0;
      break;
    case 4 :
      _gsm.setDisplayableMessage();
      startStep = 0;
      break;
    default :
      break;
    }
  }
  _setupCompleted = GSM_CONTROLLER_OK;
}



int EventController::verifySIND(String msg) {
  int pos;
  String txt = "";
  
  pos = msg.indexOf("+SIND:");
  if (pos != -1) {
    pos += 7;
    while (msg[pos] != ',' && msg[pos] != '\0' && msg[pos] != 13 ) {
      txt += msg[pos];
      pos++;
    }
    
    if (txt.equals("0")) {
      _SIMInserted = 0;
      _systemStatus = 1;
    } else if (txt.equals("1")) {
      _SIMInserted = 1;
      _systemStatus = 2;
    } else if (txt.equals("2")) {

    } else if (txt.equals("3")) {
      _partialReady = 1;
      _systemStatus = 3;
    } else if (txt.equals("4")) {
      _fullReady = 1;
      _systemStatus = 3;
    } else if (txt.equals("5")) {
      
    } else if (txt.equals("6")) {

    } else if (txt.equals("7")) {
      _emergencyReady = 1;
      _systemStatus = 7;
    } else if (txt.equals("8")) {
      _networkRegistered = 0;
      _systemStatus = 8;
    } else if (txt.equals("11")) {
      _networkRegistered = 1;
      _systemStatus = 8;
    } 
    
    return 1;
  }
  return 0;
}


char EventController::listener() {
  time = millis(); // catch time of the system
  
  //ledBlink(getLedSystemStatus());
  
  //verifica se o gsm esta pronto para ser escutado
  if (_gsm.statusSystem == GSM_CONTROLLER_OK) {
    if (charBuff = _gsm.listener()) { //listen the GSM Module
      nextExec = next + time;
      rcv[cnt_rcv] = charBuff;
      cnt_rcv++;
      if (cnt_rcv == buffsize) cnt_rcv = 0;  
      
      rcv[cnt_rcv] = '\0';
      return charBuff;
    }
  }
  
  
  //verifica os dados recebidos e concatenas em uma string apos determinado tempo
  if (nextExec != 0 && time > nextExec) {
    nextExec = 0;
    
    if (verifySIND(rcv)) {
      if (GSMAvailable()) {
        Serial.println("StartSetup");
        
        _gsm.enableSMSStatusReport();
        delay(100);
        _gsm.setFormatMessage(1);
        delay(100);
        _gsm.setSuportedMessageService();
        delay(100);
        _gsm.setDisplayableMessage();
        
        Serial.println("EndSetup");
      }
    } 
    else if (tempMessage.checkMessage(rcv)) {
      if (!_gsm.isSMB()) _gsm.confirmSMSReception();
      
      tempMessage.printMessage();
      
      if (tempMessage.isNumber("91410211")) {
        digitalWrite(_releSystem, !digitalRead(_releSystem));
        String tmpMsg = "O Dispositivo foi ";
        tmpMsg += _releSystem?"Ligado":"Desligado";
        _gsm.sendSMSMessage("91410211", tmpMsg);
        
      }
    } 
    else if (_gsm.isReceivingCall(rcv)) {
      
    }
    cnt_rcv = 0;
  }
  
  return 0;
  
}

/*

int GSMController::getLedSystemStatus() {
  switch (statusSystem) {
    case GSM_CONTROLLER_OK :
      return GSM_BLINK_FASTDASHED;
      break;
    case GSM_CONTROLLER_ERROR :
      return GSM_BLINK_SLOW;
      break;
    case GSM_CONTROLLER_LOW_SIGNAL :
    case GSM_CONTROLLER_NO_CARRIER :
      return GSM_BLINK_SLOWDASHED;
      break;
    default : 
      return GSM_BLINK_SLOW;
      break;
  }
}

*/

void EventController::ledBlink(int blinkDashedType) {
  time = millis();
  switch (blinkDashedType) {
  case GSM_BLINK_SLOW : 
    if (time > blinkLed + GSM_BLINK_SLOW_DUR) {
      setStatusLed(!_statusLed);
      blinkLed = time;
    }
    break;
  case GSM_BLINK_FAST :
    if (time > blinkLed + GSM_BLINK_FAST_DUR) {
      setStatusLed(!_statusLed);
      blinkLed = time;
    } 
    break;
  case GSM_BLINK_SLOWDASHED : 
    if (_statusLed && time > blinkLed + GSM_BLINK_SLOW_DUR) { //se ligado, desliga de primeira
      setStatusLed(!_statusLed);
      blinkLed = time;
    } else if (!_statusLed && time > blinkLed + GSM_BLINK_SLOW_DUR * GSM_BLINK_SLOWDASHED_STEP) {
      setStatusLed(!_statusLed);
      blinkLed = time;
    }
    break;
  case GSM_BLINK_FASTDASHED : 
    if (_statusLed && time > blinkLed + GSM_BLINK_FAST_DUR) { //se ligado, desliga de primeira
      setStatusLed(!_statusLed);
      blinkLed = time;
    } else if (!_statusLed && time > blinkLed + GSM_BLINK_FAST_DUR * GSM_BLINK_FASTDASHED_STEP) {
      setStatusLed(!_statusLed);
      blinkLed = time;
    }
    break;
  }
}

void EventController::setStatusLed(boolean value) {
  digitalWrite(_systemLed, value);
  _statusLed = value;
}

boolean EventController::GSMAvailable() {
    return _networkRegistered && _SIMInserted && _fullReady;// && _setupCompleted;
}

void EventController::callFunction(char gsm_char) {

    if(gsm_char=='l') {
      _gsm.requestMessageFromStorage();
    }
    else if(gsm_char=='c') {
      _gsm.cellMonitoring();
    }
    else if(gsm_char=='s') {
      _gsm.verifySignalStrength();
    } 
    else if (gsm_char == 'f') {
      _gsm.verifyPhoneFunctionality();
    }
    else if (gsm_char == 'g') {
      //_gsm.cellMonitoring();
    }
    else if (gsm_char == 'h') {
      _gsm.sendSMSMessage("91410211", "Putz");
    }
    else if (gsm_char == 'v') {
      _gsm.enableSMSStatusReport();
    }
    else if (gsm_char == 'b') {
      _gsm.setFormatMessage(1);
    }
    else if (gsm_char == 'n') {
      _gsm.setDisplayableMessage();
    }
    else if (gsm_char == 'q') {
      _gsm.setResulteCodeCME(2);
    }
    else if (gsm_char == 'w') {
      _gsm.requestPINStatus();
    }
    else if (gsm_char == 'd') {
      _gsm.deleteMessage(15);
      _gsm.deleteMessage(14);
      /*
      _gsm.deleteMessage(7);
      
      _gsm.deleteMessage(8);
      _gsm.deleteMessage(9);
      _gsm.deleteMessage(10);
      _gsm.deleteMessage(11);
      _gsm.deleteMessage(12);
      _gsm.deleteMessage(13);*/
      
    }
    else if (gsm_char == 'u') {
      _gsm.verifyMessageMemoryUsage();
    }
    
}

