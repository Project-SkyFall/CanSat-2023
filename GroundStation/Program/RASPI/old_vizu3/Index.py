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
    for datInv in dataInv:
        count = 0
        summ = 0
        for num in datInv:
            summ += num
            count += 1
        averages.append(summ/count)
    
    o2 = zav.kroky2smer2(averages[header.index("oxygen")], 5, 40, 23.5, 25, 35, 60,
                         10, 16, 18, 19.5)

    if o2 > 80:
        o2 = 80


    co2 = zav.kroky2smer1(averages[header.index("co2")], 5, 45, 500, 600, 1000, 2500)

    if co2 > 80:
        co2 = 80

    lightintenzity = zav.kroky2smer2(averages[header.index("lightIntensity")],
                                     5, 100, 30000, 35000, 50000, 100000,
                                     0, 200, 800, 1000)

    if lightintenzity > 50:
        lightintenzity = 50

    humidity = zav.kroky2smer2(averages[header.index("humidity")], 5, 50,
                               60, 62, 70, 100, 0, 25, 29, 30)

    if humidity > 50:
        humidity = 50

    temperature = zav.kroky2smer2(averages[header.index("temperature")], 5, 50,
                                  30, 32, 50, 80, -5, -8, -15, -40)

    if temperature > 50:
        temperature = 50

    pressure = zav.kroky2smer2(averages[header.index("pressure")], 5, 50,
                               300, 350, 500, 700, 20, 40, 48, 50)

    if pressure > 80:
        pressure = 80

    index -= (o2+co2+lightintenzity+humidity+temperature+pressure)
    if index < 0:
        index = 0

    return [index, o2, co2, lightintenzity, humidity, temperature, pressure]
