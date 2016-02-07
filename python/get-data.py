#!/usr/bin/python3

import re
import serial
import serial.tools.list_ports
import sys
import time

def find_serial():
    while True:
        serial=detect_serial()

        if serial != None:
            return serial

def detect_serial():
    for port in serial.tools.list_ports.comports():
        if port[2] == "USB VID:PID=2a03:0043 SNR=85431303636351A0D0E0":
            return port[0]

    return None

def write_informations(last_informations, time_stamp, output):
    line = ""
    keys=list(last_informations.keys())
    keys.sort()
    
    line=str(time_stamp)
    for key in keys:
        line += "," + last_informations[key][1]

    line += "\n"
    output.write(line)
    output.flush()

def main():
    serialPort=find_serial()
    time.sleep(2)
    print("Found:",serialPort)
    ser = serial.Serial(serialPort)

    file_name=time.strftime("%Y%m%d-%H%M%S.csv")

    output=open(file_name,"a")

    last_informations={}

    start_time=time.time()

    while True:
        t=ser.readline()
        t = t.rstrip()
        t = t.decode()

        print(t)

        m=re.search("^(.*): ([0-9]+)bpm$", t)
        if m:
            name=m.group(1)
            bpm=m.group(2)

            if name not in ['Polar', 'PulseSensor']:
                continue

            if not bpm.isdigit():
                continue

            time_stamp=time.time()-start_time
            last_informations[name] = (time_stamp, bpm)

            write_informations(last_informations, time_stamp, output)

if __name__ == "__main__":
    main()
