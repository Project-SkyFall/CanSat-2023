#include "globalVars.h"

#include "myINA.h"

MyINA::MyINA(byte address):
    _address(address){
}

bool MyINA::setup(bool verbose){
    verbose ? Serial.println("---INA219 setup-----------------------------------") : 0;
    
    if(!begin(_address)){
        isWorking = IsWorking::isWorking_FALSE;
        return false;
    }
    isWorking = IsWorking::isWorking_TRUE;
    return true;
}

bool MyINA::begin(uint8_t address, TwoWire *theWire) {
  if (!i2c_dev) {
    i2c_dev = new Adafruit_I2CDevice(address, theWire);
  }

  if (!i2c_dev->begin()) {
    return false;
  }
  setCalibration_32V_2A();
  return true;
}

void MyINA::getData(){

    status = Status::status_NACK;

    if(mode == Mode::mode_SLEEP){
        status = Status::status_SLEEP;
        return;
    }

    if(!wireCheck(_address)){
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

    current = getCurrent_mA();
    power = getPower_mW();
    voltage = getBusVoltage_V();

    status = Status::status_OK;
}

void MyINA::printData(){
    Serial.print("INA219: ");
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
    Serial.print(current); Serial.print(" mA - ");
    Serial.print(voltage); Serial.print(" V - ");
    Serial.print(power); Serial.println(" mW");
}