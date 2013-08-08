/*
 GSMController.h - Library for controll events from SARMen System.
 Created by Juscelino Tanaka, September 12, 2012.
 All Rights Reserved to J.Tanaka ® juscelinotanaka.com.br
 */

#ifndef EventController_h
#define EventController_h

#include "Arduino.h"
#include "GSMController.h"



class EventController {
  public:
    EventController(int systemLed = 12, int releSystem = 7);
    int registerGSMController(HardwareSerial *GSMController, boolean  SMB = true);
    
    char listener();
    
    void ledBlink(int blinkDashedType);
    void setStatusLed(boolean value);
    
    //Some Default Serial Functions
    
    int verifySIND(String msg);
    boolean GSMAvailable();
    
    void callFunction(char gsm_char);
    
    boolean setupGSM();
    
  private:
    int _systemLed;
    int _systemStatus;
    int _releSystem;
    int _statusLed;
    
    
    boolean _SIMInserted; //sind 0 e 1
    boolean _partialReady; //sind 3
    boolean _fullReady; // sind 4
    boolean _emergencyReady; //sind 7
    boolean _networkRegistered; //sind 11 e 8
    boolean _setupCompleted; //indicate initialization of settings
    
    GSMController _gsm;
};

#endif
