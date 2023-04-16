import pygame as pg
import sys
import matplotlib.pyplot as plt
import math
from Model import Valec
import time
import Plots
import pygame.camera as pgCam

# INIT PYPLOT FIG
fig = plt.figure(figsize=(6,3.7))

#ROTATE
def new_pos(mini, maxi, miniPos, maxiPos, value, rotation=False):
    if value > maxi:
        value = maxi
    elif value < mini:
        value = mini
    number = (miniPos + ((maxiPos - miniPos) * (value - mini) / (maxi - mini)))
    if rotation:
        number = -number
    return number

def rotate_dial(image, rect, mini, maxi, miniPos, maxiPos, value, offset):
    angle = new_pos(mini, maxi, miniPos, maxiPos, value, True)
    rot_image = pg.transform.rotate(image, angle)
    rot_vector = offset.rotate(angle)
    rot_rect = rot_image.get_rect(center=(rect.center[0] - rot_vector[0], rect.center[1] + rot_vector[1]))
    return rot_image,rot_rect

# TODO COMUNICATION
path = "/home/pi/Desktop/Data.txt"
command_path = "/home/pi/Desktop/Command.txt"

# SETUP
flags = pg.FULLSCREEN | pg.SHOWN
size = (800, 480)
screen = pg.display.set_mode(size, flags)


phase = 1
black = (0,0,0)

# LOAD FONT
pg.font.init()
Font = pg.font.Font(r"font/myriad-pro/MyriadPro-Light.otf", 14)
Font_RT = pg.font.Font(r"font/myriad-pro/MyriadPro-Light.otf", 11)

# SETUP PHASE I
# COORDINATES FOR "dead or alive" AND "up and down"
dORa_x = 462
dORa_y0 = 342
dORa_y1 = 357
dORa_y2 = 373
dORa_y3 = 388
dORa_y4 = 403
dORa_y5 = 418
dORa_y6 = 434
dORa_y7 = 449
up1_x = 316
up1_y = 126
down1_x = 316
down1_y = 155
up2_x = 485
up2_y = 126
down2_x = 485
down2_y = 154

# COORDINATES FOR DIALS
co2_x = 320
co2_y = 255
co2_min = -74
co2_max = 74
o2_x = 482
o2_y = 255
o2_min = -50
o2_max = 50
height_x = 725
height_y_min = 436
height_y_max = 304
pressure_x = 316
pressure_y = 104
pressure_min = -88
pressure_max = 88
temperature_x = 485
temperature_y = 104
temperature_min = -125
temperature_max = 125

# MIN MAX VALUES
pressureMin = 70
pressureMax = 105
temperatureMin = 5
temperatureMax = 55
co2Min = 400
co2Max = 1000
o2Min = 19
o2Max = 21
heightMin = 0
heightMax = 1000

# LOAD IMAGES
background = pg.image.load(r"GUI_grafika/GG_background2.png")
RucickaTemp = pg.image.load(r"GUI_grafika/RucickaTemp2.png")
Alive = pg.image.load(r"GUI_grafika/Active3.png")
Down = pg.image.load(r"GUI_grafika/Down3.png")
Dead = pg.image.load(r"GUI_grafika/Inactive3.png")
RucickaHeight = pg.image.load(r"GUI_grafika/RucickaHeight3.png")
RucickaCO2O2 = pg.image.load(r"GUI_grafika/RucickaO2CO22.png")
RucickaPress = pg.image.load(r"GUI_grafika/RucickaPress2.png")
Up = pg.image.load(r"GUI_grafika/Up3.png")

# GET RECTS
background_Rect = background.get_rect()

Alive_Rect0 = Alive.get_rect(center=(dORa_x, dORa_y0))
Alive_Rect1 = Alive.get_rect(center=(dORa_x, dORa_y1))
Alive_Rect2 = Alive.get_rect(center=(dORa_x, dORa_y2))
Alive_Rect3 = Alive.get_rect(center=(dORa_x, dORa_y3))
Alive_Rect4 = Alive.get_rect(center=(dORa_x, dORa_y4))
Alive_Rect5 = Alive.get_rect(center=(dORa_x, dORa_y5))
Alive_Rect6 = Alive.get_rect(center=(dORa_x, dORa_y6))
Alive_Rect7 = Alive.get_rect(center=(dORa_x, dORa_y7))

