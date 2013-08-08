/*
 GSMController.cpp - Library for controll Siemens TC35 GSM Module.
 Created by Juscelino Tanaka, September 12, 2012.
 All Rights Reserved to J.Tanaka ® juscelinotanaka.com.br
 */


/*
 Throughout the document, the GSM engines are referred to as ME (Mobile Equipment), 
 MS (Mobile Station), TA (Terminal Adapter), DCE (Data Communication Equipment) or 
 facsimile DCE (FAX modem, FAX board). When the Siemens product names are required 
 to distinguish the two models, TC3x is short for the engine type and TC35T for the terminal.
 
 To control your GSM engine you can simply send AT Commands via its serial interface. 
 The controlling device at the other end of the serial line is referred to as 
 TE (Terminal Equipment), DTE (Data Terminal Equipment) or plainly 'the application' 
 (probably running on an embedded system).
 */

#include "Arduino.h"
#include "GSMController.h"
#include "Message.h"



boolean ft = true;

//variavel auxiliar para o blink
byte blinkStep = 0;

GSMController::GSMController (int statusLed, int startButton, boolean  SMB) {
  _Serial = NULL;
  _SMB = SMB;
  statusSystem = false;
}

int GSMController::init (boolean  SMB) {
  _Serial = NULL;
  _SMB = SMB;
  statusSystem = false;
}

void GSMController::begin(HardwareSerial *serIn)
{
  _Serial = serIn;
  _Serial->begin(9600);
  statusSystem = GSM_CONTROLLER_OK;
}

char GSMController::listener() {
  if(_Serial->available() > 0) {
    char inchar = _Serial->read();
    return inchar;
  }
  return 0;
}

boolean GSMController::isReceivingCall(String rcv) {
  return (rcv.indexOf("+CLIP:") != -1);
}

char GSMController::available() {
  return _Serial->available();
}

int GSMController::read() {
  return _Serial->read();
}

//Confirm to TE SMS Reception's
void GSMController::confirmSMSReception() {
  _Serial->print("AT+CNMA\r");
}

//Get Signal Strength
float GSMController::verifySignalStrength() {
  _Serial->print("AT+CSQ\r"); //Signal quality
}


void GSMController::requestMessageFromStorage() {
  _Serial->print("AT+CMGL=\"ALL\"\r");
}

void GSMController::activeLineIdentification() { //Calling line identification presentation
  _Serial->print("AT+CLIP=1\r");
}

void GSMController::saveConfigProfile() { //Store current configuration to user defined profile
  _Serial->print("AT&W\r");
}

void GSMController::displayCurrentConfig() { // Display current configuration
  _Serial->print("AT&V\r");
}

void GSMController::deleteMessage(int idMessage) { //delete message from ID message storaged
  _Serial->print("AT+CMGD=");
  _Serial->print(idMessage);
  _Serial->print("\r");
}

void GSMController::sendSMSMessage(String number, String message){ //Send SMS Message
  _Serial->print("AT+CMGS=\"");
  _Serial->print(number);
  _Serial->print("\"\r"); //AT command to send SMS
  delay(100);
  _Serial->print(message); //Print the message
  delay(10);
  _Serial->print("\x1A"); //Send it ascii SUB
}


void GSMController::cellMonitoring() {
  if (_SMB) {
    _Serial->print("AT+CCED=0\r");
  } else {
    _Serial->print("AT^SMONC\r");
  }
}


void GSMController::verifyPhoneFunctionality() {
  _Serial->print("AT+CFUN?\r");
}

void GSMController::setPhoneFunctionality(int mode) {
  _Serial->print("AT+CFUN=");
  _Serial->print(mode);
  _Serial->print("\r");
}

void GSMController::setFormatMessage(int mode) {
  _Serial->print("AT+CMGF="); // set SMS mode to text
  _Serial->print(mode);
  _Serial->print("\r");
}


void GSMController::setResulteCodeCME(int mode) {
  _Serial->print("AT+CMEE="); // set SMS mode to text
  _Serial->print(mode);
  _Serial->print("\r");
}

void GSMController::setDisplayableMessage() {
  if (_SMB) {
    _Serial->println("AT+CNMI=3,3,1,1"); // set module to send SMS data to serial out upon receipt
  } else {
    _Serial->println("AT+CNMI=2,2,0,0,1"); // set module to send SMS data to serial out upon receipt
  }
}

void GSMController::enableSMSStatusReport() {
  if (_SMB) {
    _Serial->print("AT+CSMP=39, 143, 0, 0\r"); //enable SMS status report
  }
}

void GSMController::setDisplayAvailable() {
  if (!_SMB) {
    _Serial->print("AT^SSDA=1\r");// set the gsm module to think it has a screen(needed to auto send sms to terminal)
  }
}

void GSMController::setSuportedMessageService() {
  _Serial->print("AT+CSMS=1\r"); // set the gsm module to Phase 2+ version. needed to allow all modes in CNMI
}

void GSMController::requestPINStatus() {
  _Serial->print("AT+CPIN?\r");
}

void GSMController::verifyMessageMemoryUsage() {
  _Serial->print("AT+CPMS?\r");
}

boolean GSMController::isSMB() {
 return _SMB; 
}
