#include "globalVars.h"

MyINA::MyINA(byte address){
    _address = address;
}

bool MyINA::setup(bool verbose){
    verbose ? Serial.println("--INA219 setup-") : 0;
    if(!begin()){
        status = FAIL;
        return false;
    }
    status = OK;
    return true;
}

void MyINA::getData(){
    if(status == SLEEP) return;

    if(!wireCheck(_address)){
        status = FAIL;
        return;
    }

    if(status == FAIL){
        if(!setup()) return;
    }

    current = getCurrent_mA();
    power = getPower_mW();
}

void MyINA::printData(){
    Serial.print("INA219: ");
    if(status == SLEEP){
        Serial.println("SLEEPING");
        return;
    }
    else if(status == FAIL){
        Serial.println("FAILED");
        return;
    }
    Serial.print(current); Serial.print(" mA - ");
    Serial.print(power); Serial.println(" mW");
}