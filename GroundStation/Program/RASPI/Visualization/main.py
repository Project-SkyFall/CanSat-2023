import pygame as pg
import sys
import matplotlib.pyplot as plt
import math
from Model import Valec
import time
import Plots
import pygame.camera as pgCam
from OfferSlice import offerSlice
import Index

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
Font_Index = pg.font.Font(r"font/myriad-pro/MyriadPro-Light.otf", 100)
Font_Var = pg.font.Font(r"font/myriad-pro/MyriadPro-Light.otf", 45)

# SETUP PHASE I
# COORDINATES FOR "dead or alive" AND "up and down"
dORa_x = 458
dORa_y0 = 319
dORa_y1 = 334
dORa_y2 = 350
dORa_y3 = 365
dORa_y4 = 380
dORa_y5 = 395
dORa_y6 = 412
dORa_y7 = 427
dORa_y8 = 442
dORa_y9 = 457
dORa_y10 = 471
dORa_y11 = 304
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
co2_y = 244
co2_min = -75
co2_max = 74
o2_x = 482
o2_y = 244
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
co2Min = 0.200
co2Max = 5.000
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
Bluective = pg.image.load(r"GUI_grafika/Bluective3.png")

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
Alive_Rect8 = Alive.get_rect(center=(dORa_x, dORa_y8))
Alive_Rect9 = Alive.get_rect(center=(dORa_x, dORa_y9))
Alive_Rect10 = Alive.get_rect(center=(dORa_x, dORa_y10))
Alive_Rect11 = Alive.get_rect(center=(dORa_x, dORa_y11))

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
Cansat = Valec(30, 90, 129, 160, 8)

# SETUP SENT AND RECIEVE TEXT
sent = ""
recieve = ""

# SET PRE VALUES FOR UP OR DOWN
press_pre = None
temp_pre = None
time_pre = None
height_pre = None
speed_pre = None


# SETUP PHASE II
# LOAD IMAGES
background2 = pg.image.load(r"GUI_grafika2/GG_background2.png")
background_Rect2 = background2.get_rect()

# DATA PATH
data_path = "/home/pi/Desktop/Backup-data.txt"

# SET SCALE FOR CHARTS
scale = 2/5
plotsize = (600*scale, 370*scale)

# SET CENTER POINTS FOR CHARTS
o2_center = (155, 205)
co2_center = (155, 380)
lightIntensity_center = (645, 205)
pressure_center = (400, 380)
specter_center = (645, 380)
temperature_center = (400, 205)

# SETUP SLICE TEXT
slicetext = ""
delete = False
show = False


# SETUP PHASE III
# LOAD IMAGES
background3 = pg.image.load(r"GUI_grafika3/GG_background2.png")
Up3 = pg.image.load(r"GUI_grafika3/Up.png")
Down3 = pg.image.load(r"GUI_grafika3/Down.png")

# RECTS
background3_Rect = background3.get_rect()
Up3_O2_Rect = Up3.get_rect(center = (49, 201))
Up3_Hum_Rect = Up3.get_rect(center = (133, 201))
Up3_Temp_Rect = Up3.get_rect(center = (215, 201))
Up3_Int_Rect = Up3.get_rect(center = (580, 201))
Up3_CO2_Rect = Up3.get_rect(center = (663, 201))
Up3_Press_Rect = Up3.get_rect(center = (746, 201))
Down3_O2_Rect = Down3.get_rect(center = (49, 295))
Down3_Hum_Rect = Down3.get_rect(center = (133, 295))
Down3_Temp_Rect = Down3.get_rect(center = (215, 295))
Down3_Int_Rect = Down3.get_rect(center = (579, 294))
Down3_CO2_Rect = Down3.get_rect(center = (663, 294))
Down3_Press_Rect = Down3.get_rect(center = (746, 295))

# TEST
test = False
if test:
    j = 0
    dolu0 = False
    dolu1 = False
    data = []
    for i in range(57):
        data.append(0)

# SET PRE TEXT VALUE
line_pre = None

