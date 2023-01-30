#include "globalVars.h"

unsigned long prevMillis;
int refreshRate = 1000;
bool doDebug;

MyBme bme(0x76);
MyGPS gps;
MyLora lora;
MyINA ina(0x40);
MySD sd;
//SdFat SDfat;
File myFile;
MyServer server(80);
MyWiFi wifi;
MyTime rtc;
MyOxygen oxygen(36);
MyDS18B20 ds18(&oneWire, 4);
MyNeo neo(21, 9, NEO_GRBW + NEO_KHZ800);

TaskHandle_t saveData_handle;
TaskHandle_t loraSend_handle;

OneWire oneWire;

void setup(void) {
  Serial.begin(115200);
  //Serial1.begin(9600, 134217756U, 23, 19);
  Wire.begin(21, 22, 400000ul);
  //Wire1.begin(19, 18, 100000ul);
  SPI.begin(33, 27, 14);
  oneWire.begin(4);

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

  if(!digitalRead(RUN_SEVER_PIN)){
    printResult(wifi.setup(ssid, password, true));
    printResult(server.setup(true));
  }

  //xTaskCreate(controlTask, "Control Task", 4096, NULL, 15, NULL);
  xTaskCreate(runServer, "Run Server", 4096, NULL, 3, NULL);
  xTaskCreate(printData, "Print Data", 4096, NULL, 5, NULL);
  xTaskCreate(saveData, "Save Data Task", 4096, NULL, 7, &saveData_handle);
  xTaskCreate(loraSend, "Lora Send Task", 4096, NULL, 10, &loraSend_handle);
  xTaskCreate(getData, "Get Data Task", 4096, NULL, 5, NULL);
  xTaskCreate(runNeo, "Run Neo Pixels", 2048, NULL, 3, NULL);

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
  vTaskDelay(1);
}