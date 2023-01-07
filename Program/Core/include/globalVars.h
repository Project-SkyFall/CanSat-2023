#ifndef GLOBALVARS_H
#define GLOBALVARS_H

#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>

void printResult(bool input);

void loraSendTask(void *pvParameters);

void gpsGetDataTask(void *pvParameters);
void gpsFeedObjectTask(void *pvParameters);

extern int refreshRate;

#endif



