/*
  Message.cpp - Library for access Messages from GSM Module.
 Created by Juscelino Tanaka, September 12, 2012.
 All Rights Reserved to J.Tanaka ® juscelinotanaka.com.br
 */

#ifndef Message_h
#define Message_h

#include "Arduino.h"

class Message {
public:
  Message();
  int checkMessage(String msg);
  String getNumber();
  boolean isNumber(String number);
  void printMessage();
  //String _isValid();
private:
  String _number;
  String _date;
  String _time;
  String _message;
};

#endif

