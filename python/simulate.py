#!/usr/bin/python3

import random
import time

def find_latest_timestamp(file_path):
    with open(file_path, "r") as f:
        last_line = f.readlines()[-1]
        return int(float(last_line.split(",")[0]))
    return None

file_name = "session-20160224-224117.csv"
latest_timestamp = find_latest_timestamp(file_name)

while True:
    with open(file_name, "a") as f:
        f.write("{},{},{}\n".format(latest_timestamp + 1, random.randint(70,120), "Polar"))
        f.write("{},{},{}\n".format(latest_timestamp + 1, random.randint(70,120), "PulseSensor"))

    time.sleep(1)
