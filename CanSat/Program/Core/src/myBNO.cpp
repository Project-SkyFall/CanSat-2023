#include "globalVars.h"

#include "myBNO.h"

MyIMU::MyIMU(uint8_t address, uint8_t reset):
    _address(address),
    _reset(reset){
    Adafruit_BNO055(-1, _address);
}

bool MyIMU::setup(bool verbose){
    verbose ? Serial.println("---DS18B20 setup----------------------------------") : 0;

    if(!begin()){
        isWorking = IsWorking::isWorking_FALSE;
        return false;
    }
    
    return true;
}

void MyIMU::getData(){
    if(mode == Mode::mode_SLEEP) return;
}

void MyIMU::printData(){
    Serial.print("BNO055: ");
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
    Serial.print(roll); Serial.print(" ° - ");
    Serial.print(pitch); Serial.print(" ° - ");
    Serial.print(yaw); Serial.print(" ° - ");

    Serial.print(accel.x); Serial.print(" g - ");
    Serial.print(accel.y); Serial.print(" g - ");
    Serial.print(accel.z); Serial.println(" g");
}