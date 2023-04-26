import math

class Valec:
    def __init__(self, r, v, center_x, center_y, num_of_points):
        self.center = (center_x, center_y)
        self.points_down = []
        self.points_up = []
        self.dist = math.sqrt(r*r+v*v/4)
        self.num_of_points = num_of_points
        self.up_center = (0, v/2, 0)

        for i in range(num_of_points):
            angle = 2*math.pi*i/num_of_points
            x = r*math.sin(angle)
            z = r*math.cos(angle)
            z = round(z, 2)
            x = round(x, 2)
            if x <= 0 and x > -0.001:
                x = 0
            if z <= 0 and z > -0.001:
                z = 0
            self.points_down.append((x, -v/2, z))
            self.points_up.append((x, round(v/2, 2), z))

    def move_point2D(self, x, y, angle):
        if y == 0:
            phi0 = math.pi/2
        else:
            phi0 = math.atan(x/y)
        if y < 0 or ((y == 0) and x<0):
            phi0 += math.pi
        move_angle = angle + phi0

        lenght = math.sqrt(x*x+y*y)
        x = lenght*math.sin(move_angle)
        y = lenght*math.cos(move_angle)
        return (x, y)
        

    def project(self, roll, pitch, yaw):
        rects = []
        movedpoints_down = []
        movedpoints_up = []
        moved_center_up = [self.up_center[1] * math.cos(pitch)*math.sin(yaw),self.up_center[1] * math.cos(pitch)*math.cos(yaw), -self.up_center[1] * math.sin(pitch)] 
        for i in range(len(self.points_up)):
            movedpoints_up.append([])
            movedpoints_down.append([])

        for i in range(int(len(self.points_up)/2)):
            point = self.points_up[i]
            x, y = self.move_point2D(point[0], point[1], roll)
            new_point = [x, point[1], z]
            y, z = self.move_point2D(new_point[1], new_point[2], pitch)
            new_point = [new_point[0], y, z]
            x, z = self.move_point2D(new_point[0], new_point[2], yaw)
            new_point = [x, y, new_point[2]]
            movedpoints_up[i] = (new_point[0] + self.center[0], new_point[1] + self.center[1], new_point[2])
            j = int(i + self.num_of_points/2)
            movedpoints_down[j] = (self.center[0] - new_point[0], -new_point[1] + self.center[1], -new_point[2])

            new_point = [2*moved_center_up[0] - new_point[0], 2*moved_center_up[1] - new_point[1], 2*moved_center_up[2] - new_point[2]]
            movedpoints_up[j] = (new_point[0] + self.center[0], new_point[1] + self.center[1], new_point[2])
            movedpoints_down[i] = (self.center[0] - new_point[0], -new_point[1] + self.center[1], -new_point[2])
#            point = self.points_down[i]
 #           x, z = self.move_point2D(point[0], point[2], roll)
  #          new_point = [x, point[1], z]
   #         y, z = self.move_point2D(new_point[1], new_point[2], pitch)
    #        new_point = [new_point[0], y, z]
     #       x, y = self.move_point2D(new_point[0], new_point[1], yaw)
      #      new_point = [x, y, new_point[2]]
       #     movedpoints_down.append((new_point[0] + self.center[0], new_point[1] + self.center[1], new_point[2]))
            
        show_up = False
        show_down = False

        max_up = -self.dist
        max_down = -self.dist
        min_down = self.dist
        i_min = 0
        for i in range(len(movedpoints_up)):
            if max_up < movedpoints_up[i][2]:
                max_up = movedpoints_up[i][2]
            if max_down < movedpoints_down[i][2]:
                max_down = movedpoints_down[i][2]
            if min_down > movedpoints_down[i][2]:
                min_down = movedpoints_down[i][2]
                i_min = i
            
        if max_up > max_down:
            show_up = True
        elif max_up < max_down:
            show_down = True

        test = False
        if test:
            i = 3
            j = 0
            rects.append((movedpoints_down[i][0:2], movedpoints_down[j][0:2], movedpoints_up[j][0:2], movedpoints_up[i][0:2]))
            return rects

        i = i_min
        j = i_min-1
        if j < 0:
            j = len(movedpoints_down)-1
        rects.append((movedpoints_down[i][0:2], movedpoints_down[j][0:2], movedpoints_up[j][0:2], movedpoints_up[i][0:2]))
        while True:
            l = i + 1
            if l >= len(movedpoints_down):
                l = 0
            rects.append((movedpoints_down[l][0:2], movedpoints_down[i][0:2], movedpoints_up[i][0:2], movedpoints_up[l][0:2]))
            if l == j:
                break
            i = l

            k = j - 1
            if k < 0:
                k = len(movedpoints_down)-1
            rects.append((movedpoints_down[k][0:2], movedpoints_down[j][0:2], movedpoints_up[j][0:2], movedpoints_up[k][0:2]))
            if k == i:
                break
            j = k


        if show_up:
            base = []
            for point in movedpoints_up:
                base.append(point[0:2])
            rects.append(base)

        if show_down:
            base = []
            for point in movedpoints_down:
                base.append(point[0:2])
            rects.append(base)

        return rects
