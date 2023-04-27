#include "globalVars.h"

#include <esp_task_wdt.h>

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
#include "myBNO.h"
#include "myBH1730.h"
#include "mySpectro.h"
#include "camera.h"

uint32_t cycle;

void controlTask(void *pvParameters){
    while(true){
        !digitalRead(RUN_SEVER_PIN) ? server.mode(true) : server.mode(false);
        digitalRead(neo.enablePin) ? vTaskResume(runNeo_handle) : vTaskSuspend(runNeo_handle);

        vTaskDelay(1);
    }
}
void getData(void *pvParameters){
    getData_lastTime = xTaskGetTickCount();
    while(true){
        rtc.status = Status::status_NACK;
        bme.status = Status::status_NACK;
        oxygen.status = Status::status_NACK;
        ina.status = Status::status_NACK;
        scd.status = Status::status_NACK;
        bh.status = Status::status_NACK;
        
        lora.status = Status::status_FAIL;
        sd.status = Status::status_NACK;
        gps.status = Status::status_NACK;
        ds18.status = Status::status_NACK;
        neo.status = Status::status_NACK;

        vTaskResume(gpsGetData_handle);
        vTaskResume(ds18getData_handle);
        rtc.getData();
        bme.getData();
        oxygen.getData();
        ina.getData();
        scd.getData();
        bno.getData();
        bh.getData();
        asx.getData();

        // Wait until other getData from diferent tasks are done or skiped
        if(!xSemaphoreTake(gpsGetDataDone_semaphore, pollingDelay)) Serial.println("GPS timeout");
        if(!xSemaphoreTake(ds18GetDataDone_semaphore, pollingDelay)) Serial.println("DS18 timeout");
        // Task are done or skiped when pollingDelay passes

        lora.sendData();
        if(sd.mode != Mode::mode_SLEEP){
            sd.save();

            if(!xSemaphoreTake(saveData_semaphore, dataPrintDelay)) Serial.println("SD save timeout");
        }

        neo.updateStatuses();
        vTaskResume(printData_hadle);

        if(sd.mode != Mode::mode_SLEEP) vTaskResume(openFile_handle);
        
        if(!xTaskDelayUntil(&getData_lastTime, refreshRate/portTICK_PERIOD_MS)) Serial.println("Loop to slow!");
        cycle++;
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
        bno.printData();
        bh.printData();
        asx.printData();

        Serial.println();

        sd.printStatus();
        lora.printStatus();

        Serial.println();

        server.printStatus();
        neo.printStatus();
        cam.printStatus();
    }
}

void gpsGetData(void *pvParameters){
    while(true){
        vTaskSuspend(NULL);
        gps.getData();
        xSemaphoreGive(gpsGetDataDone_semaphore);
    }
}

void ds18getData(void *pvParameters){
    while(true){
        vTaskSuspend(NULL);
        ds18.getData();
        xSemaphoreGive(ds18GetDataDone_semaphore);
    }
}

void runServer(void *pvParameters){
    vTaskSuspend(NULL);
    while(true){
        server.handleClient();
        vTaskDelay(1); 
    }
}

/*void saveData(void *pvParameters){
    while(true){
        vTaskSuspend(NULL);
        if(xSemaphoreTake(openFile_semaphore, dataPrintDelay)){
            xSemaphoreTake(spiSemaphore_hadle, portMAX_DELAY);
            sd.save();
            xSemaphoreGive(spiSemaphore_hadle);
            xSemaphoreGive(saveData_semaphore);
        }
    }
}*/

void openFile(void *pvParameters){
    while(true){
        
        vTaskSuspend(NULL);
        esp_task_wdt_init(5, true); //enable panic so ESP32 restarts
        esp_task_wdt_add(NULL); //add current thread to WDT watch
        if(!fileOpened){
            xSemaphoreTake(spiSemaphore_hadle, portMAX_DELAY);
            fileOpened = sd.openFile();
            xSemaphoreGive(spiSemaphore_hadle);
        }
        esp_task_wdt_deinit();
    }
}

void runNeo(void *pvParameters){
    while(true){
        neo.animation();
    }
}

/*void measureBattery(void *pvParameters){
    while(true){
        analogRead(37);
        vTaskDelay(25/portTICK_PERIOD_MS);
    }
}*/

/*void isrHandleDioRise_TX(void *pvParameters){
    while(true){
        vTaskSuspend(NULL);
        lora.handleDio0Rise_TX();
    }
}*/

void isrHandleDioRise(void *pvParameters){
    while(true){
        vTaskSuspend(NULL);
        lora.handleDio0Rise();
    }
}