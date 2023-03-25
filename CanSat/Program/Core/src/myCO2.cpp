#include "globalVars.h"

#include "myCO2.h"

MyCO2::MyCO2(uint8_t address):
    _address(address){
}

byte MyCO2::setup(bool verbose){
    verbose ? Serial.println("---SCD41 setup------------------------------------") : 0;

    begin(Wire);

    if(stopPeriodicMeasurement() != 0){
        isWorking = IsWorking::isWorking_FALSE;
        return false;
    }

    if(!startPeriodicMeasurement() != 0){
        isWorking = IsWorking::isWorking_FALSE;
        return false;
    }

    isWorking = IsWorking::isWorking_TRUE;
    return true;
}

void MyCO2::getData(){
    status = Status::status_NACK;

    getDataReadyFlag(_isDataReady);
    if(!_isDataReady) return;

    readMeasurement(co2, temperature, humidity);
    status = Status::status_OK;
}

void MyCO2::printData(){
    Serial.print("SCD41: ");
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
    Serial.print(co2); Serial.print(" ppm - ");
    Serial.print(temperature); Serial.print(" °C - ");
    Serial.print(humidity); Serial.println(" %");
}