RucickaHeight_Rect = RucickaHeight.get_rect(center=(height_x, height_y_min))
RucickaO2_Rect = RucickaCO2O2.get_rect(center=(o2_x, o2_y))
RucickaCO2_Rect = RucickaCO2O2.get_rect(center=(co2_x, co2_y))
RucickaTemp_Rect = RucickaTemp.get_rect(center=(temperature_x, temperature_y))
RucickaPress_Rect = RucickaPress.get_rect(center=(pressure_x, pressure_y))

Up_Rect1 = Up.get_rect(center=(up1_x, up1_y))
Up_Rect2 = Up.get_rect(center=(up2_x, up2_y))
Down_Rect1 = Down.get_rect(center=(down1_x, down1_y))
Down_Rect2 = Down.get_rect(center=(down2_x, down2_y))

# OFFSET VECTORS
VectorTemp = pg.math.Vector2(0, -20)
VectorPress = pg.math.Vector2(0, -28)
VectorCO2O2 = pg.math.Vector2(0, -20)

# SETUP CANSAT MODEL
Cansat = Valec(30, 90, 120, 160, 8)

# SETUP SENT AND RECIEVE TEXT
sent = ""
recieve = ""

# SET PRE VALUES FOR UP OR DOWN
press_pre = None
temp_pre = None



# SETUP PHASE II
# LOAD IMAGES
background2 = pg.image.load(r"GUI_grafika2/GG_background2.png")
background_Rect2 = background2.get_rect()

# DATA PATH
data_path = "/home/pi/Desktop/Backup-data"

# SET SCALE FOR CHARTS
scale = 2/5
plotsize = (600*scale, 370*scale)

# SET CENTER POINTS FOR CHARTS
o2_center = (150, 200)
co2_center = (400, 200)
lightIntensity_center = (650, 200)
pressure_center = (150, 380)
specter_center = (400, 380)
temperature_center = (650, 380)


# TEST
test = True
if test:
    j = 0
    dolu0 = False
    dolu1 = False
    data = []
    for i in range(53):
        data.append(0)

# SET PRE TEXT VALUE
line_pre = None

# SETUP CAM
pgCam.init()
camList = pgCam.list_cameras()
print("Cams:", camList)
numOfCam = 0

camsize = (201,139)
cam = pgCam.Camera(camList[int(numOfCam)], camsize)
cam.start()
Cam_Rect = pg.Rect((593,66), camsize, border_radius=10)


