# o2: https://www.osha.gov/laws-regs/standardinterpretations/2007-04-02-0
# https://sciencing.com/minimum-oxygen-concentration-human-breathing-15546.html

# co2: Pomoc/unknown.png

# lightintenzity: google (many variants) (illumination)

# humidity: https://www.airthings.com/resources/everything-you-need-to-know-about-humidity
# https://www.weather.gov/lmk/humidity

# temperature: 

# pressure:
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
    
    o2 = zav.kroky2smer2(averages[header.index("o2")], 5, 50, 
