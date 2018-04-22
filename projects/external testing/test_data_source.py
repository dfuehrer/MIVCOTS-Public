# import serial
# import time
#
# # Open the Serial Port
# ser = serial.Serial(port = "COM42", baudrate=115200)
#
# with open("November14Sorted.csv") as f:
#     data = f.readlines()
#
#
# while 1:
#     for line in data:
#         print(line.replace("\n","") + ",!\n")
#         line = line.replace("\n","") + ",!\n"
#         ser.write(line.encode("utf-8"))
#         #ser.write(line.replace("\n","") + ",!\n")
#         time.sleep(1)

import serial
import time
timeStart = 0;
# Open the Serial Port
ser = serial.Serial(port = "COM42", baudrate=115200)

with open("new_test_data.csv") as f:
    data = f.readlines()


while 1:
    for line in data:
        #print("#," + line.replace("\n","") + ",!\n")
        #line = "#," + line.replace("\n","") + ",!\n"
        #line += "AC
        line = line.replace("!\n","") + "AC," + str(timeStart) + ",!\n"
        ser.write(line.encode("utf-8"))
        timeStart +=1;
        #print()
        print(line)
        #ser.write(line.replace("\n","") + ",!\n")
        time.sleep(0.5)
