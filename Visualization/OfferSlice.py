def offerSlice(dataInv, header):
    lightintensity = dataInv[header.index("lightIntensity")]
    pressure = dataInv[header.index("pressure")]

    presspre = pressure[0]
    for i in range(len(dataInv[0])-1):
        if pressure[i] > pressure[i+1]:
            if abs(pressure[i+1] - presspre) > 10:
                return dataInv[header.index("time2")][i]
            presspre = pressure[i+1]

    print("peek not found, adjust by yourself")
    return dataInv[header.index("time2")][0]
