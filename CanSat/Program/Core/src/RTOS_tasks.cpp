#include "globalVars.h"

#include "RTOS_tasks.h"

#include "myTime.h"
#include "temperature.h"
#include "myOxygen.h"
#include "myINA.h"
#include "myCO2.h"
#include "myServer.h"
#include "mySD.h"
#include "myLora.h"
#include "myNeo.h"
#include "gps.h"

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
        vTaskResume(saveData_handle);
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
        gps.printData();
        oxygen.printData();
        ina.printData();
        ds18.printData();
        scd.printData();

        sd.printStatus();
        lora.printStatus();

        server.printStatus();
    }
}


void runServer(void *pvParameters){
    vTaskSuspend(NULL);
    while(true){
        server.handleClient();
        vTaskDelay(1); 
    }
}

void saveData(void *pvParameters){
    while(true){
        vTaskSuspend(NULL);
        xSemaphoreTake(spiSemaphore_hadle, portMAX_DELAY);
        sd.save();
        xSemaphoreGive(spiSemaphore_hadle);
    }
}

void loraSend(void *pvParameters){
    while(true){
        vTaskSuspend(NULL);
        lora.sendData();
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
        if(xSemaphoreTake(spiSemaphore_hadle, portMAX_DELAY)){
            lora.handleDio0Rise();
            xSemaphoreGive(spiSemaphore_hadle);
        }
    }
}