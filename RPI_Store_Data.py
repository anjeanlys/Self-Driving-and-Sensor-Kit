import serial
import csv

ser = serial.Serial(port='/dev/ttyACM0',
                    baudrate=9600,
                    parity=serial.PARITY_NONE,
                    stopbits=serial.STOPBITS_ONE,
                    bytesize=serial.EIGHTBITS,
                    timeout=0.5)

with open("data.csv", "w") as new_file:
    csv_writer = csv.writer(new_file)
csv_writer.writerow(["x", "y", "z", "temp1", "temp2", "temp3", "rpm"])

while True:
    if (ser.in_waiting > 0):
        line = ser.readline().strip().decode('utf-8')
        x = int(line[0:3])
        y = int(line[3:6])
        z = int(line[6:9])
        temp1 = int(line[9:13]) / 10
        temp2 = int(line[13:17]) / 10
        temp3 = int(line[17:21]) / 10
        rpm = int(line[21:25])
        # print("T1=" + temp1 + ", T2=" + temp2 + ", T3=" + temp3 + ", rpm=" + rpm)
        #updated in pycharm cause RPi editor is dog
        csv_writer.writerow([x, y, z, temp1, temp2, temp3, rpm])
