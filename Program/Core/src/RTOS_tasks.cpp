#include "globalVars.h"
#include "RTOS_tasks.h"

void controlTask(void *pvParameters){
    while(true){
        vTaskDelay(1);
    }
}

void getData(void *pvParameters){
    getData_lastTime = xTaskGetTickCount();
    while(true){
        rtc.getData();
        bme.getData();
        //gps.getData();
        oxygen.getData();
        ina.getData();
        ds18.getData();
        vTaskResume(printData_hadle);
        vTaskResume(saveData_handle);
        vTaskResume(loraSend_handle);
        xTaskDelayUntil(&getData_lastTime, refreshRate/portTICK_PERIOD_MS);
    }
}

void printData(void *pvParameters){
    while(true){
        vTaskSuspend(NULL);

        Serial.println("\n--------------------------------------------------");

        rtc.printData();
        bme.printData();
        ds18.printData();
        gps.printData();
        oxygen.printData();
        ina.printData();

        sd.printStatus();
        lora.printStatus();
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

void runNeo(void *pvParameters){
    while(true){
        neo.animation();
        vTaskDelay(1);
    }
}