#include "globalVars.h"
#include "myBme.h"

void emptyFunction(void *pvParameters){
    while(true){
        Serial.println("Hello world");
        vTaskDelay(refreshRate/portTICK_PERIOD_MS); 
    }
}

void printResult(bool input){
    input ? Serial.println("---OK---") : Serial.println("---FAIL---");
}