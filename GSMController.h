/*
 GSMController.h - Library for controll Siemens TC35 GSM Module.
 Created by Juscelino Tanaka, September 12, 2012.
 All Rights Reserved to J.Tanaka ® juscelinotanaka.com.br
 */

#ifndef GSMController_h
#define GSMController_h

#include "Arduino.h"

#define GSM_BLINK_SLOW 0
#define GSM_BLINK_FAST 1
#define GSM_BLINK_SLOWDASHED 2
#define GSM_BLINK_FASTDASHED 3

#define GSM_BLINK_SLOW_DUR 500
#define GSM_BLINK_FAST_DUR 200
#define GSM_BLINK_SLOW_DUR 500
#define GSM_BLINK_SLOWDASHED_STEP 2
#define GSM_BLINK_FASTDASHED_STEP 10

#define GSM_CONTROLLER_ERROR 0
#define GSM_CONTROLLER_OK 1 //GSM Connected
#define GSM_CONTROLLER_LOW_SIGNAL 2
#define GSM_CONTROLLER_NO_CARRIER 3
#define GSM_CONTROLLER_BUSY 4
#define GSM_CONTROLLER_RING 5
#define GSM_CONTROLLER_CONNECT 6
#define GSM_CONTROLLER_ALERTING 7
#define GSM_CONTROLLER_DIALING 8

class GSMController {
  public:
    GSMController(int statusLed = 13, int startButton = 3, boolean  SMB = false);
    //Some Default Serial Functions
    void begin(HardwareSerial *serIn);
    char available();
    int read();
    int init(boolean  SMB = false);
    
    //essential functions
    char listener();
    
    //Functions to Manipulate GSM Module
    float verifySignalStrength(); // Get the Signal Strength 
    
    void activeLineIdentification(); //Calling line identification presentation
    void saveConfigProfile(); //Store current configuration to user defined profile
    void displayCurrentConfig(); // Display current configuration
    
    /* Message Functions */
    void confirmSMSReception(); // Confirm Message Reception
    void requestMessageFromStorage(); //get a list of storaged messages on prefered storage
    void deleteMessage(int idMessage); //delete message from ID message storaged
    void sendSMSMessage(String number, String message); //Send SMS Message
    void setFormatMessage(int mode = 1); //set format of short messages 0-PDU / 1-TEXT
    void setDisplayableMessage(); //control the way of indicating DTE, after receiving MT messages
    void enableSMSStatusReport(); //enable SMS status report - SMB Only
    void setDisplayAvailable(); //Set Display Availability - TC35 Only
    void setSuportedMessageService();//set the classes of supported short message service
    void verifyMessageMemoryUsage(); 
    
    void setResulteCodeCME(int mode = 1); //disable or enable the use of result code +CME ERROR * 0-Deactivate 1-Code/Number 2-Code/Verbose
    void cellMonitoring();
    void startSystem();
    
    void requestPINStatus(); //request PIN Status
    
    void verifyPhoneFunctionality(); //Verify phone functionality
    void setPhoneFunctionality(int mode = 1); //Set phone functionality
      
    //Status Variables
    boolean statusSystem;
    
    boolean isReceivingCall(String rcv);
    
    boolean isSMB();
  
  
  
  private:
    HardwareSerial * _Serial;
    boolean _SMB;
    
    void setStatusLed(boolean value);
    //void ledBlink(int blinkDashedType);
    //int getLedSystemStatus();
};

#endif




