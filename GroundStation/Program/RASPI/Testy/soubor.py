# Using readline()
file1 = open('/home/pi/Desktop/received_data.txt', 'r')
count = 0
 
while True:
    count += 1
 
    # Get next line from file
    line = file1.readline()
 
    # if line is empty
    # end of file is reached
    if not line:
        break
    print("{}".format(count, line.strip()))

    string = str(line, "utf-8")
    print(string)
    
 
file1.close()
