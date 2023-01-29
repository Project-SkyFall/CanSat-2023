#include "globalVars.h"
#include "myLora.h"

bool MyLora::setup(bool verbose){
    verbose ? Serial.println("---LORA setup---") : 0;

    setPins(32, 13, 10);
    if(!begin(433E6)) return false; 

    return true;
}

void MyLora::send(){
    Serial.println("Sending");

    if(!beginPacket()) return;

    print("Ola amigo");

    endPacket(true);
}

void MyLora::printStatus(){
    Serial.print("Sending data: ");
    status == OK ? Serial.println("OK") : Serial.println("FAIL");
}