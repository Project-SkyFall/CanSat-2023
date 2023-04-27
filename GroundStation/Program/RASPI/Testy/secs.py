import time

cas = int(time.time())
cycletime = cas

def Cycle():
    global cas
    global cycletime
    if cas + 1 < time.time():
        cycle = cas - cycletime
        #print(cycle)
        #print(int(time.time()))
        cas = int(time.time())
        #print(cycle)
        return int(cycle)
while True:
        cycle = Cycle()
        if cycle != None:
            print(cycle)
    
