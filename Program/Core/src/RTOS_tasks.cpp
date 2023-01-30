#include "globalVars.h"
#include "RTOS_tasks.h"

void controlTask(void *pvParameters){
    while(true){
        vTaskDelay(1);
    }
}

void getData(void *pvParameters){
    while(true){
        rtc.getData();
        bme.getData();
        //gps.getData();
        oxygen.getData();
        ina.getData();
        ds18.getData();
        vTaskResume(saveData_handle);
        vTaskResume(loraSend_handle);
        vTaskDelay(refreshRate/portTICK_PERIOD_MS);
    }
}

void printData(void *pvParameters){
    while(true){
        Serial.println("\n--------------------------------------------------");

        rtc.printData();
        bme.printData();
        ds18.printData();
        gps.printData();
        oxygen.printData();
        ina.printData();

        sd.printStatus();
        lora.printStatus();

        //Serial.println("--------------------------------------------------\n");
        vTaskDelay(refreshRate/portTICK_PERIOD_MS);
    }
}


void runServer(void *pvParameters){
    while(true){
       server.handleClient();
       vTaskDelay(1); 
    }
}

void saveData(void *pvParameters){
    while(true){
        vTaskSuspend(NULL);
        sd.save();
    }
}

void loraSend(void *pvParameters){
    while(true){
        vTaskSuspend(NULL);
        lora.status = OK;
    }
}