# PHASE I
def phase1():
    # COMMUNICATION AND PARSE DATA
    global line_pre
    global recieve

    if test:
        global data

    if not test:
        try:
            fh = open(path, "r")
            line = fh.readline()
            fh.close()
        except:
            print("chyba cteni souboru")
            return

        if (line_pre != None) and (line == line_pre):
            print("data shodna s predchozimi")
            return

        if line != "" :
            data_str = line.strip().split(";")
            line_pre = line

            data = []
            text = False
            recieve = ""
            for dat_str in data_str:
                try:
                    dat = float(dat_str)
                    data.append(dat)
                except:
                    recieve += dat_str

            print(data)
            print(recieve)

    
    global press_pre
    global temp_pre
    
    # TEST
    if test:
        
        global dolu0
        global dolu1
        global j
        
        for i in range(len(data)):
            data[i] += 0.1
        if dolu0:
            data[0] -= 0.2
        if data[0] > 120:
            dolu0 = True
        if data[0] < 60:
            dolu0 = False
        if dolu1:
            data[1] -= 0.2
        if data[1] > 65:
            dolu1 = True
        if data[1] < -10:
            dolu1 = False
        data[2] += 0.9
        if data[2] > 1100:
            data[2] = 300
        data[3] -= 0.09
        if data[3] > 22:
            data[3] = 18
        if data[4] > 1100:
            data[4] = -100
        
        if data[5] > 100:
             data[5] = 0
        j += 1
        if j%1000 <500:
            for i in range(8):
                data[6+i] = 1
        else:
            for i in range(8):
                data[6+i] = 0
        data[22] -= 0.099
        data[23] -= 0.099

    # ASING DATA
    batteryGS = data[0]    
    tempGS = data[1]
    humidityGS = data[2]
    pressGS = data[3]
    altGS = data[4]
    timeGS = data[5]
    latGS = data[6]
    longGS = data[7]
    rangeCan = data[8]

    dORa0 = data[9]
    dORa1 = data[10]
    dORa2 = data[11]
    dORa3 = data[12]
    dORa4 = data[13]
    dORa5 = data[14]
    dORa7 = data[15]

    lightintensity = data[16]
    temp = data[17]
    press = data[18]
    humidity = data[19]
    battery = data[20]
    co2 = data[21]

    dORa6 = data[22]
    lat = data[23]
    long = data[24]
    height = data[25]
    speed = data[26]
    time = data[27]
    sattelitesNum = data[28]

    roll = data[29]
    pitch = data[30]
    yaw = data[31]

    o2 = data[32]

    asx0 = data[33]
    asx1 = data[34]
    asx2 = data[35]
    asx3 = data[36]
    asx4 = data[37]
    asx5 = data[38]
    asx6 = data[39]
    asx7 = data[40]
    asx8 = data[41]
    asx9 = data[42]
    asx10 = data[43]
    asx11 = data[44]
    asx12 = data[45]
    asx13 = data[46]
    asx14 = data[47]
    asx15 = data[48]
    asx16 = data[49]
    asx17 = data[50]

    transfer = data[51]
    currentAntena = data[52]

    # CALCULATE LAND IN
    if speed != 0:
        landIn = height/speed
    else:
        landIn = 0

    # CALCULATE LAT AND LONG
    degN = int(lat)
    minN = int((lat-degN)*60)
    vteN = round(((lat-degN)*60-minN)*60, 1)

    degE = int(long)
    minE = int((long-degE)*60)
    vteE = round(((long-degE)*60-minE)*60, 1)

    # SETUP NEW FRAME
    screen.fill(black)
    screen.blit(background, background_Rect)
    
    # ROTATE
    RucickaTemp2, RucickaTemp_Rect2 = rotate_dial(RucickaTemp, RucickaTemp_Rect, temperatureMin,
                                                  temperatureMax, temperature_min, temperature_max,
                                                  temp, VectorTemp)

    RucickaPress2, RucickaPress_Rect2 = rotate_dial(RucickaPress, RucickaPress_Rect, pressureMin,
                                                  pressureMax, pressure_min, pressure_max,
                                                  press, VectorPress)

    RucickaCO22, RucickaCO2_Rect2 = rotate_dial(RucickaCO2O2, RucickaCO2_Rect, co2Min,
                                                  co2Max, co2_min, co2_max,
                                                  co2, VectorCO2O2)

    RucickaO22, RucickaO2_Rect2 = rotate_dial(RucickaCO2O2, RucickaO2_Rect, o2Min,
                                                  o2Max, o2_min, o2_max,
                                                  o2, VectorCO2O2)

    # MOVE
    RucickaHeight_Rect2 = RucickaHeight_Rect.move(0, new_pos(heightMin, heightMax,
                                                             height_y_min, height_y_max,
                                                             height) - height_y_min)

    # SHOW UP OR DOWN
    if press_pre == None:
        pass
    elif press_pre < press:
        screen.blit(Up, Up_Rect1)
    elif press_pre > press:
        screen.blit(Down, Down_Rect1)

    if temp_pre == None:
        pass
    elif temp_pre < temp:
        screen.blit(Up, Up_Rect2)
    elif temp_pre > temp:
        screen.blit(Down, Down_Rect2)

    press_pre = press
    temp_pre = temp

    # SHOW DEAD OR ALIVE
    if dORa0:
        screen.blit(Alive, Alive_Rect0)
    else:
        screen.blit(Dead, Alive_Rect0)

    if dORa1:
        screen.blit(Alive, Alive_Rect1)
    else:
        screen.blit(Dead, Alive_Rect1)

    if dORa2:
        screen.blit(Alive, Alive_Rect2)
    else:
        screen.blit(Dead, Alive_Rect2)

    if dORa3:
        screen.blit(Alive, Alive_Rect3)
    else:
        screen.blit(Dead, Alive_Rect3)

    if dORa4:
        screen.blit(Alive, Alive_Rect4)
    else:
        screen.blit(Dead, Alive_Rect4)

    if dORa5:
        screen.blit(Alive, Alive_Rect5)
    else:
        screen.blit(Dead, Alive_Rect5)

    if dORa6:
        screen.blit(Alive, Alive_Rect6)
    else:
        screen.blit(Dead, Alive_Rect6)

    if dORa7:
        screen.blit(Alive, Alive_Rect7)
    else:
        screen.blit(Dead, Alive_Rect7)

    # SHOW CANSAT MODEL
    rects_to_show = Cansat.project(roll, pitch, yaw)
    for rect_to_show in rects_to_show:
        rect = pg.draw.polygon(screen, (220,220,220), rect_to_show)
        line = pg.draw.polygon(screen, black, rect_to_show, 1)

    # TIME RECREATION
    if test:
        time = 311299235959.0
    time = str(int(time))
    RT = (time[6:8] + ":" + time[8:10] + ":" + time[10:12] +
          "        " + time[0:2] + "." + time[2:4] + ".20" + time[4:6])

    # TEXTS
    Text_Name = Font.render(r"X-Sat", True, black)
    Text_Name_Rect = Text_Name.get_rect(center=(129,77))

    Text_BatteryGS = Font.render(str(round(batteryGS)), True, black)
    Text_BatteryGS_Rect = Text_BatteryGS.get_rect(center=(400,32))

    Text_Press = Font.render(str(round(press, 1)), True, black)
    Text_Press_Rect = Text_Press.get_rect(center=(306,142))

    Text_Temp = Font.render(str(round(temp, 1)), True, black)
    Text_Temp_Rect = Text_Temp.get_rect(center=(479,142))

    Text_O2 = Font.render(str(round(o2, 1)), True, black)
    Text_O2_Rect = Text_O2.get_rect(center=(482,274))

    Text_CO2 = Font.render(str(round(co2)), True, black)
    Text_CO2_Rect = Text_CO2.get_rect(center=(306,274))

    Text_Range = Font.render(str(round(rangeCan)), True, black)
    Text_Range_Rect = Text_Range.get_rect(center=(658,346))

    Text_Height = Font.render(str(round(height)), True, black)
    Text_Height_Rect = Text_Height.get_rect(center=(658,371))

    Text_Speed = Font.render(str(round(speed)), True, black)
    Text_Speed_Rect = Text_Speed.get_rect(center=(654,410))

    Text_LandIn = Font.render(str(round(landIn, 1)), True, black)
    Text_LandIn_Rect = Text_LandIn.get_rect(center=(657,430))

    Text_RT = Font_RT.render(RT, True, black)
    Text_RT_Rect = Text_RT.get_rect(center=(650, 451))

    Text_Humidity = Font.render(str(round(humidity, 1)), True, black)
    Text_Humidity_Rect = Text_Humidity.get_rect(center=(399,200))
    
    Text_Transfer = Font.render(str(round(transfer)), True, black)
    Text_Transfer_Rect = Text_Transfer.get_rect(center=(150,293))
    
    Text_Battery = Font.render(str(round(battery, 1)), True, black)
    Text_Battery_Rect = Text_Battery.get_rect(center=(152,312))

    Text_Sent = Font.render(sent, True, black)
    Text_Sent_Rect = Text_Sent.get_rect(center=(128,374))

    Text_Recieve = Font.render(recieve, True, black)
    Text_Recieve_Rect = Text_Recieve.get_rect(center=(128,438))

    Text_Roll = Font_RT.render(str(round(roll)), True, black)
    Text_Roll_Rect = Text_Roll.get_rect(center=(117, 255))

    Text_Pitch = Font_RT.render(str(round(pitch)), True, black)
    Text_Pitch_Rect = Text_Pitch.get_rect(center=(149, 255))

    Text_Yaw = Font_RT.render(str(round(yaw)), True, black)
    Text_Yaw_Rect = Text_Yaw.get_rect(center=(181, 255))

    Text_degN = Font_RT.render(str(round(degN)), True, black)
    Text_degN_Rect = Text_degN.get_rect(center=(641, 255))

    Text_minN = Font_RT.render(str(round(minN)), True, black)
    Text_minN_Rect = Text_minN.get_rect(center=(658, 255))

    Text_vteN = Font_RT.render(str(round(vteN, 1)), True, black)
    Text_vteN_Rect = Text_vteN.get_rect(center=(677, 255))

    Text_degE = Font_RT.render(str(round(degE)), True, black)
    Text_degE_Rect = Text_degE.get_rect(center=(708, 255))

    Text_minE = Font_RT.render(str(round(minE)), True, black)
    Text_minE_Rect = Text_minE.get_rect(center=(725, 255))

    Text_vteE = Font_RT.render(str(round(vteE, 1)), True, black)
    Text_vteE_Rect = Text_vteE.get_rect(center=(744, 255))

    # GET CAM IMAGE
    Cam_Image = pg.transform.smoothscale(cam.get_image(), camsize)
    
    # SHOW TEXTS AND DIALS
    screen.blits(blit_sequence=((Text_Name, Text_Name_Rect),
                                (Text_BatteryGS, Text_BatteryGS_Rect),
                                (Text_Press, Text_Press_Rect),
                                (Text_Temp, Text_Temp_Rect),
                                (Text_O2, Text_O2_Rect),
                                (Text_CO2, Text_CO2_Rect),
                                (Text_Range, Text_Range_Rect),
                                (Text_Height, Text_Height_Rect),
                                (Text_Speed, Text_Speed_Rect),
                                (Text_LandIn, Text_LandIn_Rect),
                                (Text_RT, Text_RT_Rect),
                                (Text_Transfer, Text_Transfer_Rect),
                                (Text_Battery, Text_Battery_Rect),
                                (Text_Sent, Text_Sent_Rect),
                                (Text_Recieve, Text_Recieve_Rect),
                                (Text_Roll, Text_Roll_Rect),
                                (Text_Pitch, Text_Pitch_Rect),
                                (Text_Yaw, Text_Yaw_Rect),
                                (Text_Humidity, Text_Humidity_Rect),
                                (Text_degN, Text_degN_Rect),
                                (Text_minN, Text_minN_Rect),
                                (Text_vteN, Text_vteN_Rect),
                                (Text_degE, Text_degE_Rect),
                                (Text_minE, Text_minE_Rect),
                                (Text_vteE, Text_vteE_Rect)))

    screen.blits(blit_sequence=((RucickaTemp2, RucickaTemp_Rect2),
                                (RucickaPress2, RucickaPress_Rect2),
                                (RucickaCO22, RucickaCO2_Rect2),
                                (RucickaO22, RucickaO2_Rect2),
                                (RucickaHeight, RucickaHeight_Rect2),
                                (Cam_Image, Cam_Rect)))
    pg.display.flip()

