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
import datetime
timeStart = 0;
mph = 100
# Open the Serial Port
ser = serial.Serial(port = "COM42", baudrate=115200)

with open("new_test_data.csv") as f:
    data = f.readlines()

# line = data[0]
# line = line.replace("!\n","") + "AC," + str(timeStart) + ",AR," + str(mph) + ",!\n"
# print("Sending test packet at: " + str(datetime.datetime.now()))
# ser.write(line.encode("utf-8"))
# print("Sent!")

#time.sleep(200)
	
while 1:
    for line in data:
        #print("#," + line.replace("\n","") + ",!\n")
        #line = "#," + line.replace("\n","") + ",!\n"
        #line += "AC
        line = line.replace("!\n","") + "AC," + str(timeStart) + ",AR," + str(mph) + ",!\n"
        ser.write(line.encode("utf-8"))
        timeStart +=10
        mph += 5
        mph = mph%200
        #print()
        print(line)
        #ser.write(line.replace("\n","") + ",!\n")
        time.sleep(0.5)