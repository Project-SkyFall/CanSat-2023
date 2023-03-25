#ifndef GLOBALVARS_H
#define GLOBALVARS_H

#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include <OneWire.h>

enum class Status : int8_t {status_NACK = 0, status_FAIL = -1, status_OK = 1, status_SLEEP = 2};
enum class Mode {mode_RUN = 0, mode_SLEEP = 1};
enum class IsWorking {isWorking_FALSE = 0, isWorking_TRUE = 1};

#define RUN_SEVER_PIN 35

bool wireCheck(byte address, TwoWire& theWire = Wire);

void printResult(bool input);

extern bool doDebug;
extern String serialBuffer;

extern OneWire oneWire;

#endif



