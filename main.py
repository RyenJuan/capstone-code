import serial
import matplotlib.pyplot as plt
from collections import deque
import time
import numpy as np

BUFFER_SIZE = 100000
LINE_NAMES = ["HC1", "TC1", "HC2", "TC2",  "HC4", "TC4",  "HC7", "TC7"]

def initialize_plot(NUM_LINES = 2, BUFFER_SIZE=100000):
    """Initialize the plot with specified number of lines and buffer size."""
    data_buffers = [deque(maxlen=BUFFER_SIZE) for _ in range(NUM_LINES)]
    lines = []
    for i in range(NUM_LINES):
        # line, = plt.plot([], [], label=f'Line {i + 1}')
        line, = plt.plot([], [], label=LINE_NAMES[i])
        lines.append(line)
    time_buffer = deque(maxlen=BUFFER_SIZE)

    # Set plot properties
    plt.ion()  # Turn on interactive mode
    plt.legend()
    plt.xlabel('Time (s)')
    plt.ylabel('Sensor Data')
    plt.title('Real-Time Sensor Data')
    plt.ylim(0, 120)  # Adjust y-axis limits based on your sensor range

    return data_buffers, lines, time_buffer


def update_plot(data_buffers, lines, time_buffer, elapsed_time, BUFFER_SIZE):
    """Update the plot with new data."""
    for i, line in enumerate(lines):
        line.set_data(time_buffer, data_buffers[i])

    # Dynamically adjust x-axis limit to show data for the last BUFFER_SIZE seconds
    if elapsed_time > BUFFER_SIZE:
        plt.xlim(elapsed_time - BUFFER_SIZE, elapsed_time + 1)
    else:
        plt.xlim(0, BUFFER_SIZE)

    # plt.pause(0.01)


def main():
    # Initialize serial communication with Arduino
    # ser = serial.Serial('/dev/tty.usbserial-DN03ETCL', 115200)  # Adjust COM port and baud rate as needed
    ser = serial.Serial('/dev/tty.usbmodem14501', 115200)
    # Initialize plot
    data_buffers, lines, time_buffer = initialize_plot()
    start_time = time.time()


    try:
        while True:
            # Read data from Arduino
            data = ser.readline().decode().strip()
            if data:
                # Parse data
                values = list(map(float, data.split()))
                print(values)
                # print(f"Right Wall: {values[2]}% and {values[3]}C | Door: {values[0]}% and {values[1]}C |"
                #       f" Left Wall: {values[6]}% and {values[7]}C | RTD/Back: {values[4]}% and {values[5]}C")
                #
                # print(f"Average Temperature: {(values[1]+values[3]+values[5]+values[7])/4}")
                print(f"Average Humidity: {values[0]}% | Average Temperature: {values[1]}C")
                elapsed_time = time.time() - start_time

                # Update data buffers
                for i, value in enumerate(values):
                    data_buffers[i].append(value)
                time_buffer.append(elapsed_time)

                # Update plot
                update_plot(data_buffers, lines, time_buffer, elapsed_time, BUFFER_SIZE)

    except KeyboardInterrupt:
        # Close serial connection and plot on Ctrl+C
        ser.close()
        plt.close()


    # This last block of code writes to a CSV file
    for i in range(2):
        print(list(data_buffers[i]))
        if i%2 == 0:
            np.savetxt(f'humidity_{i}.csv', list(data_buffers[i]), delimiter=',')
        else:
            np.savetxt(f'temp_{i}.csv', list(data_buffers[i]), delimiter=',')

    print(f"Total time: {time.time()-start_time}")


if __name__ == "__main__":
    main()
