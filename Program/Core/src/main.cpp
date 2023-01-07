#include "globalVars.h"

#include "myWifi.h"
#include "server.h"
#include "gps.h"
#include "myBme.h"
#include "myLora.h"
#include "gps_temp.h"
#include "myINA219.h"

unsigned long prevMillis;
int refreshRate = 1000;

MyBme bme;
//MyGPS gps;
MyLora lora;
TinyGPSPlus gps;
Adafruit_INA219 ina;
//WebServer server(80);

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600, 134217756U, 23, 19);
  Wire.begin(21, 22, 400000ul);
  Wire1.begin(37, 36, 100000ul);
  SPI.begin(33, 27, 14);
  //WiFi.softAP(ssid, password);

  //printResult(gps.setup());
  gpsSetup();
  printResult(bme.setup());
  printResult(lora.setup());
  printResult(ina.begin());

  /*xTaskCreate(
    bmeGetData,
    "bmeGetData",
    2048,
    NULL,
    7,
    NULL);

  xTaskCreate(
    loraSendTask,
    "loraSendTask",
    2048,
    NULL,
    10,
    NULL);

  xTaskCreate(
    gpsGetDataTask,
    "gpsData",
    5000,
    NULL,
    7,
    NULL);

  xTaskCreate(
    gpsFeedObjectTask,
    "gpsFeedObject",
    5000,
    NULL,
    5,
    NULL);*/
  
}

void loop() {
  //vTaskDelay(1);
  printINA();
  delay(1000);
}