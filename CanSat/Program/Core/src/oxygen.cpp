#include "globalVars.h"

MyOxygen::MyOxygen(uint8_t pin){
    _pin = pin;
}

bool MyOxygen::setup(bool verbose){
    verbose ? Serial.println("---Oxygen setup-----------------------------------") : 0;

    pinMode(_pin, INPUT);

    status = OK;
    return true;
}

void MyOxygen::getData(){
    if(status == SLEEP) return;

    int val = analogRead(_pin);

    concentration = map(val, 0, 4095, 0, 25);
}

void MyOxygen::printData(){
    Serial.print("OXYGEN: ");
    if(status == SLEEP){
        Serial.println("SLEEPING");
        return;
    }
    else if(status == FAIL){
        Serial.println("FAILED");
        return;
    }

    Serial.print(concentration); Serial.println(" %");
}