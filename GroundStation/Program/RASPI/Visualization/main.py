import pygame as pg
import sys
import matplotlib as mpl
import math
from Model import Valec
import time

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
flags = pg.FULLSCREEN
size = (800, 480)
screen = pg.display.set_mode(size, flags)
pg.font.init()

phase = 1
black = (0,0,0)

# LOAD FONT
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

# TEST
#j = 0
#dolu0 = False
#dolu1 = False
#data = []
#for i in range(20):
#    data.append(0)

# SET PRE TEXT VALUE
line_pre = None

# PHASE I
def phase1():
    # TODO podminka, že mám nová data
    global line_pre
    global recieve

    try:
        fh = open(path, "r")
        line = fh.readline()
        fh.close()
    except:
        print("chyba cteni souboru")
        return

    if (line_pre != None) and (line == line_pre):
        #print("data shodna s predchozimi")
        return

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
        
#    global data
    
    global press_pre
    global temp_pre
    
    # TEST
#    global dolu0
#    global dolu1
#    global j
    
#    for i in range(len(data)):
#        data[i] += 0.1
#    if dolu0:
#        data[0] -= 0.2
#    if data[0] > 120:
#        dolu0 = True
#    if data[0] < 60:
#        dolu0 = False
#    if dolu1:
#        data[1] -= 0.2
#    if data[1] > 65:
#        dolu1 = True
#    if data[1] < -10:
#        dolu1 = False
#    data[2] += 0.9
#    if data[2] > 1100:
#        data[2] = 300
#    data[3] -= 0.09
#    if data[3] > 22:
#        data[3] = 18
#    if data[4] > 1100:
#        data[4] = -100
#    
#    if data[5] > 100:
#        data[5] = 0

#    j += 1
#    if j%1000 <500:
#        for i in range(8):
#            data[6+i] = 1
#    else:
#        for i in range(8):
#            data[6+i] = 0

    # TODO PARSE AND ASING DATA
    try:
        press = data[0]
    
        temp = data[1]

        co2 = data[2]

        o2 = data[3]

        height = data[4]

        speed = data[5]

        dORa0 = data[6]
        dORa1 = data[7]
        dORa2 = data[8]
        dORa3 = data[9]
        dORa4 = data[10]
        dORa5 = data[11]
        dORa6 = data[12]
        dORa7 = data[13]

        humidity = data[14]

        transfer = data[15]

        battery = data[16]

        roll = data[17]
        pitch = data[18]
        yaw = data[19]
    
    except:
        screen.fill(black)
        screen.blit(background, background_Rect)
        Text_Recieve = Font.render(recieve, True, black)
        Text_Recieve_Rect = Text_Recieve.get_rect(center=(128,438))
        screen.blit(Text_Recieve, Text_Recieve_Rect)
        return
    
    
#    roll -=0.05
#    pitch -=0.05
#    yaw -=0.05

    # CALCULATE LAND IN
    if speed != 0:
        landIn = height/speed
    else:
        landIn = 0

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
        

    # TEXTS
    Text_Press = Font.render(str(round(press, 1)), True, black)
    Text_Press_Rect = Text_Press.get_rect(center=(306,142))

    Text_Temp = Font.render(str(round(temp, 1)), True, black)
    Text_Temp_Rect = Text_Temp.get_rect(center=(479,142))

    Text_O2 = Font.render(str(round(o2, 1)), True, black)
    Text_O2_Rect = Text_O2.get_rect(center=(482,274))

    Text_CO2 = Font.render(str(round(co2)), True, black)
    Text_CO2_Rect = Text_CO2.get_rect(center=(306,274))

    Text_Height = Font.render(str(round(height)), True, black)
    Text_Height_Rect = Text_Height.get_rect(center=(658,350))

    Text_Speed = Font.render(str(round(speed)), True, black)
    Text_Speed_Rect = Text_Speed.get_rect(center=(654,369))

    Text_LandIn = Font.render(str(round(landIn, 1)), True, black)
    Text_LandIn_Rect = Text_LandIn.get_rect(center=(657,388))

    Text_RT = Font_RT.render(r"00:00:00    00.00.0000", True, black)
    Text_RT_Rect = Text_RT.get_rect(center=(650, 409))

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
    
    
    # SHOW TEXTS AND DIALS
    screen.blits(blit_sequence=((Text_Press, Text_Press_Rect),
                                (Text_Temp, Text_Temp_Rect),
                                (Text_O2, Text_O2_Rect),
                                (Text_CO2, Text_CO2_Rect),
                                (Text_Height, Text_Height_Rect),
                                (Text_Speed, Text_Speed_Rect),
                                (Text_LandIn, Text_LandIn_Rect),
                                (Text_RT, Text_RT_Rect),
                                (Text_Transfer, Text_Transfer_Rect),
                                (Text_Battery, Text_Battery_Rect),
                                (Text_Sent, Text_Sent_Rect),
                                (Text_Recieve, Text_Recieve_Rect),
                                (Text_Humidity, Text_Humidity_Rect)))

    screen.blits(blit_sequence=((RucickaTemp2, RucickaTemp_Rect2),
                                (RucickaPress2, RucickaPress_Rect2),
                                (RucickaCO22, RucickaCO2_Rect2),
                                (RucickaO22, RucickaO2_Rect2),
                                (RucickaHeight, RucickaHeight_Rect2)))
    pg.display.flip()


    
# MAIN
while True:
    for event in pg.event.get():
        if event.type == pg.QUIT:
            pg.quit()
            sys.exit()
            quit()
        if event.type == pg.KEYDOWN:
            if event.__dict__["unicode"] == "\x08":
                sent = sent[0:-1]
            elif event.__dict__["unicode"] == '\r':
                fh = open(command_path, "w")
                fh.write(sent)
                fh.close()
            else:
                sent = sent + event.__dict__["unicode"]
            
    if (phase == 1):
        phase1()
    elif (phase == 0):
        break