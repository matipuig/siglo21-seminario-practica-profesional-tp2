import datetime
import json
import os
import serial
import time

ARDUINO_PORT = "COM3" 
BAUD_RATE = 9600
DATABASE_FILE_PATH = "database.csv"



def get_arduino_serial_connection(arduino_port, baud_rate):
    arduino_serial = serial.Serial(arduino_port, baud_rate, timeout=1)
    time.sleep(2)
    return arduino_serial



def make_database_file_if_not_exists():
    if os.path.exists(DATABASE_FILE_PATH):
        return

    with open(DATABASE_FILE_PATH, 'a') as file:
        file.write("\"datetime\",\"action\",\"people_count\"\n")
    


def add_to_database(action_data):
    date = datetime.datetime.utcnow()
    action = action_data["action"]
    people_count = action_data["count"]

    with open(DATABASE_FILE_PATH, 'a') as file:
        file.write(f"\"{date}\",\"{action}\",{people_count}\n")



def process_arduino_entry(data_line):
    parsed_data = json.loads(data_line)
    action = parsed_data["action"]
    print(f"Received: {data_line}")

    if action == "START":
        print("Arduino started sending information...")
        return
    
    add_to_database(parsed_data)



def listen_arduino_serial(arduino_serial): 
    print("Started reading data from arduino")
    
    try:
        while True:
            if arduino_serial.in_waiting > 0:
                data = arduino_serial.readline().decode('utf-8').rstrip()
                process_arduino_entry(data)

    except KeyboardInterrupt:
        print("Exiting arduino serial...")

    finally:
        arduino_serial.close()


def run_app():
    arduino_serial = get_arduino_serial_connection(ARDUINO_PORT, BAUD_RATE)
    make_database_file_if_not_exists()
    listen_arduino_serial(arduino_serial)


run_app()
