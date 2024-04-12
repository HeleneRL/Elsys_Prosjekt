from flask import Flask, request
import matplotlib.pyplot as plt
import threading
import time
from matplotlib.animation import FuncAnimation
from matplotlib.offsetbox import OffsetImage, AnnotationBbox
import numpy as np

app = Flask(__name__)

# Initialize list to store sensor data
timestamps = []
piezo_data = []
gyro_data = []
max_g = 0

lock = threading.Lock()
new_data_received = False  # Flag to indicate if new data has been received

def plot_data_thread():
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(8, 6))

    def animate(i):
        ax1.clear()
        ax1.plot(timestamps[-30:], piezo_data[-30:], label='Piezo')
        ax1.set_xlabel('Timestamp')
        ax1.set_ylabel('Piezo Data')
        ax1.set_title('Real-time Piezo Data')
        ax1.legend()

        ax2.clear()
        ax2.plot(timestamps[-30:], gyro_data[-30:], label='Gyro')
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
    global max_g
    data = request.get_data().decode("utf-8")
    data = data[5:]
    piezo, gyro, timestamp = map(float, data.split(','))
    if(gyro > max_g):
        max_g = gyro
    timestamps.append(float(timestamp))
    piezo_data.append(float(piezo))
    gyro_data.append(float(gyro))
    print(max_g)

    return "Data received successfully"


if __name__ == '__main__':
    plot_data()  # Start the plot thread
    app.run(host='0.0.0.0', port=5000)