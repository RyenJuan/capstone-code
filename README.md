# capstone-code
<h1> Subsystem Control and Data Collection </h1>

The code was written by Ryan Huang and Demitri Kokoros for an environmental control chamber for use in Professor Xiaoyu Tang's microfluidics research. 

The humidifier, dehumidifier, fans, valves, and sensors are regulated by control.ino. The gantry system is contorlled by acuator_control.ino. Real time data collection is done in main.py. Temperature and humidity data over time is plotted in graph.py

<h2>Files</h2>
<ul>
  <li>control.ino - Humidifier, dehumidifier, fan, valve, and sensor control</li>
  <li>actuator_control.ino - Gantry homing and positoning</li>
  <li>main.py - Real-time graphing and exports as csv files</li>
  <li>graph.py - Generates plots of temperature and humidity data from main.py</li>
</ul> 

<h2>Environmental Control Usage</h2>
<ol>
  <li>Ensure the electrical box is turned on</li>
  <li>Within control.ino:
    <ol>
      <li>Modify TARGET_TEMP, TARGET_HUMIDITY, and coolingDuration in control.ino and upload to the Electrical Box's Arduino</li>
      <li>Set printToPython to true</li>
    </ol>
  </li>
  <li>Change the COM port and Baud Rate in main.py to match the port/baud rate used by Arduino</li>
  <li>Run main.py</li>
  <li>When the experiment is over, stop the program by keyboard interrupt</li>
</ol>

<h2>Gantry Usage</h2>
<ol>
  <li>Ensure the electrical box is turned on</li>
  <li>Verify and upload actuator_control.ino to the gantry's Arduino</li>
  <li>Follow the text commands in the serial monitor to move the gantry</li>
</ol>

<h2>Data Collection and Graphing</h2>
<ol>
  <li>Find two files generated after main.py was manually ended (temp_0.csv and humidity_1.csv)</li>
  <li>Set printToPython to false in control.ino
  <li>Modify the file paths in graph.py to the locations of temp_0.csv and humidity_1.csv</li>
  <li>Within humidity_0.csv, add the total time the experiment ran for as a new row at the end of the csv file. This duration can be found in the python console of main.py after the keyboard interrupt.</li>
  <li>Run graph.py</li>

</ol>

<h2>Future Tasks</h2>
A lot of these files were written in a hurry to collect data as most of us do not plan on working on this after Capstone Day so there are a lot of additional tasks to improve the readability, functionality, and modularity.
<br></br>
<ul>
  <li>Combine main.py and graph.py into a single python file that runs collects data, exports it, and graphs it​</li>

  <li>Streamline the graphing process by removing the manual step of adding experiment time to the last line of humidity data​</li>

  <li>Refactor graph.py. This program was written the fastest and is somewhat messy to read​</li>

  <li>Find a way to manually grab the current port being used. Ocassionally, Arduino switches ports, requiring you to manually change the port in the python code (highly unnecessary).</li>

  <li>Fix the real-time graphing code inside main.py. Data can be plotted in real-time in a continuosly updating plot within main.py. However, this stopped working at some point and I'm not sure why​</li>

  <li>Create a separate Arduino script to act as a "playground". Allowing users to run specific valves and fans via keypress. Currently, control.ino runs everything sequentially (cooling, heating, humidifying), making it difficult to debug specific subsytems until it reaches the subsystem in the sequence  ​</li>
</ul>


