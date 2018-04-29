import serial
import time
import datetime
mstime = 0;
stime = 0;
mintime = 0;
hourtime = 0;
timestamp = ""

mph = 100
# Open the Serial Port
ser = serial.Serial(port = "COM42", baudrate=115200, parity=serial.PARITY_EVEN)

with open("newer_test_data.csv") as f:
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
        if hourtime == 0:
            timestamp = "00"
        elif hourtime < 10:
            timestmap = "0" + str(hourtime)
        else:
            timestamp = str(hourtime)

        if mintime == 0:
            timestamp = timestamp + "00"
        elif mintime < 10:
            timestamp = timestamp + "0" + str(mintime)
        else:
            timestamp = timestamp + str(mintime)

        if stime == 0:
            timestamp = timestamp + "00"
        elif stime < 10:
            timestamp = timestamp + "0" + str(stime)
        else:
            timestamp = timestamp + str(stime)

        if mstime == 0:
            timestamp = timestamp + "000"
        else:
            timestamp = timestamp + str(mstime)

        line = line.replace("!\n","") + "AC," + timestamp + ",AR," + str(mph) + ",!\n"
        ser.write(line.encode("utf-8"))
        mstime+=100
        timestamp = ""

        if mstime == 1000:
            mstime = 0
            stime+=1
        if stime == 60:
            stime = 0
            mintime+=1
        if mintime == 60:
            mintime = 0
            htime+=1
        if hourtime == 24:
            hourtime = 0

        mph += 5
        mph = mph%200
        #print()
        print(line)
        #ser.write(line.replace("\n","") + ",!\n")
        time.sleep(0.5)