def ToPhaseII():
        # LOAD DATA
    global header
    global recieved
    data = []
    if test:
        header = ["random","random","random","random","random","oxygen",
                  "co2", "temperature", "pressure", "lightIntensity",
                  "humidity","asx0","asx1","asx2","asx3","asx4","asx5",
                  "asx6","asx7","asx8","asx9","asx10","asx11","asx12","asx13",
                  "asx14","asx15","asx16","asx17"]
        for i in range(40):
            data.append([])
            for j in range(29):
                data[i].append(j*j-i*j+i+i*i)
    else:
        try:
            fh = open(data_path, "r")
        except:
            print("chyba cteni souboru 2")
            return
        line = fh.readline()
        header = line.strip().split(';')
        while ((line == fh.readline()) != None):
            strline = line.strip().split(';')
            intline = []
            for string in strline:
                intline.append(float(string))
            data.append(intline)
        fh.close()

    global dataInv
    dataInv = []
    recieved = []
    for i in range(len(header)):
        dataInv.append([])
        recieved.append(False)

    for i in range(len(data)):
        for j in range(len(data[0])):
            if not recieved[j]:
                if (data[i][j] != '0.0') and (data[i][j] != '0'):
                    recieved[j] = True
            data[i][j] = float(data[i][j])
            dataInv[j].append(data[i][j])

