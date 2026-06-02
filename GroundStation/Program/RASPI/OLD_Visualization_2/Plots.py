import matplotlib.pyplot as plt
import numpy as np

# make 1 plot and either save it as .png or plot it out
def makePlot(header, label, dataInv, show=True):
    fig = plt.figure(figsize=(6,3.7))
    ax = plt.subplot(111)
    if label == "specter":
        ax.set_xlabel("wavelenght")
        ax.set_ylabel("average intensity")
        specter = []
        for i in range(18):
            suma = 0
            count = 0
            for j in range(len(dataInv[0])):
                suma += dataInv[header.index("asx0")+i][j]
                count += 1
            specter.append(suma/count)
        frequencies = [410, 435, 460, 485, 510, 535, 560, 585, 610, 645, 680, 705,
                       730, 760, 810, 860, 900, 940]
        ax.plot(frequencies, specter)
    elif label == "oxygen":
        ax.set_ylabel("oxygen (blue), humidity (magenta)")
        ax.set_xlabel("time")
        ax.plot(dataInv[0], dataInv[header.index(label)], '-b')
        ax.plot(dataInv[0], dataInv[header.index("humidity")], '-m')
    else:
        ax.set_ylabel(label)
        ax.set_xlabel("time")
        ax.plot(dataInv[0], dataInv[header.index(label)], '-b')
    if show:
        plt.show()
    else:
        fig.savefig("GUI_grafika2/Plot"+label+".png")
    plt.close(fig)

def sliceData(start, end, dataInv):
    isStart = False
    isEnd = False
    for i in range(len(dataInv[0])):
        if isEnd:
            break
        if not isStart:
            if dataInv[0][i] >= start:
                startIndex = i
                isStart = True
        else:
            if dataInv[0][i] > end:
                endIndex = i
                isEnd = True       

    for i in range(len(dataInv)):
        if not isEnd:
            dataInv[i] = dataInv[i][:startIndex]
        else:
            dataInv[i] = dataInv[i][:startIndex]+dataInv[i][endIndex:]

    plots()

def plots(dataInv, header):
    plotList = ["oxygen", "co2", "temperature", "pressure", "lightIntensity",
                "specter"]

    for label in plotList:
        makePlot(header, label, dataInv, False)
