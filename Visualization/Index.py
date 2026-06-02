# o2: https://www.osha.gov/laws-regs/standardinterpretations/2007-04-02-0
# https://sciencing.com/minimum-oxygen-concentration-human-breathing-15546.html

# co2: Pomoc/unknown.png

# lightintenzity: google (many variants) (illuminance)

# humidity: https://www.airthings.com/resources/everything-you-need-to-know-about-humidity
# https://www.weather.gov/lmk/humidity

# temperature: 

# pressure:https://www.physicsforums.com/threads/what-air-pressure-can-the-human-body-survive.333248/
# https://www.quora.com/What-is-the-minimum-atmospheric-pressure-that-a-human-can-live-comfortably-in-and-not-just-survive

import Zavislosti as zav

def MakeIndex(dataInv, header):
    index = 100
    averages = []
    UpDown = [0,0,0,0,0,0]
    for datInv in dataInv:
        count = 0
        summ = 0
        for num in datInv:
            summ += num
            count += 1
        averages.append(summ/count)
    
    o2 = zav.kroky2smer2(averages[header.index("oxygen")], 5, 38, 23.5, 25, 35, 60,
                         10, 16, 18, 19.5)

    if o2 > 70:
        o2 = 70

    if averages[header.index("oxygen")] > 23.5:
        UpDown[0] = 1
    elif averages[header.index("oxygen")] < 19.5:
        UpDown[0] = -1

    co2 = zav.kroky2smer1(averages[header.index("co2")], 5, 38, 500, 600, 1000, 2500)

    if co2 > 40:
        co2 = 40

    if averages[header.index("co2")] > 500:
        UpDown[1] = 1

    lightintenzity = 0#zav.kroky2smer2(averages[header.index("lightIntensity")],
                       #              5, 70, 30000, 35000, 50000, 100000,
                        #             0, 200, 800, 1000)

    if lightintenzity > 40:
        lightintenzity = 40

    if averages[header.index("lightIntensity")] > 30000:
        UpDown[2] = 1
    elif averages[header.index("lightIntensity")] < 1000:
        UpDown[2] = -1
        
    UpDown[2] = 0
        
    humidity = zav.kroky2smer2(averages[header.index("humidity")], 5, 40,
                               60, 62, 70, 100, 0, 25, 29, 30)

    if humidity > 40:
        humidity = 40

    if averages[header.index("humidity")] > 60:
        UpDown[3] = 1
    elif averages[header.index("humidity")] < 30:
        UpDown[3] = -1
        
    temperature = zav.kroky2smer2(averages[header.index("temperature")], 5, 40,
                                  30, 32, 50, 80, -40, -15, -8, -5)

    if temperature > 40:
        temperature = 40

    if averages[header.index("temperature")] > 30:
        UpDown[4] = 1
    elif averages[header.index("temperature")] < -5:
        UpDown[4] = -1
        
    pressure = zav.kroky2smer2(averages[header.index("pressure")], 5, 40,
                               300, 350, 500, 700, 20, 40, 48, 50) # TEST /10

    if pressure > 70:
        pressure = 70

    if averages[header.index("pressure")] > 300:
        UpDown[5] = 1
    elif averages[header.index("pressure")] < 50:
        UpDown[5] = -1

    index -= (o2+co2+lightintenzity+humidity+temperature+pressure)
    if index < 0:
        index = 0

    return [index, o2, co2, lightintenzity, humidity, temperature, pressure], UpDown
