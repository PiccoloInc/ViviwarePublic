/*
  ViviLib.cpp - Library for viviparts to communicate with Android by serial interface.
  Created by Siddharth Pillai, January 01, 2017.
  Released into the Company domain.
*/
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "ViviLib.h"
#include "EEPROM.h"

char HEAD_ID = 0x00;
char HEAD_DATA[] = {0x01};
char HEAD_FW = 0x02;

int _dataNodes = 0;
int _swSerialRx = 10;
int _swSerialTx = 9;
uint8_t _sensorID[8];
char libdatabuffer[64];
char inDataSize;
char *_sensorIDptr;
char datatest[] = {0x01, 0x02, 0x03, 0x01, 0x02, 0x02, 0x01, 0x0a, 0x00, 0x0b};
unsigned long TIMEOUT = 200;
SoftwareSerial swSerial(_swSerialRx, _swSerialTx);
//AltSoftSerial swSerial(_swSerialRx, _swSerialTx);
bool STATUS_CONNECTED = false;
char fwver = 0x02;
// struct DataModel{
//     int dc;
//     int dsize;
//     char *ptr;
//   };
// DataModel dataModel[6];

ViviLib::ViviLib(char branchfw, char *sensorID, bool hasMac) {
    char serialTemp[3];
    serialTemp[0] = HEAD_FW;
    serialTemp[1] = fwver;
    serialTemp[2] = branchfw;
    _sensorIDptr = sensorID;
    _sensorID[0] = *_sensorIDptr;
    _sensorID[1] = *(_sensorIDptr + 1);
    for(int i=0;i<6;i++){
        if(hasMac){
          _sensorID[i+2] = EEPROM.read(E2END-(5-i));
        }else{
          _sensorID[i+2] = 0;
        }
        // Serial.print("id: ");
        // Serial.println(_sensorID[i]);
    }

    swSerial.begin(9600);
    //memcpy(serialTemp + 2, serialID, 6);
    swSerial.write(serialTemp, 3);
    //delay(100);
    // libdatabuffer[1] = 0x02;
    //   libdatabuffer[2] = 0x03;

}

void ViviLib::init() {

    while (!STATUS_CONNECTED) {
        if (swSerial.available() > 0) {
            byte acknowledge = swSerial.read();
            unsigned int x1 = (unsigned int)acknowledge;
            if (acknowledge == 0x31) {
                STATUS_CONNECTED = true;
                //Serial.write("CONNECTED \n");
            } else {
                STATUS_CONNECTED = false;
                // Serial.write("NOT CONNECTED \n");
            }
        } else {
            sendID();
            delay(100);
        }
    }
}

bool ViviLib::connected() {
    return STATUS_CONNECTED;
}

// void ViviLib::setDataPoint(int dc,int dsize, char *ptr){
//   dckey[dc] = _dataNodes;
//   dataModel[_dataNodes].dc=dc;
//   dataModel[_dataNodes].dsize=dsize;
//   dataModel[_dataNodes].ptr=ptr;
//   _dataNodes++;
// }

// void ViviLib::setDataPoint(int dc,int dsize, char ptr){
//   dckey[dc] = _dataNodes;
//   dataModel[_dataNodes].dc=dc;
//   dataModel[_dataNodes].dsize=dsize;
//   dataModel[_dataNodes].ptr=&ptr;
//   _dataNodes++;
// }

void ViviLib::sendData(char *data, int len) {
    // if (len>10){
    // memcpy(&libdatabuffer[1],data,10);
    //  memcpy(&libdatabuffer[11],data+10,len-10);}
    // else{
    // memcpy(&libdatabuffer[1],data,len);}
    int newl = len + 1;
    // libdatabuffer[0] = HEAD_DATA[0];
    char temp[20];
    temp[0] = 0x01;

    //  for(int i=0;i<=len;i++){
    // //   int j= i-1;
    //    // libdatabuffer[i] = data[j];
    //    temp[i+1] = data[i];
    //  }
//  lent = len+1;
    // int totallen = len+1;
    // char sendBuffert[] = {0x01,0x02,0x01,0x03,0x05,0x00,0x01,0x02,0x03,0x02,0x03,0x01,0x02,0x02,0x01,0x0a,0x00,0x01};
    // if(len>=10){
    //   memcpy(temp+1,data,10);
    //   swSerial.write(temp,11);
    //   delay(500);
    //   memcpy(temp+1,data+10,len-10);
    //    swSerial.write(temp,len-10);
    // }else{
    memcpy(temp + 1, data, len);
    swSerial.write(temp, newl);
    // }

    // swSerial.write(data,10);
    // delay(200);
}

int ViviLib::recvData(unsigned char *data) {
    int len = inDataSize;
 //   data =
    memcpy(data, libdatabuffer + 1, len);
//    char sendBuffert[] = {0x01,0x02,0x01,0x03,0x05,0x00,0x01,0x02,0x03};
    return len;
}

bool ViviLib::dataAvailable() {
    unsigned long timeout = millis() + TIMEOUT;
    int inIndex = 0;
    if (swSerial.available() && STATUS_CONNECTED) {
        while (((int32_t)(millis() - timeout) < 0)) {
            if (swSerial.available() > 0) {
                char temp = swSerial.read();
                libdatabuffer[inIndex + 1] = temp;
                inIndex++;
                // if(inIndex >= 24) {
                //    inDataSize = inIndex;
                //   break;
                // }
            }
        }
        inDataSize = inIndex;
        return true;
    } else {
        return false;
    }

}


void ViviLib::sendID() {
    uint8_t temp[9] ;
    temp[0] = HEAD_ID;
    memcpy(temp+1,_sensorID,8);
    for(int i=0;i<8;i++){
        // Serial.print("id: ");
        // Serial.println(_sensorID[i]);
    }
    swSerial.write(temp, 9);
}
