import matplotlib.pyplot as plt
import numpy as np
import time
import serial
import csv

# Define variables -------------------------------------------------------

arduino = serial.Serial(port='COM3', baudrate=115200, timeout=.1)
blacklist = [b'', b' ', b'\r\n', b'\n', b'\r', b'0,0\r\n'] # Useless values

#-------------------------------------------------------------------------


# Define functions -------------------------------------------------------

# Read data from serial port
def read_data():

    line = arduino.readline()
    return line


# Read data from serial port
def collect_data(duration = 10, sample_rate = 100):

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

    piezo_array = np.zeros(num_samples)
    gyro_array = np.zeros(num_samples)
    time_array = np.zeros(num_samples)
    percent = 10
    print("Processing data...")
    for i in range(num_samples):
        try:
            string = byte_array[i].decode('utf-8').strip()
            line = string.split(",")
            piezo_array[i] = float(line[0])
            gyro_array[i] = float(line[1])
            time_array[i] = int(line[2])
        except:
            pass
        if i/num_samples * 100 >= percent:
            print(f"{percent}%")
            percent += 10
    print("Data processing complete")
    return piezo_array, gyro_array, time_array


# Export data to CSV and plot
def export_and_plot(piezo_array, gyro_array, time_array, filename='output'):

    print("Exporting data to CSV...")
    with open(f"{filename}.csv", 'w', newline='') as csvfile:
        csv_writer = csv.writer(csvfile)
        csv_writer.writerow(['Timestamp', 'Piezo', 'Gyro'])
        for i in range(len(piezo_array)):
            csv_writer.writerow([time_array[i], piezo_array[i], gyro_array[i]])
    print(f"Data has been successfully exported to {filename}.csv")
    # Plot data
    print("Plotting data...")
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(8, 6))
    ax1.set_ylim(0, 2000)
    ax2.set_ylim(-200, 200)
    ax1.plot(time_array, piezo_array)
    ax1.title.set_text('Piezo data')
    ax2.plot(time_array, gyro_array)
    ax2.title.set_text('Gyro data')
    plt.savefig(f"{filename}.png")

#-------------------------------------------------------------------------
    

# Main program -----------------------------------------------------------
    
duration = int(input("Enter the number of seconds to collect data: "))
sample_rate = 100
byte_array = collect_data(duration, sample_rate)
piezo_array, gyro_array, time_array = decode_data(byte_array, duration * sample_rate)
export_and_plot(piezo_array, gyro_array, time_array)

#--------------------------------------------------------------------------