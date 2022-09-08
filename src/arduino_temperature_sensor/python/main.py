################################
# Adruino temperature sensor kit
################################

 
import serial
import json 
import csv
from datetime import datetime 
import time 


# Global Value 
RUN_RECORD = True
arduino_port = 'COM5'       # change the port based on computer 
baud = 9600 # ardunino uno run at 9600

NOW_DATE = datetime.now().date().strftime("%Y%m%d") 

FLIENAME = NOW_DATE + '_' + "temperatrue_data.csv"
RECODED_DELAY_TIME = 10     # Followed as reqired


# set up the serial to arduino
serial_py = serial.Serial()

if __name__ == "__main__":
    
    with serial_py as ser:

        ser.baudrate = baud
        ser.port = arduino_port
        ser.open()
        print(f"Connected to Arduino port: {arduino_port}")
        

        dict_data = {}
        c = 0
        while c <= 100:
            # flush the adruino memory
            ser.flush()

            try:                    

                c += 1
                log = c

                # read data form Arduino
                data = ser.readline().decode("utf-8")                   # Decoding UTF-8 strings 
                dict_from_json = json.loads(data)
                print(dict_from_json)
                temp = dict_from_json.get("temperature")
                NOW_TIME = datetime.now().time().strftime("%H:%M:%S")   # Get current time 
                
                # store in py dictionary
                if dict_data is not None:
                    dict_data[log] = {}

                    # Time 
                    dict_data[log]["Time"] = NOW_TIME
                    # Temperature
                    dict_data[log]["Temperature"] = temp

            except json.JSONDecodeError as e:
                c -= 1
                print("json:", e)
            time.sleep(RECODED_DELAY_TIME)
        

        print(dict_data)

        #--------------------------------------------------------
        # Write the python dictionary to csv file
        header = list(dict_data[1].keys())
        dict_data_without_log = []
        for k in dict_data.keys():
            dict_data_without_log.append(dict_data[k])

        # only for one level of dictionary
        with open(FLIENAME, 'w+', newline="") as f:
            writer = csv.DictWriter(f, fieldnames=header)
            writer.writeheader()
            writer.writerows(dict_data_without_log)
            print('\nThe csv file is sucessfully exported!!!')
        #--------------------------------------------------------


        # Close the sever
        ser.close()
        print(f"Disconnected to Arduino port: {arduino_port}")

