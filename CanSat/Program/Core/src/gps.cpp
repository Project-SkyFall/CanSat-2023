#include "gps.h"
#include "myTime.h"

MyGPS::MyGPS(byte address):
  _address(address){
}

bool MyGPS::setup(bool verbose){
  verbose ? Serial.println("---GPS setup--------------------------------------") : 0;

  if(!begin(Wire1, _address)){
    isWorking = IsWorking::isWorking_FALSE;
    return false;
  }

  if(verbose){
    setTimePulse() ? Serial.println("Set timepulse OK") : Serial.println("Set timepulse FAIL");
  }
  
  setNavigationFrequency(2);
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

  //uint32_t getTimeOfWeek(uint16_t maxWait = defaultMaxWait);
  if(getConfirmedDate() && getConfirmedTime()){
    year = getYear();
    month = getMonth();
    day = getDay();
    hour = getHour();
    minute = getMinute();
    second = getSecond();

    rtc.setDate(day, month, year);
    rtc.setTime(hour, minute, second);
  }
  
  if(getGnssFixOk()){
    latitude = getLatitude()*(pow(10, -7));
    longitude = getLongitude()*(pow(10, -7));
    altitude = getAltitudeMSL()*(pow(10, -3));
    siv = getSIV();
  }

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

  Serial.print(latitude, 4); Serial.print((" N - "));
  Serial.print(longitude, 4); Serial.print(" E - ");
  Serial.print(altitude); Serial.print(" m - ");
  Serial.print(day); Serial.print("."); Serial.print(month); Serial.print("."); Serial.print(year); Serial.print(" ");
  Serial.print(hour); Serial.print(":"); Serial.print(minute); Serial.print(":"); Serial.print(second); Serial.print(" - ");
  Serial.print(siv); Serial.println(" SIV");
}

bool MyGPS::setTimePulse(){
  timePulse.tpIdx = 0;

  timePulse.freqPeriod = 0;
  timePulse.pulseLenRatio = 0;

  timePulse.freqPeriodLock = 1000000;
  timePulse.pulseLenRatioLock = 1000000;

  timePulse.flags.bits.active = 1;
  timePulse.flags.bits.lockedOtherSet = 1;
  timePulse.flags.bits.isFreq = 0;
  timePulse.flags.bits.isLength = 1;
  timePulse.flags.bits.polarity = 1;
  if(!setTimePulseParameters(&timePulse)) return false;
  if(!saveConfigSelective(VAL_CFG_SUBSEC_NAVCONF)) return false; // Finally, save the time pulse parameters in battery-backed memory so the pulse will automatically restart at power on
  return true;
}