import numpy as np
import matplotlib.pyplot as plt
import time
import serial
import csv

# Define variables -------------------------------------------------------

arduino = serial.Serial(port='COM7', baudrate=115200, timeout=.1)
blacklist = [b'', b' ', b'\r\n', b'\n', b'\r', b'0,0\r\n'] # Useless values

#-------------------------------------------------------------------------


# Define functions -------------------------------------------------------

# Read data from serial port
def read_data():

    line = arduino.readline()
    return line


# Read data from serial port
def collect_data(duration = 10, sample_rate = 1000):

    num_samples = duration * sample_rate
    byte_array = np.zeros(num_samples, dtype=bytearray)
    i = 0
    percent = 10

    print("Collecting data...")
    arduino.flush()
    while(i < num_samples):
        data = read_data()
        if data in blacklist:
            continue
        byte_array[i] = data
        i += 1
        if i/num_samples * 100 >= percent:
            print(f"{percent}%")
            percent += 10
    print("Data collection complete")
    return byte_array


# Decode byte arrays to strings and extract data and time
def decode_data(byte_array, num_samples):

    data_array = np.zeros(num_samples)
    time_array = np.zeros(num_samples)
    percent = 10
    print("Processing data...")
    for i in range(num_samples):
        try:
            string = byte_array[i].decode('utf-8').strip()
            line = string.split(",")
            data_array[i] = float(line[0])
            time_array[i] = int(line[1])
        except:
            pass
        if i/num_samples * 100 >= percent:
            print(f"{percent}%")
            percent += 10
    print("Data processing complete")
    return data_array, time_array


# Export data to CSV and plot
def export_and_plot(data_array, time_array, filename='output'):

    print("Exporting data to CSV...")
    with open(f"{filename}.csv", 'w', newline='') as csvfile:
        csv_writer = csv.writer(csvfile)
        csv_writer.writerow(['Timestamp', 'Data'])
        for i in range(len(data_array)):
            csv_writer.writerow([time_array[i], data_array[i]])
    print(f"Data has been successfully exported to {filename}.csv")
    # Plot data
    print("Plotting data...")
    plt.plot(time_array, data_array)
    plt.xlabel('Time [ms]')
    plt.ylabel('Data')
    plt.title('Data from Arduino')
    plt.savefig(f"{filename}.png")

#-------------------------------------------------------------------------
    

# Main program -----------------------------------------------------------
    
duration = int(input("Enter the number of seconds to collect data: "))
sample_rate = 1000
byte_array = collect_data(duration, sample_rate)
data_array, time_array = decode_data(byte_array, duration * sample_rate)
export_and_plot(data_array, time_array)

#--------------------------------------------------------------------------