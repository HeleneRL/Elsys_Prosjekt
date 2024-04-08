from flask import Flask, request
import matplotlib.pyplot as plt
import threading
import time
from matplotlib.animation import FuncAnimation
from matplotlib.offsetbox import OffsetImage, AnnotationBbox
import numpy as np

app = Flask(__name__)

# Initialize list to store sensor data
swallow_list = []
lock = threading.Lock()
new_data_received = False  # Flag to indicate if new data has been received

def plot_data_thread():
    fig, ax = plt.subplots()
    ax.axis('off')  # Turn off the axis
    ax.set_xlim(-1, 1)  # Set x-axis limits
    ax.set_ylim(-1, 1)  # Set y-axis limits
    circle = plt.Circle((0, -0.25), 0.1, color='red', alpha=0.5)
    ax.add_artist(circle)

        # Load the JPEG image
    img = plt.imread('python/neck.png')
    ax.imshow(img, extent=[-1, 1, -1, 1], aspect='auto')

    def update_circle(frame):
        global new_data_received
        with lock:
            if new_data_received:
                # Calculate target position for the circle to move up
                target_y_up = circle.center[1] + 0.5
            
            # Interpolate the circle's position to move it up gradually
                delta_y_up = (target_y_up - circle.center[1]) / 10  # Move the circle by 1/10 of the total distance each frame
                for _ in range(10):
                    circle.center = (circle.center[0], circle.center[1] + delta_y_up)
                    yield circle,

            # Reset the flag after moving up
                new_data_received = False
            
            # Calculate target position for the circle to move down
                target_y_down = circle.center[1] - 0.5
            
            # Interpolate the circle's position to move it down gradually
                delta_y_down = (target_y_down - circle.center[1]) / 10  # Move the circle by 1/10 of the total distance each frame
                for _ in range(10):
                    circle.center = (circle.center[0], circle.center[1] + delta_y_down)
                    yield circle,
        return circle,

    ani = FuncAnimation(fig, update_circle, frames=None, blit=True)
    
    plt.show()

def plot_data():
    threading.Thread(target=plot_data_thread, daemon=True).start()

@app.route('/receiver_path', methods=['POST'])
def receive_data():
    global swallow_list, new_data_received
    data_packet = request.form['data']
    print("Received data packet:", data_packet)
    # Data format: [svelge_update, svelge_data, ligge_alarm_update, ligge_alarm, ligge_pos_update, ligge_pos, ligge_pos_timestamp, falle_update, falle_alarm]
    packet_list = data_packet.split(',')
    
    # Add sensor data to the list
    with lock:
        if(packet_list[0]):
            swallow_list.append(packet_list[1])
        if(packet_list[2]):
            alarm = packet_list[3]
        if(packet_list[4]):
            ligge_pos = packet_list[5]           
            ligge_pos_ts = packet_list[6] 
        if(packet_list[7]):
            fall_alarm = packet_list[8]
        
        # Limit the list to store only the last 20 values
        swallow_list = swallow_list[-20:]
    
    # Set flag to indicate new data received
    new_data_received = True
    
    return 'Data received successfully'

if __name__ == '__main__':
    plot_data()  # Start the plot thread
    app.run(host='0.0.0.0', port=5000)