# SETUP CAM
pgCam.init()
try:
    camList = pgCam.list_cameras()
    print("Cams:", camList)
    numOfCam = 0

    camsize = (204,142)
    cam = pgCam.Camera(camList[int(numOfCam)], camsize)
    cam.start()
    Cam_Rect = pg.Rect((592,65), camsize, border_radius=10)
except:
    pass


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
    global time_pre
    global height_pre
    global speed_pre
    
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
        data[31] *= 1.001

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

    dORa4 = data[9] #BH
    dORa0 = data[10] #BME
    dORa3 = data[11] #SCD
    dORa1 = data[12] #BNO
    dORa2 = data[13] #OXY
    dORa5 = data[14] #ASX
    dORa7 = data[15] #SD
    dORa10 = data[16] #GPS
    dORa8 = data[17] #INA
    dORa6 = data[18] #NEO
    dORa9 = data[19] #DS18

    time = data[20]

    lightintensity = data[21]
    temp = data[22]
    press = data[23]
    humidity = data[24]

    battery = data[25]
    current = data[26]
    power = data[27]
    
    co2 = data[28] / 1000

    lat = data[29]
    long = data[30]
    height_GPS = data[31]
    height = data[32]
    sattelitesNum = data[33]

    roll = data[34]
    pitch = data[35]
    yaw = data[36]

    o2 = data[37]

    asx0 = data[38]
    asx1 = data[39]
    asx2 = data[40]
    asx3 = data[41]
    asx4 = data[42]
    asx5 = data[43]
    asx6 = data[44]
    asx7 = data[45]
    asx8 = data[46]
    asx9 = data[47]
    asx10 = data[48]
    asx11 = data[49]
    asx12 = data[50]
    asx13 = data[51]
    asx14 = data[52]
    asx15 = data[53]
    asx16 = data[54]
    asx17 = data[55]

    transfer = data[56]
    
    dORa11 = data[57]
    
    time2 = data[58]

    # CALCULATE SPEED
    if time_pre == None:
        speed = 0
        time_pre = time
        height_pre = height
    elif abs(time - time_pre) >= 2:
        speed = abs(height - height_pre)/(time - time_pre)
        height_pre = height
        time_pre = time
    else:
        speed = speed_pre

    speed_pre = speed

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
    if dORa0 == 1:
        screen.blit(Alive, Alive_Rect0)
    elif dORa0 == 0:
        screen.blit(Dead, Alive_Rect0)
    else:
        screen.blit(Bluective, Alive_Rect0)

    if dORa1 == 1:
        screen.blit(Alive, Alive_Rect1)
    elif dORa1 == 0:
        screen.blit(Dead, Alive_Rect1)
    else:
        screen.blit(Bluective, Alive_Rect1)

    if dORa2 == 1:
        screen.blit(Alive, Alive_Rect2)
    elif dORa2 == 0:
        screen.blit(Dead, Alive_Rect2)
    else:
        screen.blit(Bluective, Alive_Rect2)

    if dORa3 == 1:
        screen.blit(Alive, Alive_Rect3)
    elif dORa3 == 0:
        screen.blit(Dead, Alive_Rect3)
    else:
        screen.blit(Bluective, Alive_Rect3)

    if dORa4 == 1:
        screen.blit(Alive, Alive_Rect4)
    elif dORa4 == 0:
        screen.blit(Dead, Alive_Rect4)
    else:
        screen.blit(Bluective, Alive_Rect4)

    if dORa5 == 1:
        screen.blit(Alive, Alive_Rect5)
    elif dORa5 == 0:
        screen.blit(Dead, Alive_Rect5)
    else:
        screen.blit(Bluective, Alive_Rect5)

    if dORa6 == 1:
        screen.blit(Alive, Alive_Rect6)
    elif dORa6 == 0:
        screen.blit(Dead, Alive_Rect6)
    else:
        screen.blit(Bluective, Alive_Rect6)

    if dORa7 == 1:
        screen.blit(Alive, Alive_Rect7)
    elif dORa7 == 0:
        screen.blit(Dead, Alive_Rect7)
    else:
        screen.blit(Bluective, Alive_Rect7)

    if dORa8 == 1:
        screen.blit(Alive, Alive_Rect8)
    elif dORa8 == 0:
        screen.blit(Dead, Alive_Rect8)
    else:
        screen.blit(Bluective, Alive_Rect8)

    if dORa9 == 1:
        screen.blit(Alive, Alive_Rect9)
    elif dORa9 == 0:
        screen.blit(Dead, Alive_Rect9)
    else:
        screen.blit(Bluective, Alive_Rect9)

    if dORa10 == 1:
        screen.blit(Alive, Alive_Rect10)
    elif dORa10 == 0:
        screen.blit(Dead, Alive_Rect10)
    else:
        screen.blit(Bluective, Alive_Rect10)
        
    if dORa11 == 1:
        screen.blit(Alive, Alive_Rect11)
    elif dORa11 == 0:
        screen.blit(Dead, Alive_Rect11)
    else:
        screen.blit(Bluective, Alive_Rect11)

    # SHOW CANSAT MODEL
    rects_to_show = Cansat.project(yaw*math.pi/180, -pitch*math.pi/180, -roll*math.pi/180)
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
    Text_Name = Font.render(r"CanSat", True, black)
    Text_Name_Rect = Text_Name.get_rect(center=(129,77))

    Text_BatteryGS = Font.render(str(round(batteryGS, 2)), True, black)
    Text_BatteryGS_Rect = Text_BatteryGS.get_rect(center=(400,32))

    Text_Press = Font.render(str(round(press, 1)), True, black)
    Text_Press_Rect = Text_Press.get_rect(center=(306,142))

    Text_Temp = Font.render(str(round(temp, 1)), True, black)
    Text_Temp_Rect = Text_Temp.get_rect(center=(479,142))

    Text_O2 = Font.render(str(round(o2, 1)), True, black)
    Text_O2_Rect = Text_O2.get_rect(center=(482,263))

    Text_CO2 = Font.render(str(round(co2, 3)), True, black)
    Text_CO2_Rect = Text_CO2.get_rect(center=(306,263))

    Text_Range = Font.render(str(round(rangeCan)), True, black)
    Text_Range_Rect = Text_Range.get_rect(center=(658,346))

    Text_Height = Font.render(str(round(height)), True, black)
    Text_Height_Rect = Text_Height.get_rect(center=(658,371))

    Text_Speed = Font.render(str(round(speed)), True, black)
    Text_Speed_Rect = Text_Speed.get_rect(center=(654,410))

    Text_LandIn = Font.render(str(round(landIn)), True, black)
    Text_LandIn_Rect = Text_LandIn.get_rect(center=(657,430))

    Text_RT = Font_RT.render(RT, True, black)
    Text_RT_Rect = Text_RT.get_rect(center=(650, 451))

    Text_Humidity = Font.render(str(round(humidity, 1)), True, black)
    Text_Humidity_Rect = Text_Humidity.get_rect(center=(399,200))
    
    Text_Transfer = Font.render(str(round(transfer, 1)), True, black)
    Text_Transfer_Rect = Text_Transfer.get_rect(center=(150,293))
    
    Text_Battery = Font.render(str(round(battery, 2)), True, black)
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
    try:
        Cam_Image = pg.transform.smoothscale(cam.get_image(), camsize)
        screen.blit(Cam_Image, Cam_Rect)
    except:
        pass
    
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
                                (RucickaHeight, RucickaHeight_Rect2)))
    pg.display.flip()

