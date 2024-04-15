from flask import Flask, request
import matplotlib.pyplot as plt
import threading
import time
from matplotlib.animation import FuncAnimation
from matplotlib.offsetbox import OffsetImage, AnnotationBbox
import numpy as np

app = Flask(__name__)

# Initialize list to store sensor data
swallow_list = [0, 0, 0, 0, 0, 0, 0, 0, 0]
update = 0
lock = threading.Lock()
new_data_received = False  # Flag to indicate if new data has been received
current = 0

import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import time
import numpy as np

def plot_data_thread():
    fig, ax = plt.subplots()   
    global swallow_list, current

    def animate(i):
        global current
        current_time = time.time() - start_time
        ax.clear()
        if current == swallow_list[1]:
            values.append(0)
        else:
            current = swallow_list[1]
            values.append(1)
        timestamps.append(current_time)  # Append current time to timestamps list
        ax.plot(timestamps[-30:], values[-30:], marker='o')  # Plot the value against time
        ax.set_xlabel('Time (s)')
        ax.set_ylabel('Boolean Value')
        ax.set_title('Real-time Boolean Value')

    start_time = time.time()  # Record the start time
    timestamps = []  # Initialize list to store timestamps
    values = []

    ani = FuncAnimation(fig, animate, interval=1000)
    plt.tight_layout()
    plt.show()




def plot_data():
    threading.Thread(target=plot_data_thread, daemon=True).start()

@app.route('/receiver_path', methods=['POST'])
def receive_data():
    global swallow_list, new_data_received, update
    data_packet = request.form['data']
    print("Received data packet:", data_packet)
    # Data format: [svelge_update, svelge_data, ligge_alarm_update, ligge_alarm, ligge_pos_update, ligge_pos, ligge_pos_timestamp, falle_update, falle_alarm]
    packet_list = data_packet.split(',')
    
    # Add sensor data to the list
    with lock:
        update = packet_list[0]
        if(packet_list[0]):
            swallow_list.append(packet_list[1])
        
        # Limit the list to store only the last 20 values
        swallow_list = swallow_list[-20:]
    
    # Set flag to indicate new data received
    new_data_received = True
    
    return 'Data received successfully'

if __name__ == '__main__':
    plot_data()  # Start the plot thread
    app.run(host='0.0.0.0', port=5000)
