#include "globalVars.h"

#include "myINA.h"

MyINA::MyINA(byte address){
    _address = address;
}

bool MyINA::setup(bool verbose){
    verbose ? Serial.println("---INA219 setup-----------------------------------") : 0;
    if(!begin()){
        status = Status::status_FAIL;
        return false;
    }
    status = Status::status_OK;
    return true;
}

void MyINA::getData(){
    if(status == Status::status_SLEEP) return;

    if(!wireCheck(_address)){
        status = Status::status_FAIL;
        return;
    }

    if(status == Status::status_FAIL){
        if(!setup()) return;
    }

    current = getCurrent_mA();
    power = getPower_mW();
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
    Serial.print(current); Serial.print(" mA - ");
    Serial.print(power); Serial.println(" mW");
}