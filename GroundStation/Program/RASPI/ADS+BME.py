
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
chan = AnalogIn(ads, ADS.P0)

dataword = ("")
raw = ("")
voltage = ("")
temperature = ("")
humidity = ("")
pressure = ("")
altitude = ("")

def ADSread():
    global raw
    global voltage

    raw = ("%6i" % chan.value)
    voltage =("%0.3f" %chan.voltage)

def BMEread():
    global temperature
    global humidity
    global pressure
    global altitude

    temperature = ("%0.1f" % bme280.temperature)
    humidity = ("%0.1f" % bme280.relative_humidity)
    pressure = ("%0.1f" % bme280.pressure)
    altitude = ("%0.2f" % bme280.altitude)

def DataWord():
    global raw
    global voltage
    global temperature
    global humidity
    global pressure
    global altitude
    global dataword
    
    dataword = ("")
    #dataword = ("ADSBME; ")
    dataword +=("%s; " % voltage)
    dataword +=("%s; " % raw)
    dataword +=("%s; " % temperature)
    dataword +=("%s; " % humidity)
    dataword +=("%s; " % pressure)

    dataword +=("%s; " % altitude)

    print(dataword)

    
    with open("/home/pi/Desktop/adsbme.txt", "w") as file:
        if file.writable():
            file.write(dataword)

    
    
    
while True:
    ADSread()
    BMEread()
    DataWord()

    time.sleep(0.5)