#    print(data)
 #   print(dataInv)
  #  print(recieved)

    # MAKE NEW WINDOW
#    global screen
    Plots.plots(dataInv, header, fig)
#    pg.quit()
#    pg.init()
#    screen = pg.display.set_mode(size, flags)
    


def phase2():
    # SETUP SCREEN
    screen.fill(black)
    screen.blit(background2, background_Rect2)

    # LOAD CHARTS AND CREATE RECTS
    Plot_Oxygen = pg.transform.smoothscale_by(pg.image.load(r"GUI_grafika2/Plotoxygen.png"),scale)
    Plot_Oxygen_Rect = Plot_Oxygen.get_rect(center=o2_center)

    Plot_CO2 = pg.transform.smoothscale_by(pg.image.load(r"GUI_grafika2/Plotco2.png"),scale)
    Plot_CO2_Rect = Plot_CO2.get_rect(center=co2_center)

    Plot_LightIntensity = pg.transform.smoothscale_by(pg.image.load(r"GUI_grafika2/PlotlightIntensity.png"),scale)
    Plot_LightIntensity_Rect = Plot_LightIntensity.get_rect(center=lightIntensity_center)

    Plot_Pressure = pg.transform.smoothscale_by(pg.image.load(r"GUI_grafika2/Plotpressure.png"),scale)
    Plot_Pressure_Rect = Plot_Pressure.get_rect(center=pressure_center)

    Plot_Specter = pg.transform.smoothscale_by(pg.image.load(r"GUI_grafika2/Plotspecter.png"),scale)
    Plot_Specter_Rect = Plot_Specter.get_rect(center=specter_center)

    Plot_Temperature = pg.transform.smoothscale_by(pg.image.load(r"GUI_grafika2/Plottemperature.png"),scale)
    Plot_Temperature_Rect = Plot_Temperature.get_rect(center=temperature_center)


    screen.blits(blit_sequence=((Plot_Oxygen, Plot_Oxygen_Rect),
                                (Plot_CO2, Plot_CO2_Rect),
                                (Plot_LightIntensity, Plot_LightIntensity_Rect),
                                (Plot_Pressure, Plot_Pressure_Rect),
                                (Plot_Specter, Plot_Specter_Rect),
                                (Plot_Temperature, Plot_Temperature_Rect)))

    pg.display.flip()

    
