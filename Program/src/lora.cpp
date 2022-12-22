#include "globalVars.h"
#include "myLora.h"

bool MyLora::setup(){
    Serial.println("---LORA setup---");

    LoRa.setPins(32, 13, 10);
    if(!LoRa.begin(433E6)) return false; 

    return true;
}

void MyLora::send(){
    Serial.println("Sending");

    if(!LoRa.beginPacket()) return;

    LoRa.print("Ola amigo");

    LoRa.endPacket(true);
}

void loraSendTask(void *pvParameters){
    while(true){
        //lora.send();
        Serial.println("Trying to send");
        if(LoRa.beginPacket()){
            Serial.println("Sending");

            LoRa.print("Ola amigo");

            LoRa.endPacket(true);
        }

        vTaskDelay(refreshRate/portTICK_PERIOD_MS);
    }
}