def ToPhaseII():
        # LOAD DATA
    global header
    global recieved
    data = []
    if test:
        header = ["time","lat","long","random","random","oxygen",
                  "co2", "temperature", "pressure", "lightIntensity",
                  "humidity","asx0","asx1","asx2","asx3","asx4","asx5",
                  "asx6","asx7","asx8","asx9","asx10","asx11","asx12","asx13",
                  "asx14","asx15","asx16","asx17"]
        for i in range(40):
            data.append([])
            for j in range(29):
                data[i].append(i+i*i*j)
    else:
        try:
            fh = open(data_path, "r")
        except:
            print("chyba cteni souboru 2")
            return
        text = fh.read()
        text = text.strip().split("\n")
        header = text[0].strip().split(';')
        text = text[1:]
        for line in text:
            strline = line.strip().split(';')
            intline = []
            for string in strline:
                if string == "":
                    intline.append(0.0)
                else:
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

    if test:
        press = [0,1,0,-1,0,3,4,6,8,9,19,29,39,40,50,66,67,54,43,32,
                 23,21,12,10,9,8,3,2,3,1,0,0,0,0,0,0,0,0,0,0]
        dataInv[header.index("pressure")] = press
#    print(data)
 #   print(dataInv)
  #  print(recieved)
    global offeredTimes
    offeredTimes = [offerSlice(dataInv, header)]
    global slicetext
    slicetext = ";" + str(offeredTimes[0])
    global delete
    delete = True
    global fig

    Plots.plots(dataInv, header, fig, offeredTimes)
    fig = Plots.makePlot(header, "current", dataInv, fig, offeredTimes, False)


