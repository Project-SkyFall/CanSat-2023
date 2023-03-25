#include "globalVars.h"

#include "RTOS_tasks.h"

#include "myTime.h"
#include "temperature.h"
#include "gps.h"
#include "myLora.h"
#include "myINA.h"
#include "mySD.h"
#include "myServer.h"
#include "myOxygen.h"
#include "myNeo.h"
#include "myINA.h"
#include "myCO2.h"
#include "myBNO.h"

bool doDebug;
String serialBuffer;

MyBme bme(0x77);
MyGPS gps(0x42);
MyLora lora(&SPI, 433E6, 32, 13, 39, 0xFF, 10);
MyINA ina(0x45);
MySD sd(26);
File myFile;
MyServer server(80);
MyWiFi wifi("Project-SkyFall", "1234abcd");
MyTime rtc;
MyOxygen oxygen(36);
MyDS18B20 ds18(&oneWire, 4);
MyNeo neo(16, 10, NEO_GRB + NEO_KHZ800);
#define DOPLNIT 0
MyIMU bno(0x29, DOPLNIT); //TODO doplnit
MyCO2 scd(0x62);

TaskHandle_t runServer_handle;
TaskHandle_t printData_hadle;
TaskHandle_t saveData_handle;
TaskHandle_t getData_task;
TaskHandle_t loraSend_handle;
TaskHandle_t runNeo_handle;
TaskHandle_t isrHandleDioRise_handle;
TaskHandle_t loraCheckTxDone_handle;
TaskHandle_t ds18getData_handle;
TaskHandle_t gpsGetData_handle;

TickType_t getData_lastTime;
TickType_t refreshRate = 1000;

SemaphoreHandle_t spiSemaphore_hadle = NULL;

OneWire oneWire;

void setup(void) {
  Serial.begin(115200);
  //Serial1.begin(9600, 134217756U, 23, 19);
  Wire.begin(21, 22, 400000ul);
  Wire1.begin(19, 18, 100000ul);
  oneWire.begin(4);
  SPI.begin(33, 27, 14);
  
  pinMode(lora.cs, OUTPUT); digitalWrite(lora.cs, HIGH);
  pinMode(sd.cs, OUTPUT); digitalWrite(sd.cs, HIGH);

  pinMode(RUN_SEVER_PIN, INPUT);
  pinMode(37, INPUT);

  Serial.println('\n');
  printResult(rtc.setup(true));
  printResult(sd.setup(true));
  printResult(gps.setup(true));
  printResult(bme.setup(true));
  printResult(ds18.setup(true));
  printResult(lora.setup(true));
  printResult(ina.setup(true));
  printResult(oxygen.setup(true));
  printResult(neo.setup(true));
  printResult(scd.setup(true));
  printResult(bno.setup(true));

  spiSemaphore_hadle = xSemaphoreCreateBinary();

  xTaskCreate(runServer, "Run Server", 4096, NULL, 3, &runServer_handle);
  xTaskCreate(printData, "Print Data", 4096, NULL, 5, &printData_hadle);
  xTaskCreate(saveData, "Save Data Task", 4096, NULL, 7, &saveData_handle);
  xTaskCreate(loraSend, "Lora Send Task", 4096, NULL, 10, &loraSend_handle);
  xTaskCreate(runNeo, "Run Neo Pixels", 2048, NULL, 3, &runNeo_handle);

  xTaskCreate(controlTask, "Control Task", 8192, NULL, 1, NULL);

  xTaskCreate(isrHandleDioRise, "ISR DIO Rise", 2048, NULL, 15, &isrHandleDioRise_handle);

  xTaskCreate(ds18getData, "DS18B20 get data", 2048, NULL, 3, &ds18getData_handle);
  xTaskCreate(gpsGetData, "GPS get data", 4096, NULL, 3, &gpsGetData_handle);

  xTaskCreate(getData, "Get Data Task", 4096, NULL, 5, &getData_task);
  xSemaphoreGive(spiSemaphore_hadle)/* ? Serial.println("SPI semaphore released") : Serial.println("SPI semaphore not released")*/;

  /*xTaskCreate(
    gpsGetDataTask,
    "gpsData",
    4096,
    NULL,
    7,
    NULL);

  xTaskCreate(
    gpsFeedObjectTask,
    "gpsFeedObject",
    4096,
    NULL,
    5,
    NULL);*/
}

void loop(void) {
}