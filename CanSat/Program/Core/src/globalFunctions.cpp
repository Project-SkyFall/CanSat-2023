#include "globalVars.h"

#include "myTime.h"
#include "temperature.h"
#include "gps.h"
#include "myLora.h"
#include "myINA.h"
#include "mySD.h"
#include "myServer.h"
#include "myOxygen.h"
#include "myNeo.h"
#include "myINA.h"
#include "myCO2.h"
#include "myBNO.h"
#include "myBH1730.h"
#include "mySpectro.h"
#include "pot.h"
#include "gpioExtender.h"
#include "camera.h"

bool wireCheck(byte address, TwoWire &theWire){
    theWire.beginTransmission(address);
    byte val = theWire.endTransmission();
    return (val == 0 ? true : false);
}

void printResult(bool input){
    input ? Serial.println("===OK=============================================\n") : Serial.println("===FAIL===========================================\n");
}

String verbose_print_reset_reason(RESET_REASON reason){
    switch(reason){
        case 1  : return "Vbat power on reset"; break;
        case 3  : return "Software reset digital core"; break;
        case 4  : return "Legacy watch dog reset digital core"; break;
        case 5  : return "Deep Sleep reset digital core"; break;
        case 6  : return "Reset by SLC module, reset digital core"; break;
        case 7  : return "Timer Group0 Watch dog reset digital core"; break;
        case 8  : return "Timer Group1 Watch dog reset digital core"; break;
        case 9  : return "RTC Watch dog Reset digital core"; break;
        case 10 : return "Instrusion tested to reset CPU"; break;
        case 11 : return "Time Group reset CPU"; break;
        case 12 : return "Software reset CPU";  break;
        case 13 : return "RTC Watch dog Reset CPU"; break;
        case 14 : return "for APP CPU, reseted by PRO CPU"; break;
        case 15 : return "Reset when the vdd voltage is not stable"; break;
        case 16 : return "RTC Watch dog reset digital core and rtc module"; break;
        default : return "NO_MEAN";
    }
    return "NO_MEAN";
}

String sensorStatuses(){
    uint32_t statusByte = 0;

    statusByte += static_cast<uint8_t>(bh.status)       << 0;
    statusByte += static_cast<uint8_t>(bme.status)      << 2;
    statusByte += static_cast<uint8_t>(scd.status)      << 4;
    statusByte += static_cast<uint8_t>(bno.status)      << 6;
    statusByte += static_cast<uint8_t>(oxygen.status)   << 8;
    statusByte += static_cast<uint8_t>(asx.status)      << 10;
    statusByte += static_cast<uint8_t>(sd.status)       << 12;
    statusByte += static_cast<uint8_t>(gps.status)      << 14;
    statusByte += static_cast<uint8_t>(ina.status)      << 16;
    statusByte += static_cast<uint8_t>(neo.status)      << 18;
    statusByte += static_cast<uint8_t>(ds18.status)     << 20;
    statusByte += static_cast<uint8_t>(cam.status)      << 22;
    
    return String(statusByte, HEX);
}