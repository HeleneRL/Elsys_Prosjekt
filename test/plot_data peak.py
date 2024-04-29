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

plot_size = 1000
gyro_lim = 2
piezo_lim = 4200
gyro_threshold = 0.2
piezo_threshold = 1000

lock = threading.Lock()
new_data_received = False  # Flag to indicate if new data has been received

def plot_data_thread():
    fig, ax = plt.subplots()

    def animate(i):
        ax.clear()
        ax.set_ylim(-100, piezo_lim)
        ax.plot(timestamps[-plot_size:], piezo_data[-plot_size:], label='Piezo')
        ax.set_ylabel('Value[0-4096]')
        ax.set_xlabel('Time[ms]')

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
    if piezo > 500:
        piezo_data = np.append(piezo_data, (float(piezo)))
    else:
        piezo_data = np.append(piezo_data, 0)

    return "Data received successfully"


if __name__ == '__main__':
    plot_data()  # Start the plot thread
    app.run(host='0.0.0.0', port=5000)