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

void controlTask(void *pvParameters){
    while(true){
        !digitalRead(RUN_SEVER_PIN) ? server.mode(true) : server.mode(false);
        vTaskDelay(1);
    }
}

void getData(void *pvParameters){
    getData_lastTime = xTaskGetTickCount();
    while(true){

        rtc.status = Status::status_NACK;
        bme.status = Status::status_NACK;
        gps.status = Status::status_NACK;
        oxygen.status = Status::status_NACK;
        ina.status = Status::status_NACK;
        //ds18.status = Status::status_NACK;
        scd.status = Status::status_NACK;
        lora.status = Status::status_NACK;
        sd.status = Status::status_NACK;

        vTaskResume(ds18getData_handle);
        vTaskResume(gpsGetData_handle);
        rtc.getData();
        bme.getData();
        gps.getData();
        oxygen.getData();
        ina.getData();
        scd.getData();

        vTaskResume(loraSend_handle);
        vTaskResume(saveData_handle);
        vTaskResume(printData_hadle);
        xTaskDelayUntil(&getData_lastTime, refreshRate/portTICK_PERIOD_MS);
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
        Serial.print(analogRead(37)); Serial.println(" V");

        Serial.println();

        sd.printStatus();
        lora.printStatus();

        Serial.println();

        server.printStatus();
    }
}

void ds18getData(void *pvParameters){
    while(true){
        vTaskSuspend(NULL);
        ds18.status = Status::status_NACK;
        ds18.getData();
    }
}

void gpsGetData(void *pvParameters){
    while(true){
        vTaskSuspend(NULL);
        gps.status = Status::status_NACK;
        gps.getData();
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
        sd.status = Status::status_NACK;
        if(xSemaphoreTake(spiSemaphore_hadle, refreshRate/portTICK_PERIOD_MS)){
            sd.save();
            xSemaphoreGive(spiSemaphore_hadle);
        }
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
        lora.handleDio0Rise();
    }
}