# MAIN
while True:
    for event in pg.event.get():
        if event.type == pg.QUIT:
            cam.stop()
            pg.quit()
            sys.exit()
            quit()
        if event.type == pg.KEYDOWN:
            if event.__dict__["scancode"] == 79:
                phase = 2
                ToPhaseII()
            elif event.__dict__["scancode"] == 80:
                phase = 1
            
            if event.__dict__["unicode"] == "\x1b":
                cam.stop()
                pg.quit()
                sys.exit()
                quit()
            elif event.__dict__["unicode"] == "\x08":
                sent = sent[0:-1]    
            elif event.__dict__["unicode"] == '\r':
                fh = open(command_path, "w")
                fh.write(sent)
                fh.close()
            else:
                sent = sent + event.__dict__["unicode"]

        if event.type == pg.MOUSEBUTTONDOWN and phase == 2:
            if (event.__dict__["pos"][0] > o2_center[0]-(plotsize[0]/2)) and (event.__dict__["pos"][0] < o2_center[0]+(plotsize[0]/2)) and (
                event.__dict__["pos"][1] > o2_center[1]-(plotsize[1]/2)) and (event.__dict__["pos"][1] < o2_center[1]+(plotsize[1]/2)):
                fig = Plots.makePlot(header, "oxygen", dataInv, fig)
                
            elif (event.__dict__["pos"][0] > co2_center[0]-(plotsize[0]/2)) and (event.__dict__["pos"][0] < co2_center[0]+(plotsize[0]/2)) and (
                event.__dict__["pos"][1] > co2_center[1]-(plotsize[1]/2)) and (event.__dict__["pos"][1] < co2_center[1]+(plotsize[1]/2)):
                fig = Plots.makePlot(header, "co2", dataInv, fig)
                
            elif (event.__dict__["pos"][0] > lightIntensity_center[0]-(plotsize[0]/2)) and (event.__dict__["pos"][0] < lightIntensity_center[0]+(plotsize[0]/2)) and (
                event.__dict__["pos"][1] > lightIntensity_center[1]-(plotsize[1]/2)) and (event.__dict__["pos"][1] < lightIntensity_center[1]+(plotsize[1]/2)):
                fig = Plots.makePlot(header, "lightIntensity", dataInv, fig)
                
            elif (event.__dict__["pos"][0] > pressure_center[0]-(plotsize[0]/2)) and (event.__dict__["pos"][0] < pressure_center[0]+(plotsize[0]/2)) and (
                event.__dict__["pos"][1] > pressure_center[1]-(plotsize[1]/2)) and (event.__dict__["pos"][1] < pressure_center[1]+(plotsize[1]/2)):
                fig = Plots.makePlot(header, "pressure", dataInv, fig)
                
            elif (event.__dict__["pos"][0] > specter_center[0]-(plotsize[0]/2)) and (event.__dict__["pos"][0] < specter_center[0]+(plotsize[0]/2)) and (
                event.__dict__["pos"][1] > specter_center[1]-(plotsize[1]/2)) and (event.__dict__["pos"][1] < specter_center[1]+(plotsize[1]/2)):
                fig = Plots.makePlot(header, "specter", dataInv, fig)
                
            elif (event.__dict__["pos"][0] > temperature_center[0]-(plotsize[0]/2)) and (event.__dict__["pos"][0] < temperature_center[0]+(plotsize[0]/2)) and (
                event.__dict__["pos"][1] > temperature_center[1]-(plotsize[1]/2)) and (event.__dict__["pos"][1] < temperature_center[1]+(plotsize[1]/2)):
                fig = Plots.makePlot(header, "temperature", dataInv, fig)
            
    if (phase == 1):
        phase1()
    elif phase == 2:
        phase2()

