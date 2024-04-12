import numpy as np
import random
from flask import Flask, request
import threading
from matplotlib.animation import FuncAnimation
from matplotlib.offsetbox import OffsetImage, AnnotationBbox
import matplotlib.image as mpimg
import matplotlib.pyplot as plt

app = Flask(__name__)

# Initialize list to store sensor data
swallow_list = []
lock = threading.Lock()
new_data_received = False  # Flag to indicate if new data has been received

# Load the image from your computer
image_path = "/svelgebilde.png"  # Replace this with the path to your image
image = mpimg.imread(image_path)

# Divide the image into 5 horizontal and 5 vertical parts
num_horizontal_parts = 5
num_vertical_parts = 5
part_height = image.shape[0] // num_vertical_parts
part_width = image.shape[1] // num_horizontal_parts
parts = [
    image[i * part_height: (i + 1) * part_height, j * part_width: (j + 1) * part_width]
    for i in range(num_vertical_parts)
    for j in range(num_horizontal_parts)
]

# Initialize the figure with 25 subplots at the correct positions
fig, axs = plt.subplots(5, 5, figsize=(10, 10), gridspec_kw={"hspace": 0.02, "wspace": 0.01})

# Create subplots and set all subplots to invisible initially
for i in range(num_vertical_parts):
    for j in range(num_horizontal_parts):
        ax = axs[i, j]
        ax.imshow(parts[i * num_horizontal_parts + j])
        ax.axis("off")
        ax.set_visible(False)

# Function to reveal one part of the photo
def show_random_part():
    invisible_axs = [ax for ax in axs.ravel() if not ax.get_visible()]
    if invisible_axs:
        random_ax = random.choice(invisible_axs)
        random_ax.set_visible(True)
        plt.draw()
        plt.pause(0.01)  # Added to force the plot to update

# Turn on interactive mode
plt.ion()



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
            if(packet_list[0]):
                show_random_part()
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