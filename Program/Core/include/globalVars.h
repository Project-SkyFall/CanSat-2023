#ifndef GLOBALVARS_H
#define GLOBALVARS_H

#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include <OneWire.h>

#include "myServer.h"
#include "gps.h"
#include "temperature.h"
#include "myLora.h"
#include "gps_temp.h"
#include "myINA.h"
#include "mySD.h"
#include "myTime.h"
#include "RTOS_tasks.h"
#include "myOxygen.h"

#define FAIL 0
#define OK 1
#define SLEEP 2

#define RUN_SEVER_PIN 35

bool wireCheck(byte address);

void printResult(bool input);

extern int refreshRate;
extern bool doDebug;

extern OneWire oneWire;

#endif