def phase2():
    # SETUP SCREEN
    screen.fill(black)
    screen.blit(background2, background_Rect2)

    # TIMESTAMP
    time2 = dataInv[header.index("time")][0]
    if test:
        time2 = 311299235959.0
    time2 = str(int(time2))
    day2 = time2[0:2]+"."+time2[2:4]+".20"+time2[4:6]
    hour2 = time2[6:8]+":"+time2[8:10]
    sec2 = time2[10:12]

    Text_day2 = Font_RT.render(day2, True, black)
    Text_day2_Rect = Text_day2.get_rect(center=(515, 42))
    
    Text_hour2 = Font_RT.render(hour2, True, black)
    Text_hour2_Rect = Text_hour2.get_rect(center=(440, 42))

    Text_sec2 = Font_RT.render(sec2, True, black)
    Text_sec2_Rect = Text_sec2.get_rect(center=(470, 42))

    # GPS STAMP
    try:
        lat = dataInv[header.index("lat")][0]
        long = dataInv[header.index("long")][0]
    except:
        lat = dataInv[header.index("latitude")][0]
        long = dataInv[header.index("longitude")][0]
    
    degN = int(lat)
    minN = int((lat-degN)*60)
    vteN = round(((lat-degN)*60-minN)*60, 1)

    degE = int(long)
    minE = int((long-degE)*60)
    vteE = round(((long-degE)*60-minE)*60, 1)

    Text_degN = Font_RT.render(str(round(degN)), True, black)
    Text_degN_Rect = Text_degN.get_rect(center=(437, 55))

    Text_minN = Font_RT.render(str(round(minN)), True, black)
    Text_minN_Rect = Text_minN.get_rect(center=(453, 55))

    Text_vteN = Font_RT.render(str(round(vteN, 1)), True, black)
    Text_vteN_Rect = Text_vteN.get_rect(center=(471, 55))

    Text_degE = Font_RT.render(str(round(degE)), True, black)
    Text_degE_Rect = Text_degE.get_rect(center=(501, 55))

    Text_minE = Font_RT.render(str(round(minE)), True, black)
    Text_minE_Rect = Text_minE.get_rect(center=(517, 55))

    Text_vteE = Font_RT.render(str(round(vteE, 1)), True, black)
    Text_vteE_Rect = Text_vteE.get_rect(center=(535, 55))

    Text_slice = Font.render(slicetext, True, black)
    Text_slice_Rect = Text_slice.get_rect(center=(434, 82))
    
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

    screen.blits(blit_sequence=((Text_day2, Text_day2_Rect),
                                (Text_hour2, Text_hour2_Rect),
                                (Text_sec2, Text_sec2_Rect),
                                (Text_degN, Text_degN_Rect),
                                (Text_minN, Text_minN_Rect),
                                (Text_vteN, Text_vteN_Rect),
                                (Text_degE, Text_degE_Rect),
                                (Text_minE, Text_minE_Rect),
                                (Text_vteE, Text_vteE_Rect),
                                (Text_slice, Text_slice_Rect)))

    pg.display.flip()

# PHASE III
def ToPhaseIII():
    global Results
    global UpDown
    Results, UpDown = Index.MakeIndex(dataInv, header)


