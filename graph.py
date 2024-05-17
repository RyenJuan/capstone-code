import csv
import numpy as np
import matplotlib.pyplot as plt

# Read the first CSV file
data1 = []
folder_name="test9_H60_T30"

with open(f'/Users/ryanhuang/Desktop/CAPSTONE RESULTS/{folder_name}/humidity_0.csv', 'r') as file:
    reader = csv.reader(file)
    for row in reader:
        data1.append(float(row[0]))  # Assuming one column of float data
    time1 = data1[-1]
    data1.pop()

# Read the second CSV file
data2 = []
with open(f'/Users/ryanhuang/Desktop/CAPSTONE RESULTS/{folder_name}/temp_1.csv', 'r') as file:
    reader = csv.reader(file)
    for row in reader:
        data2.append(float(row[0]))  # Assuming one column of float data


# Define the maximum time
max_time = time1

# Define your custom x-axis range
x_start = 0
x_end = max_time
num_points = len(data1) # Number of points between start and end

# Generate equally spaced x-axis values
x = np.linspace(x_start, x_end, num_points)

# Plot the data
plt.plot(x, data1, label='Relative Humidity')
plt.plot(x, data2, label='Temperature')
plt.xlabel('Time (seconds)')
plt.ylabel('Relative Humidity % and Temperature C')
plt.title('Relative Humidity (%) and Temperature (C) in the Chamber')
plt.grid(True)
plt.legend()

# Set the x-axis limits
plt.xlabel('Time (seconds)')
plt.ylabel('Relative Humidity % and Temperature C')
plt.title('Relative Humidity (%) and Temperature (C) in the Chamber')
plt.grid(True)
plt.legend()

# Set the x-axis limits
plt.xlim(x_start, x_end)

# Specify the actual line for reference
settling_value = 5
plt.axhline(y=settling_value, color='gray', linestyle='--')

# # Show the plot
# plt.show()

plt.xlim(x_start, x_end)

# Specify the actual line for reference
settling_value1 = 95
plt.axhline(y=settling_value1, color='gray', linestyle='--')

# Show the plot
plt.show()
