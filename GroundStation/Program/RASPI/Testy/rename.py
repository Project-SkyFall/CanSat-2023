import os

# Absolute path of a file
old_name = r"/home/pi/github/CanSat-2023/GroundStation/Program/RASPI/Testy/data.txt"
new_name = r"/home/pi/github/CanSat-2023/GroundStation/Program/RASPI/Testy/data1.txt"

# Renaming the file
os.rename(old_name, new_name)