def phase3():
    screen.fill(black)
    screen.blit(background3, background3_Rect)

    index, o2, co2, lightintenzity, humidity, temperature, pressure = Results

    # TEXTS
    Text_Index = Font_Index.render(str(round(index, 1)), True, black)
    Text_Index_Rect = Text_Index.get_rect(center=(400, 214))

    Text_o2 = Font_Var.render(str(round(o2)), True, black)
    Text_o2_Rect = Text_o2.get_rect(center=(49, 248))

    Text_co2 = Font_Var.render(str(round(co2)), True, black)
    Text_co2_Rect = Text_co2.get_rect(center=(663, 248))

    Text_lightintenzity = Font_Var.render(str(round(lightintenzity)), True, black)
    Text_lightintenzity_Rect = Text_lightintenzity.get_rect(center=(580, 248))

    Text_humidity = Font_Var.render(str(round(humidity)), True, black)
    Text_humidity_Rect = Text_humidity.get_rect(center=(133, 248))
    
    Text_temperature = Font_Var.render(str(round(temperature)), True, black)
    Text_temperature_Rect = Text_temperature.get_rect(center=(215, 248))

    Text_pressure = Font_Var.render(str(round(pressure)), True, black)
    Text_pressure_Rect = Text_pressure.get_rect(center=(746, 248))

    screen.blits(blit_sequence=((Text_Index, Text_Index_Rect),
                                (Text_o2, Text_o2_Rect),
                                (Text_co2, Text_co2_Rect),
                                (Text_lightintenzity, Text_lightintenzity_Rect),
                                (Text_humidity, Text_humidity_Rect),
                                (Text_temperature, Text_temperature_Rect),
                                (Text_pressure, Text_pressure_Rect)))

    if UpDown[0] == 1:
        screen.blit(Up3, Up3_O2_Rect)
    elif UpDown[0] == -1:
        screen.blit(Down3, Down3_O2_Rect)

    if UpDown[1] == 1:
        screen.blit(Up3, Up3_CO2_Rect)
    elif UpDown[1] == -1:
        screen.blit(Down3, Down3_CO2_Rect)

    if UpDown[2] == 1:
        screen.blit(Up3, Up3_Int_Rect)
    elif UpDown[2] == -1:
        screen.blit(Down3, Down3_Int_Rect)

    if UpDown[3] == 1:
        screen.blit(Up3, Up3_Hum_Rect)
    elif UpDown[3] == -1:
        screen.blit(Down3, Down3_Hum_Rect)

    if UpDown[4] == 1:
        screen.blit(Up3, Up3_Temp_Rect)
    elif UpDown[4] == -1:
        screen.blit(Down3, Down3_Temp_Rect)

    if UpDown[5] == 1:
        screen.blit(Up3, Up3_Press_Rect)
    elif UpDown[5] == -1:
        screen.blit(Down3, Down3_Press_Rect)

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
            
            elif event.__dict__["unicode"] == "\x1b":
                cam.stop()
                pg.quit()
                sys.exit()
                quit()
            elif event.__dict__["unicode"] == "\x08":
                if phase == 1:
                    sent = sent[0:-1]
                elif phase == 2:
                    if delete:
                        slicetext = ""
                        delete = False
                    else:
                        slicetext = slicetext[0:-1]
                    show = True
            elif event.__dict__["unicode"] == '\r':
                if phase == 1:
                    try:
                        fh = open(command_path, "w")
                        fh.write(sent)
                        fh.close()
                    except:
                        sent = "error: cannot send"
                elif phase == 2:
                    if slicetext == "" or slicetext == ";":
                        continue
                    if not ";" in slicetext:
                        slicetext = slicetext + ";"
                    slicetextList = slicetext.split(";")
                    try:
                        if slicetextList[0] == "":
                            slicetextList[0] = dataInv[header.index("time")][0]
                        else:
                            slicetextList[0] = float(slicetextList[0])
                    except:
                        slicetext = "Not a number"
                        delete = True
                        show = True
                        continue
                    try:
                        if slicetextList[1] == "":
                            slicetextList[1] = dataInv[header.index("time")][-1]
                        else:
                            slicetextList[1] = float(slicetextList[1])
                    except:
                        slicetext = "Not a number"
                        delete = True
                        show = True
                        continue
                    if not show:
                        dataInv = Plots.sliceData(slicetextList[0], slicetextList[1], dataInv, header, fig)
                        slicetext = ""
                        show = True
                    else:
                        offeredTimes = slicetextList
                        Plots.plots(dataInv, header, fig, offeredTimes)
                        show = False
            else:
                if phase == 1:
                    sent = sent + event.__dict__["unicode"]
                elif phase == 2:
                    if delete:
                        slicetext = ""
                        delete = False
                    slicetext = slicetext + event.__dict__["unicode"]
                    show = True

        if event.type == pg.MOUSEBUTTONDOWN and phase == 2:
            if (event.__dict__["pos"][0] > o2_center[0]-(plotsize[0]/2)) and (event.__dict__["pos"][0] < o2_center[0]+(plotsize[0]/2)) and (
                event.__dict__["pos"][1] > o2_center[1]-(plotsize[1]/2)) and (event.__dict__["pos"][1] < o2_center[1]+(plotsize[1]/2)):
                fig = Plots.makePlot(header, "oxygen", dataInv, fig, offeredTimes)
                
            elif (event.__dict__["pos"][0] > co2_center[0]-(plotsize[0]/2)) and (event.__dict__["pos"][0] < co2_center[0]+(plotsize[0]/2)) and (
                event.__dict__["pos"][1] > co2_center[1]-(plotsize[1]/2)) and (event.__dict__["pos"][1] < co2_center[1]+(plotsize[1]/2)):
                fig = Plots.makePlot(header, "co2", dataInv, fig, offeredTimes)
                
            elif (event.__dict__["pos"][0] > lightIntensity_center[0]-(plotsize[0]/2)) and (event.__dict__["pos"][0] < lightIntensity_center[0]+(plotsize[0]/2)) and (
                event.__dict__["pos"][1] > lightIntensity_center[1]-(plotsize[1]/2)) and (event.__dict__["pos"][1] < lightIntensity_center[1]+(plotsize[1]/2)):
                fig = Plots.makePlot(header, "lightIntensity", dataInv, fig, offeredTimes)
                
            elif (event.__dict__["pos"][0] > pressure_center[0]-(plotsize[0]/2)) and (event.__dict__["pos"][0] < pressure_center[0]+(plotsize[0]/2)) and (
                event.__dict__["pos"][1] > pressure_center[1]-(plotsize[1]/2)) and (event.__dict__["pos"][1] < pressure_center[1]+(plotsize[1]/2)):
                fig = Plots.makePlot(header, "pressure", dataInv, fig, offeredTimes)
                
            elif (event.__dict__["pos"][0] > specter_center[0]-(plotsize[0]/2)) and (event.__dict__["pos"][0] < specter_center[0]+(plotsize[0]/2)) and (
                event.__dict__["pos"][1] > specter_center[1]-(plotsize[1]/2)) and (event.__dict__["pos"][1] < specter_center[1]+(plotsize[1]/2)):
                fig = Plots.makePlot(header, "specter", dataInv, fig, offeredTimes)
                
            elif (event.__dict__["pos"][0] > temperature_center[0]-(plotsize[0]/2)) and (event.__dict__["pos"][0] < temperature_center[0]+(plotsize[0]/2)) and (
                event.__dict__["pos"][1] > temperature_center[1]-(plotsize[1]/2)) and (event.__dict__["pos"][1] < temperature_center[1]+(plotsize[1]/2)):
                fig = Plots.makePlot(header, "temperature", dataInv, fig, offeredTimes)

            elif (event.__dict__["pos"][0] > 507) and (event.__dict__["pos"][0] < 525) and (
                event.__dict__["pos"][1] > 71) and (event.__dict__["pos"][1] < 90):
                ToPhaseII()

            elif (event.__dict__["pos"][0] > 765) and (
                event.__dict__["pos"][1] < 34):
                phase = 3
                ToPhaseIII()
            
    if (phase == 1):
        phase1()
    elif phase == 2:
        phase2()
    elif phase == 3:
        phase3()
