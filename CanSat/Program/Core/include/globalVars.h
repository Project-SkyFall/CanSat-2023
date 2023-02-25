#ifndef GLOBALVARS_H
#define GLOBALVARS_H

#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include <OneWire.h>

enum class Status : int8_t {status_OK = 1, status_FAIL = -1, status_NACK = 0, status_SLEEP = 2};

#define RUN_SEVER_PIN 35

bool wireCheck(byte address);

void printResult(bool input);

extern bool doDebug;
extern String serialBuffer;

extern OneWire oneWire;

#endif



