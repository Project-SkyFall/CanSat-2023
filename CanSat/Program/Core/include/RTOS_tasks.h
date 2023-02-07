#ifndef RTOS_TASKS_H
#define RTOS_TASKS_H

void controlTask(void *pvParameters);

void printData(void *pvParameters);

void loraSend(void *pvParameters);

void getData(void *pvParameters);

void runServer(void *pvParameters);

void saveData(void *pvParameters);

void runNeo(void *pvParameters);

void gpsGetDataTask(void *pvParameters);
void gpsFeedObjectTask(void *pvParameters);

extern TaskHandle_t printData_hadle;
extern TaskHandle_t saveData_handle;
extern TaskHandle_t loraSend_handle;
extern TaskHandle_t runNeo_handle;

extern TickType_t getData_lastTime;
extern TickType_t refreshRate;

extern SemaphoreHandle_t spiSemaphore_hadle;

#endif