#ifndef RTOS_TASKS_H
#define RTOS_TASKS_H

void controlTask(void *pvParameters);

void printData(void *pvParameters);

void loraSend(void *pvParameters);

void getData(void *pvParameters);

void runServer(void *pvParameters);

void saveData(void *pvParameters);

void gpsGetDataTask(void *pvParameters);
void gpsFeedObjectTask(void *pvParameters);

extern TaskHandle_t saveData_handle;
extern TaskHandle_t loraSend_handle;

#endif