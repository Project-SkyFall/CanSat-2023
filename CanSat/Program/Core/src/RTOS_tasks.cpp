#include "globalVars.h"
#include "RTOS_tasks.h"

void myTaskResume(TaskHandle_t taskHandle){
    try{
        vTaskResume(taskHandle);
    }
    catch(...){
        Serial.println("Could not resume task");
    }
}

void controlTask(void *pvParameters){
    while(true){
        !digitalRead(RUN_SEVER_PIN) ? server.mode(true) : server.mode(false);
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
        scd.getData();
        vTaskResume(loraSend_handle);
        //vTaskResume(saveData_handle);
        xTaskDelayUntil(&getData_lastTime, refreshRate/portTICK_PERIOD_MS);
        vTaskResume(printData_hadle);
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
        scd.printData();

        sd.printStatus();
        lora.printStatus();

        server.printStatus();
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
        //vTaskSuspend(NULL);
        if(xSemaphoreTake(spiSemaphore_hadle, portMAX_DELAY)){
            sd.save();
        }
    }
}

void loraSend(void *pvParameters){
    while(true){
        vTaskSuspend(NULL);
        lora.sendData();
        xSemaphoreGive(spiSemaphore_hadle);
    }
}

void runNeo(void *pvParameters){
    while(true){
        neo.animation();
    }
}

void isrHandleDioRise(void *pvParameters){
    while(true){
        vTaskSuspend(NULL);
        xSemaphoreTake(spiSemaphore_hadle, portMAX_DELAY);
        lora.handleDio0Rise();
        xSemaphoreGive(spiSemaphore_hadle);
    }
}