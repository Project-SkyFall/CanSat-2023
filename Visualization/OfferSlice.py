def offerSlice(dataInv, header):
    lightintensity = dataInv[header.index("lightIntensity")]
    pressure = dataInv[header.index("pressure")]

    presspre = None
    slope = False
    for i in range(len(dataInv[0])):
        if presspre is None or presspre <= pressure[i]:
            if slope == True:
                if abs(pressure[i] - presspre) > 10:
                    return dataInv[header.index("time")][i]
            presspre = pressure[i]
        else:
            slope = True

    print("peek not found, adjust by yourself")
    return dataInv[header.index("time")][0]
