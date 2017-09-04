#ifndef ViviLib_h
#define ViviLib_h
/*
  ViviLib.h - Library for viviparts to communicate with Android by serial interface.
  Created by Siddharth Pillai, January 01, 2017.
  Released into the Company domain.
*/
// the #include statment and code go here...
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "EEPROM.h"


class ViviLib {
public:
    ViviLib(char branchfw, char *sensorID, bool hasMac = 1);
    void init();
    bool ViviLib::dataAvailable();
    void ViviLib::sendData(char *data, int len);
    int ViviLib::recvData(unsigned char *data);
    bool ViviLib::connected();
private:
    void sendID();
};
#endif
