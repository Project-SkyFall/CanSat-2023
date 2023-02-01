
import time
import board
import busio
#Knihovna pro ADS
import adafruit_ads1x15.ads1015 as ADS
from adafruit_ads1x15.analog_in import AnalogIn
#Knihovny pro BME
from adafruit_bme280 import basic as adafruit_bme280

# Create the I2C bus
i2c = busio.I2C(board.SCL, board.SDA)

# Create the BME object
bme280 = adafruit_bme280.Adafruit_BME280_I2C(i2c)
# change this to match the location's pressure (hPa) at sea level
bme280.sea_level_pressure = 1013.25

# Create the ADC object using the I2C bus
ads = ADS.ADS1015(i2c)
# Create differential input between channel 0 and 1
chan = AnalogIn(ads, ADS.P0, ADS.P1)

print("         raw,   volt,  temp, humi, press,  alti")

while True:
    #print("{:>5}\t{:>5}".format("raw", "v"))
    #print("{:>5}\t{:>5.3f}".format(chan.value, chan.voltage))
    #print()

    #print("\nTemperature: %0.1f C" % bme280.temperature)
    #print("Humidity: %0.1f %%" % bme280.relative_humidity)
    #print("Pressure: %0.1f hPa" % bme280.pressure)
    #print("Altitude = %0.2f meters" % bme280.altitude)
    #print("---------------------------------------------------")
    dataword = ("ADSBME; ")
    dataword +=("%6i; " % chan.value)
    dataword +=("%0.3f; " %chan.voltage)
    dataword +=("%0.1f; " % bme280.temperature)
    dataword +=("%0.1f; " % bme280.relative_humidity)
    dataword +=("%0.1f; " % bme280.pressure)
    dataword +=("%0.2f; " % bme280.altitude)
    
    print(dataword)
    
    time.sleep(1)
