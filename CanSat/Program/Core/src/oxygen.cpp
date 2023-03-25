#include "globalVars.h"

#include "myOxygen.h"

MyOxygen::MyOxygen(uint8_t pin){
    _pin = pin;
}

bool MyOxygen::setup(bool verbose){
    verbose ? Serial.println("---Oxygen setup-----------------------------------") : 0;

    pinMode(_pin, INPUT);

    status = Status::status_OK;
    return true;
}

void MyOxygen::getData(){
    int val = analogRead(_pin);
    if(val < 500){
        status = Status::status_FAIL;
        return;
    }
    status = Status::status_OK;
    concentration = map(val, 0, 4095, 0, 25);
}

void MyOxygen::printData(){
    Serial.print("OXYGEN: ");
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

    Serial.print(concentration); Serial.println(" %");
}