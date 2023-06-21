#ifndef RTOS_TASKS_H
#define RTOS_TASKS_H

#define pollingDelay 700/portTICK_PERIOD_MS - (xTaskGetTickCount()-getData_lastTime)/portTICK_PERIOD_MS
#define dataPrintDelay 850/portTICK_PERIOD_MS - (xTaskGetTickCount()-getData_lastTime)/portTICK_PERIOD_MS

void controlTask(void *pvParameters);

void printData(void *pvParameters);

void loraSend(void *pvParameters);

void getData(void *pvParameters);

void runServer(void *pvParameters);

void runNeo(void *pvParameters);

void loraCheckTxDone(void *pvParameters);

void isrHandleDioRise(void *pvParameters);

void gpsGetDataTask(void *pvParameters);
void gpsFeedObjectTask(void *pvParameters);

void ds18getData(void *pvParameters);
void gpsGetData(void *pvParameters);

extern uint32_t cycle;

extern TaskHandle_t runServer_handle;
extern TaskHandle_t printData_hadle;
extern TaskHandle_t getData_handle;
extern TaskHandle_t runNeo_handle;
extern TaskHandle_t isrHandleDioRise_handle;
extern TaskHandle_t ds18getData_handle;
extern TaskHandle_t gpsGetData_handle;

extern TickType_t getData_lastTime;
extern TickType_t refreshRate;

extern SemaphoreHandle_t spiSemaphore_hadle;
extern SemaphoreHandle_t gpsGetDataDone_semaphore;
extern SemaphoreHandle_t ds18GetDataDone_semaphore;
extern SemaphoreHandle_t saveData_semaphore;
extern SemaphoreHandle_t openFile_semaphore;

#endif