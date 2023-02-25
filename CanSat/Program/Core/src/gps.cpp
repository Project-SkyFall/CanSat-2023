#include "globalVars.h"

#include "gps.h"

bool MyGPS::setup(bool verbose){

    verbose ? Serial.println("---GPS setup--------------------------------------") : 0;

    if(!begin()) return false;

    setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
    saveConfigSelective(VAL_CFG_SUBSEC_IOPORT); //Save (only) the communications port settings to flash and BBR

    return true;
}

void MyGPS::getData(){

  if(status == Status::status_SLEEP) return;

  if(wireCheck(address) != 0){
      status = Status::status_FAIL;
      return;
  }

  if(status == Status::status_FAIL){
      setup();
      return;
  }
  
  latitude = getLatitude();
  longitude = getLongitude();
  altitude = getAltitude();
  siv = getSIV();
  return;
}

void MyGPS::printData(){
  Serial.print("GPS: ");
  if(status == Status::status_SLEEP){
    Serial.println("SLEEPING");
      return;
  }
  else if(status == Status::status_FAIL){
    Serial.println("FAILED");
    return;
  }

  Serial.print(latitude); Serial.print((" N - "));
  Serial.print(longitude); Serial.print(" E - ");
  Serial.print(altitude); Serial.print(" mm - ");
  Serial.print(siv); Serial.println(" SIV");
}