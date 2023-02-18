#include "globalVars.h"

unsigned long prevMillis;
//int refreshRate = 1000;
bool doDebug;

MyBme bme(0x76);
MyGPS gps;
MyLora lora(&SPI, 433E6, 32, 13, 39, 0x60);
MyINA ina(0x40);
MySD sd(26);
File myFile;
MyServer server(80);
MyWiFi wifi(ssid, password);
MyTime rtc;
MyOxygen oxygen(36);
MyDS18B20 ds18(&oneWire, 4);
MyNeo neo(16, 10, NEO_GRBW + NEO_KHZ800);
MyIMU bno;
MyCO2 scd(0x62);

TaskHandle_t runServer_handle;
TaskHandle_t printData_hadle;
TaskHandle_t saveData_handle;
TaskHandle_t loraSend_handle;
TaskHandle_t runNeo_handle;
TaskHandle_t isrHandleDioRise_handle;

TickType_t getData_lastTime;
TickType_t refreshRate = 1000;

SemaphoreHandle_t spiSemaphore_hadle = NULL;

OneWire oneWire;

void setup(void) {
  Serial.begin(115200);
  //Serial1.begin(9600, 134217756U, 23, 19);
  Wire.begin(21, 22, 400000ul);
  //Wire1.begin(19, 18, 100000ul);
  oneWire.begin(4);
  SPI.begin(33, 27, 14);
  
  pinMode(lora.cs, OUTPUT); digitalWrite(lora.cs, HIGH);
  pinMode(sd.cs, OUTPUT); digitalWrite(sd.cs, HIGH);

  pinMode(RUN_SEVER_PIN, INPUT);

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

  spiSemaphore_hadle = xSemaphoreCreateBinary();

  xTaskCreate(runServer, "Run Server", 4096, NULL, 3, &runServer_handle);
  xTaskCreate(printData, "Print Data", 4096, NULL, 5, &printData_hadle);
  xTaskCreate(saveData, "Save Data Task", 4096, NULL, 7, &saveData_handle);
  xTaskCreate(loraSend, "Lora Send Task", 4096, NULL, 10, &loraSend_handle);
  xTaskCreate(getData, "Get Data Task", 4096, NULL, 5, NULL);
  xTaskCreate(runNeo, "Run Neo Pixels", 2048, NULL, 3, &runNeo_handle);
  xTaskCreate(controlTask, "Control Task", 8192, NULL, 1, NULL);

  xTaskCreate(isrHandleDioRise, "ISR DIO Rise", 2048, NULL, 15, &isrHandleDioRise_handle);

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