#ifndef MYBME_H
#define MYBME_H

#include <Adafruit_BME280.h>
#include <DallasTemperature.h>

class MyBme : public Adafruit_BME280{
    public:
    MyBme(byte address);
    bool setup(bool debug=false);
    void getData();
    void printData();

    Status status;
    float temperature;
    float pressure;
    float humidity;

    private:
    byte _address;
};

class MyDS18B20 : public DallasTemperature{
    public:
    MyDS18B20(OneWire* bus, uint8_t pin);
    using DallasTemperature::DallasTemperature;
    bool setup(bool verbose=false);
    void getData();
    void printData();

    Status status;
    float temperature;

    private:
    uint8_t _pin;
    OneWire* _bus;
};

void bmeGetDataTask(void *pvParameters);

extern MyBme bme;
extern MyDS18B20 ds18;

#endif