import pygame as pg
import sys
import math

# SETUP
# LOAD IMAGES
Ruzice3 = pg.image.load("images\Ruzice2.jpg")
Ruzice3_Rect = Ruzice3.get_rect(center=(1820, 100))

Ruzice1 = pg.image.load("images\LongXHeight.png")
Ruzice1_Rect = Ruzice1.get_rect(center=(700, 80))

Ruzice2 = pg.image.load("images\LatXHeight.png")
Ruzice2_Rect = Ruzice2.get_rect(center=(700, 620))

# LOAD DATA
data_path = "Backup-data.txt"#"/home/pi/Desktop/Backup-data.txt"

data = []
try:
    fh = open(data_path, "r")
except:
    print("chyba cteni souboru 2")
    quit()
text = fh.read()
text = text.strip().split("\n")
header = text[0].strip().split(';')
text = text[1:]
for line in text:
    nan = False
    strline = line.strip().split(';')
    intline = []
    for string in strline:
        if string == "":
            intline.append(0.0)
        else:
            try:
                num = float(string)
            except:
                nan = True
                break
            if num != num:
                nan = True
                break
            intline.append(num)
    if nan:
        continue
    data.append(intline)

fh.close()

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

lat = dataInv[header.index("lat")]
long = dataInv[header.index("long")]
height = dataInv[header.index("height")]

# FIND PEAK
start = None
end = None
for i in range(len(height)-10):
    if (start == None) and (height[i+10] > height[i] + 10):
        start = i
        continue
    elif (end == None) and (start != None):
        if (abs(height[i] - height[i+10]) < 1) and (abs(height[i] - height[i+5]) < 1):
            end = i

lat = lat[start:end+1]
long = long[start:end+1]
height = height[start:end+1]

# TEST
test = False
if test:
    import random as rd
    for i in range(len(height)):
        lat[i] = ((rd.randrange(100)+492300)/10000)
        long[i] = ((rd.randrange(100)+165400)/10000)

# RECALCULATE LAT AND LONG
Lat0 = min(lat)
Long0 = min(long)
LongCircle = 2*math.pi*6378135*math.cos(Lat0*math.pi/180)
LatCircle = 2*math.pi*6378135
for i in range(len(lat)):
    lat[i] = LatCircle*(lat[i]-Lat0)/360
    long[i] = LongCircle*(long[i]-Long0)/360

print(lat)
print(long)
print(height)

# SET ANCHORPOINTS
latUP = (220, 40)
latDOWN = (660, 480)
longUP = (220, 580)
longDOWN = (660, 1020)
heightUP = (820, 40)
heightDOWN = (1800, 1020)

# COMPUTE SCREEN VIZUALISATION RATIOS
H_ratio = (latDOWN[1]-latUP[1])/(max(height)-min(height))
L1_ratio = (latDOWN[0]-latUP[0])/(max(lat+long)-min(lat+long))
L2_ratio = (heightDOWN[0]-heightUP[0])/(max(lat+long)-min(lat+long))

# CONVERT DATA
LatXLong = []
LatXHeight = []
LongXHeight = []
for i in range(len(lat)):
    LatXLong.append((long[i]*L2_ratio + heightUP[0], heightDOWN[1] - lat[i]*L2_ratio))
    LatXHeight.append((lat[i]*L1_ratio+longUP[0], longDOWN[1]-height[i]*H_ratio))
    LongXHeight.append((long[i]*L1_ratio+latUP[0], latDOWN[1]-height[i]*H_ratio))

# SETUP SCREEN
flags = pg.FULLSCREEN | pg.SHOWN
size = (1920, 1080)
screen = pg.display.set_mode(size, flags)

white = (255,255,255)
black = (0,0,0)

# LOAD FONT
pg.font.init()
Font = pg.font.Font(r"font/myriad-pro/MyriadPro-Light.otf", 20)

# SETUP MOVEMENT SHOWER
i = -1
time = 300
stop = False

