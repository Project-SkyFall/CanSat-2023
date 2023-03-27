#include "globalVars.h"

#include "gps.h"

MyGPS::MyGPS(byte address):
  _address(address)
  {}

bool MyGPS::setup(bool verbose){
  verbose ? Serial.println("---GPS setup--------------------------------------") : 0;

  if(!begin(Wire1, _address)){
    isWorking = IsWorking::isWorking_FALSE;
    return false;
  }

  setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
  saveConfigSelective(VAL_CFG_SUBSEC_IOPORT); //Save (only) the communications port settings to flash and BBR

  isWorking = IsWorking::isWorking_TRUE;
  return true;
}

void MyGPS::getData(){
  if(mode == Mode::mode_SLEEP){
    status = Status::status_SLEEP;
    return;
  }

  if(!wireCheck(_address, Wire1)){
    isWorking = IsWorking::isWorking_FALSE;
    status = Status::status_FAIL;
    return;
  }

  if(isWorking == IsWorking::isWorking_FALSE){
    if(!setup()){
      status = Status::status_FAIL;
      return;
    }
  }
  
  latitude = getLatitude()*10^-7;
  longitude = getLongitude()*10^-7;
  altitude = getAltitude()*10^3;
  siv = getSIV();

  status = Status::status_OK;
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
  else if(status == Status::status_NACK){
    Serial.println("NACK");
    return;
  }

  Serial.print(latitude); Serial.print((" N - "));
  Serial.print(longitude); Serial.print(" E - ");
  Serial.print(altitude); Serial.print(" mm - ");
  Serial.print(siv); Serial.println(" SIV");
}