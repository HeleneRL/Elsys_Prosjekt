from flask import Flask, request
import matplotlib.pyplot as plt
import threading
import time
from matplotlib.animation import FuncAnimation
from matplotlib.offsetbox import OffsetImage, AnnotationBbox
import numpy as np

app = Flask(__name__)

# Initialize list to store sensor data
timestamps = np.array([])
piezo_data = np.array([])
gyro_data_x = np.array([])
gyro_data_y = np.array([])
gyro_data_z = np.array([])

plot_size = 100
gyro_lim = 2
piezo_lim = 4200
gyro_threshold = 0.2
piezo_threshold = 1000

lock = threading.Lock()
new_data_received = False  # Flag to indicate if new data has been received

def plot_data_thread():
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(8, 6))

    def animate(i):
        ax1.clear()
        ax1.set_ylim(0, piezo_lim)
        ax1.plot(timestamps[-plot_size:]/1000, piezo_data[-plot_size:], label='Piezo')
        #ax1.axhline(y = piezo_threshold, color = 'r', linestyle = '--')
        ax1.set_ylabel('Piezo Data')
        ax1.set_title('Real-time Piezo Data')
        ax1.legend()

        ax2.clear()
        ax2.set_ylim(-gyro_lim, gyro_lim)
        ax2.plot(timestamps[-plot_size:]/1000, gyro_data_x[-plot_size:], label='Gyro X', color='b')
        ax2.plot(timestamps[-plot_size:]/1000, gyro_data_y[-plot_size:], label='Gyro Y', color='r')
        ax2.plot(timestamps[-plot_size:]/1000, gyro_data_z[-plot_size:], label='Gyro Z', color='g')
        #plt.axhline(y = gyro_threshold, color = 'r', linestyle = '--')
        #plt.axhline(y = -gyro_threshold, color = 'r', linestyle = '--')
        ax2.set_xlabel('Timestamp')
        ax2.set_ylabel('Gyro Data')
        ax2.set_title('Real-time Gyro Data')
        ax2.legend()

    ani = FuncAnimation(fig, animate, interval=1000)
    plt.tight_layout()
    plt.show()



def plot_data():
    threading.Thread(target=plot_data_thread, daemon=True).start()

@app.route('/receiver_path', methods=['POST'])
def receive_data():
    # Receive sensor and timestamp data from Arduino
    global timestamps, piezo_data, gyro_data_x, gyro_data_y, gyro_data_z
    data = request.get_data().decode("utf-8")
    data = data[5:]
    print(data)
    piezo, gyro_x, gyro_y, gyro_z, timestamp = map(float, data.split(','))
    timestamps = np.append(timestamps, (float(timestamp)))
    piezo_data = np.append(piezo_data, (float(piezo)))
    gyro_data_x = np.append(gyro_data_x, (float(gyro_x)))
    gyro_data_y = np.append(gyro_data_y, (float(gyro_y)))
    gyro_data_z = np.append(gyro_data_z, (float(gyro_z)))

    return "Data received successfully"


if __name__ == '__main__':
    plot_data()  # Start the plot thread
    app.run(host='0.0.0.0', port=5000)