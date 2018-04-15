import serial
import time

# Open the Serial Port
ser = serial.Serial(port = "COM42", baudrate=115200)

with open("November14Sorted.csv") as f:
    data = f.readlines()


while 1:
    for line in data:
        print(line.replace("\n","") + ",!\n")
        #ser.write(line.encode("utf-8"))
        ser.write(line.replace("\n","") + ",!\n")
        time.sleep(1)
