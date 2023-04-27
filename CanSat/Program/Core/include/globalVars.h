#ifndef GLOBALVARS_H
#define GLOBALVARS_H

#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include <OneWire.h>

#include "esp32/rom/rtc.h"

enum class Status : uint8_t {status_NACK = 3, status_FAIL = 0, status_OK = 1, status_SLEEP = 2};
enum class Mode {mode_RUN = 0, mode_SLEEP = 1};
enum class IsWorking {isWorking_FALSE = 0, isWorking_TRUE = 1};

#define RUN_SEVER_PIN 35

bool wireCheck(byte address, TwoWire& theWire = Wire);

void printResult(bool input);

String verbose_print_reset_reason(RESET_REASON reason);

String sensorStatuses();

extern bool doDebug;
extern String serialBuffer;
extern bool softwareReset;

extern bool launched;

extern OneWire oneWire;

#endif


