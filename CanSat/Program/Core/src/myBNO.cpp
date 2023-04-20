#include "globalVars.h"

#include "myBNO.h"

MyIMU::MyIMU(int id, uint8_t address, uint8_t reset):
    _address(address),
    _reset(reset),
    Adafruit_BNO055(id, address){
    //Adafruit_BNO055(-1, _address);
    //i2c_dev = new Adafruit_I2CDevice(_address);
}

bool MyIMU::setup(bool verbose){
    verbose ? Serial.println("---BNO055 setup----------------------------------") : 0;

    if(!begin()){
        isWorking = IsWorking::isWorking_FALSE;
        return false;
    }

    isWorking = IsWorking::isWorking_TRUE;
    
    return true;
}

void MyIMU::getData(){

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
    
    sensors_event_t orientationData, accelData;
    getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
    getEvent(&accelData, Adafruit_BNO055::VECTOR_LINEARACCEL);

    roll = orientationData.orientation.x;
    pitch = orientationData.orientation.y;
    yaw = orientationData.orientation.z;

    /*imu::Vector<3> orientation = getVector(Adafruit_BNO055::VECTOR_EULER);
    roll = orientation.z();
    pitch = orientation.y();
    yaw = orientation.x();*/

    status = Status::status_OK;

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