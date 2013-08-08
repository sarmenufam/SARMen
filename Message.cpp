/*
  Message.cpp - Library for access Messages from GSM Module.
 Created by Juscelino Tanaka, September 12, 2012.
 All Rights Reserved to J.Tanaka ® juscelinotanaka.com.br
 */

#include "Arduino.h"
#include "Message.h"

Message::Message() {

}

//Verifica a existencia de CMT no vetor, o que conclui ser uma mensagem
int Message::checkMessage(String msg) {
  int startNumb, endNumb, startDate, endDate, startTime, endTime, startMsg = 0;
  int i=0, j=0, aspa = 0;

  if (msg.indexOf("+CMT:") != -1) {
    for (j = 0; j < msg.length() - 5; j++) {

      if (msg[j] == '"') { //procura a primeira as do texto, onde iniciara o
        switch (aspa) {    //numero enviado e os outros dados
        case 0 : 
          startNumb = j + 1; 
          aspa++;
          break;
        case 1 : 
          endNumb = j; 
          aspa++;
          break;
        case 2 : 
          startDate = j + 1; 
          endDate = j + 9;
          startTime = j + 10;
          endTime = j + 18;
          aspa = 0;
          break;
        } 

        if (aspa == 0) {  //sera 0 apenas quando mudar do 3 para o 0
          break;          //indicando que achou todos os intervalos
        }
      } 

    }

    for ( j = j ; j < msg.length() ; j++) {
      if (msg[j] == '\r') { // procura pelo carro final da linha de infos
        if (startMsg == 0) { //default mode da variavel
          startMsg = j + 2;
          break;
        } 
      } 
    }

    _number = msg.substring(startNumb, endNumb);
    _date = msg.substring(startDate, endDate);
    _time = msg.substring(startTime, endTime);
    _message = msg.substring(startMsg);

    return 1;
  }

  return 0;
}

String Message::getNumber() {
  return _number;
}

boolean Message::isNumber(String number) {
  return (_number.indexOf(number) != -1);
}

void Message::printMessage() {
  Serial.print("N: ");
  Serial.println(_number);
  Serial.print("D: ");
  Serial.println(_date);
  Serial.print("T: ");
  Serial.println(_time);
  Serial.print("M: ");
  Serial.println(_message);
}
