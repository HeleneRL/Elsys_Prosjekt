from flask import Flask, request
import matplotlib.pyplot as plt
import threading
import time
from matplotlib.animation import FuncAnimation

app = Flask(__name__)

# Initialize list to store sensor data
sensor_data_list = []
lock = threading.Lock()
datalist = [0,0,0,0,0,0,0,0,0,0]
new_data_received = False  # Flag to indicate if new data has been received
new_y = -0.25
svelgetall = 0


def update_plot(frame,ax1,ax2,ax3):
    global sensor_data_list, new_data_received,datalist
    with lock:
        if new_data_received:
            # Update each subplot based on the corresponding index of datalist
            update_svelg(int(datalist[0]))
            update_side(int(datalist[2]))
            update_fall(int(datalist[7]))
            new_data_received = False
    return ax1,ax2,ax3

def update_svelg(data):
    global new_y, svelgetall
    if data:
        svelgetall += 1
        print(svelgetall)
        if new_y >= -0.15:
            new_y = -0.25
        else:
            new_y = min(circle.center[1] + 0.05, 0.9)  # Move circle upward by 0.01 each time
        circle.set_center((circle.center[0], new_y))
    
    print("Updating subplot 1 with data:", data)

def update_side(data):
    if data:
        text_obj.set_visible(True)
    else:
        text_obj.set_visible(False)
    print("Updating subplot 2 with data:", data)

def update_fall(data):
    if data:
        ax2.add_artist(circle2)
    print("Updating subplot 3 with data:", data)

def plot_data():
    global circle, circle2,text_obj,ax3,ax2
    fig, (ax1, ax2, ax3) = plt.subplots(1, 3, figsize=(15, 5))

    
    # Subplot 1 settings
    ax1.axis('off')
    ax1.set_xlim(-1, 1)
    ax1.set_ylim(-1, 1)

    # Load the JPEG image and display it in subplot 1
    img = plt.imread('neck.png')
    ax1.imshow(img, extent=[-1, 1, -1, 1], aspect='auto')
    circle = plt.Circle((0, -0.25), 0.1, color='blue', alpha=0.5)
    ax1.add_artist(circle)


    # Subplot 2 settings
    ax2.axis('off')
    ax2.set_xlim(-1,1)
    ax2.set_ylim(-1,1)
    circle2 = plt.Circle((0,0),1,color='red',alpha=0.5)

    #Subplot 3 settings
    ax3.axis('off')
    text_obj = ax3.text(0.5, 0.5, 'Bytt side', fontsize=12, color='black', ha='center', va='center')

    # Initialize the subplots as needed

    # Create animation
    ani = FuncAnimation(fig, update_plot, frames=None, fargs=(ax1,ax2,ax3), blit=True)
    plt.show()


@app.route('/receiver_path', methods=['POST'])
def receive_data():
    global sensor_data_list, new_data_received,datalist
    sensor_data =request.form['data']
    datalist = sensor_data.split(",")
    print("Received sensor data:", sensor_data)
    
    new_data_received = True
    
    return 'Data received successfully'

if __name__ == '__main__':
    threading.Thread(target=plot_data, daemon=True).start()
    app.run(host='0.0.0.0', port=5000)  