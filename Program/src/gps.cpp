/*#include "gps.h"
#include "globalVars.h"

unsigned long lastTime;

bool MyGPS::setup(){

    Serial.println("---GPS setup---");

    if(!gps.begin()) return false;

    gps.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
    gps.saveConfigSelective(VAL_CFG_SUBSEC_IOPORT); //Save (only) the communications port settings to flash and BBR

    return true;
}

void MyGPS::getData(){
    if (millis() - lastTime > 1000){
    lastTime = millis(); //Update the timer
    
    long latitude = gps.getLatitude();
    Serial.print(F("Lat: "));
    Serial.print(latitude);

    long longitude = gps.getLongitude();
    Serial.print(F(" Long: "));
    Serial.print(longitude);
    Serial.print(F(" (degrees * 10^-7)"));

    long altitude = gps.getAltitude();
    Serial.print(F(" Alt: "));
    Serial.print(altitude);
    Serial.print(F(" (mm)"));

    byte SIV = gps.getSIV();
    Serial.print(F(" SIV: "));
    Serial.print(SIV);

    Serial.println();
  }
}*/