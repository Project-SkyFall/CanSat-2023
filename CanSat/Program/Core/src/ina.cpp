#include "globalVars.h"

#include "myINA.h"

MyINA::MyINA(byte address):
    _address(address),
    Adafruit_INA219(address){
}

bool MyINA::setup(bool verbose){
    verbose ? Serial.println("---INA219 setup-----------------------------------") : 0;
    
    if(!begin()){
        isWorking = IsWorking::isWorking_FALSE;
        return false;
    }
    isWorking = IsWorking::isWorking_TRUE;
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

    /*current = inaTest.getCurrent_mA();
    voltage = inaTest.getBusVoltage_V();
    power = inaTest.getPower_mW();*/

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