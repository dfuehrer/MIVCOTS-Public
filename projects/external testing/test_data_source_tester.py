import serial
import time

# Open the Serial Port
ser = serial.Serial(port = "COM43", baudrate=115200)

while(True):
    if(ser.in_waiting > 0):
        print(ser.readline().split(','))
        #print(ser.readline().decode("utf-8").split(','))