# MAIN
def Vizu():
    # MOVE ITERABLE
    global i
    if not stop:
        i += 1
        if i == len(LatXLong):
            i = 0

    # SETUP SCREEN
    screen.fill(white)
    screen.blits(blit_sequence=((Ruzice1, Ruzice1_Rect),
                                (Ruzice2, Ruzice2_Rect),
                                (Ruzice3, Ruzice3_Rect)))
    
    # TRAJECTORY
    pg.draw.lines(screen, black, False, LatXLong)
    pg.draw.lines(screen, black, False, LatXHeight)
    pg.draw.lines(screen, black, False, LongXHeight)
    
    pg.draw.circle(screen, (0,255,0), LatXLong[0], 10)
    pg.draw.circle(screen, (0,255,0), LatXHeight[0], 10)
    pg.draw.circle(screen, (0,255,0), LongXHeight[0], 10)
    
    pg.draw.circle(screen, (255,0,0), LatXLong[-1], 10)
    pg.draw.circle(screen, (255,0,0), LatXHeight[-1], 10)
    pg.draw.circle(screen, (255,0,0), LongXHeight[-1], 10)
    
    pg.draw.circle(screen, (0,0,255), LatXLong[i], 10)
    pg.draw.circle(screen, (0,0,255), LatXHeight[i], 10)
    pg.draw.circle(screen, (0,0,255), LongXHeight[i], 10)

    # PLOT AXES
    pg.draw.lines(screen, black, False, (latUP, (latUP[0], latDOWN[1]), latDOWN), 3)
    pg.draw.lines(screen, black, False, (longUP, (longUP[0], longDOWN[1]), longDOWN), 3)
    pg.draw.lines(screen, black, False, (heightUP, (heightUP[0], heightDOWN[1]), heightDOWN), 3)

    # WRITE CURRENT NUMBERS
    Text_HeightNOW = Font.render(str(int(height[i])), True, black)
    Text_HeightNOW_Rect1 = Text_HeightNOW.get_rect(center=(latUP[0]-30, LongXHeight[i][1]))
    Text_HeightNOW_Rect2 = Text_HeightNOW.get_rect(center=(latUP[0]-30, LatXHeight[i][1]))
    
    Text_LatNOW = Font.render(str(int(lat[i])), True, black)
    Text_LatNOW_Rect3 = Text_LatNOW.get_rect(center=(LatXHeight[i][0], longDOWN[1]+20))
    Text_LatNOW_Rect2 = Text_LatNOW.get_rect(center=(heightUP[0]-30, LatXLong[i][1]))
    
    Text_LongNOW = Font.render(str(int(long[i])), True, black)
    Text_LongNOW_Rect1 = Text_LongNOW.get_rect(center=(LongXHeight[i][0], latDOWN[1]+20))
    Text_LongNOW_Rect3 = Text_LongNOW.get_rect(center=(LatXLong[i][0], heightDOWN[1]+20))

    # PLOT TEXTS
    Text_0 = Font.render("0", True, black)
    Text_0_Rect1 = Text_0.get_rect(center=(latUP[0]-10, latDOWN[1]+15))
    Text_0_Rect2 = Text_0.get_rect(center=(longUP[0]-10, longDOWN[1]+15))
    Text_0_Rect3 = Text_0.get_rect(center=(heightUP[0]-10, heightDOWN[1]+15))

    Text_Lat = Font.render(r"distance/m", True, black)
    Text_Lat_Rect2 = Text_Lat.get_rect(center=(longDOWN[0]+50, longDOWN[1]+15))
    Text_Lat_Rect3 = Text_Lat.get_rect(center=(heightUP[0]-20, heightUP[1]-15))

    Text_Long = Font.render(r"distance/m", True, black)
    Text_Long_Rect1 = Text_Long.get_rect(center=(latDOWN[0]+50, latDOWN[1]+15))
    Text_Long_Rect3 = Text_Long.get_rect(center=(heightDOWN[0]+50, heightDOWN[1]+15))

    Text_Height = Font.render(r"height/m", True, black)
    Text_Height_Rect1 = Text_Height.get_rect(center=(latUP[0]-20, latUP[1]-15))
    Text_Height_Rect2 = Text_Height.get_rect(center=(longUP[0]-20, longUP[1]-15))

    screen.blits(blit_sequence=((Text_0, Text_0_Rect1),
                                (Text_0, Text_0_Rect2),
                                (Text_0, Text_0_Rect3),
                                (Text_Lat, Text_Lat_Rect2),
                                (Text_Lat, Text_Lat_Rect3),
                                (Text_Long, Text_Long_Rect1),
                                (Text_Long, Text_Long_Rect3),
                                (Text_Height, Text_Height_Rect1),
                                (Text_Height, Text_Height_Rect2),
                                (Text_HeightNOW, Text_HeightNOW_Rect1),
                                (Text_HeightNOW, Text_HeightNOW_Rect2),
                                (Text_LatNOW, Text_LatNOW_Rect2),
                                (Text_LatNOW, Text_LatNOW_Rect3),
                                (Text_LongNOW, Text_LongNOW_Rect1),
                                (Text_LongNOW, Text_LongNOW_Rect3)))

    pg.display.flip()

    # WAIT
    pg.time.delay(time)

# MAIN
while True:
    for event in pg.event.get():
        if event.type == pg.QUIT:
            pg.quit()
            sys.exit()
            quit()
        if event.type == pg.KEYDOWN:
            if event.__dict__["unicode"] == "\x1b":
                pg.quit()
                sys.exit()
                quit()
            elif event.__dict__["scancode"] == 79:
                time -= 100
                if time < 100:
                    time = 100
            elif event.__dict__["scancode"] == 80:
                time += 100
                if time > 1000:
                    time = 1000
            elif event.__dict__["unicode"] == " ":
                stop = not stop

    Vizu()
