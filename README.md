# SolarPanelSunTracker 
## by Colin Bacon
This project was inspired when I bought my first solar panel and after getting fed up constantly trying to reposition the solar panel to point towards the sun.

A solar panel sun tracker, with its tilting turntable and azimuth and altitude control, offers several benefits compared to a static solar panel position as follows:

1. Increased energy production: By constantly adjusting the position of the solar panel to face the sun directly, a sun tracker maximizes the amount of sunlight captured throughout the day. This leads to higher energy production and increased efficiency.
2. Improved efficiency: Sun trackers ensure that the solar panel is always oriented optimally, minimizing the angle of incidence between sunlight and the panel's surface. This allows for better absorption of solar radiation and improved conversion of sunlight into electricity.
3. Extended operational hours: Sun trackers enable the solar panel to track the sun's movement from sunrise to sunset. By following the sun's path, they capture sunlight for a longer duration compared to fixed panels, maximizing electricity generation throughout the day.
4. Enhanced seasonal performance: Sun trackers can accommodate seasonal changes in the sun's path. They can adjust the tilt and orientation of the solar panel to match the sun's position during different times of the year, optimizing energy production in all seasons.
5. Reduced solar panel shading: Sun trackers can actively monitor and mitigate shading issues caused by nearby objects such as trees, buildings, or other structures. By continuously adjusting the position of the solar panel, they can minimize shading and ensure maximum exposure to sunlight.
6. Cost-effectiveness: While sun trackers may require an initial investment, their increased energy production and efficiency, along with reduced shading, can result in higher overall energy savings. The additional electricity generated can offset the cost of the tracker over time.

These benefits make solar panel sun trackers an attractive option for maximizing solar energy generation and optimizing the performance of solar panel systems.


To test the logic of a solar panel sun tracker, an ESP32 TTGO version and two servo motors can be used to rotate a solar panel so it continuously faces the sun. The Arduino code uses several libraries to connect to the internet, determine the current date and time, and track the sun's current position. The code also uses a library to control the display of the ESP32-TTGO display and another library to rotate the servo.

While RC servo motors could be appropriate for small solar panels, such as those used to charge a mobile phone, they may lack the strength required to rotate a full-sized solar panel, like a 100-watt one. Nevertheless, this setup can prove valuable for measuring the power increase achieved by tracking the sun compared to keeping the solar panel fixed in one direction. To estimate potential savings based on average solar radiation data in your area, you can enhance accuracy by referencing historical weather data for a more precise estimation.

In this version, which serves as a Proof of Concept (PoC), an ESP32 TTGO version and 2 servo motors are utilized to ensure continuous solar panel alignment with the sun. The Arduino code relies on the following main libraries:
1. WiFiUdp.h: This library enables internet connectivity and connects to de.pool.ntp.org for network time synchronization.
2. NTPClient.h - A library to determine the current date and time. This connects to the Network Time Protocol in Germany, de.pool.ntp.org. Additional information can be found at --> https://www.ntppool.org/en/#news  The date and time are required as inputs to the sunPosition.h library, to determine the sun's position. 
3. SunPosition.h - A library developed by AlexGyver (https://github.com/GyverLibs/SunPosition) to track the sun's current position. My sketch is hardcoded with my current address near Aachen in Germany, so you'll need to update this line with your own location. The following website was used to validate the results of the sun position angles -> http://shadowcalculator.eu/#/lat/50.753824866570575/lng/6.020683529072293
![Solar Tracker ](https://github.com/GyverLibs/SunPosition/blob/main/doc/angles.png)
4. TFT_eSPI.h - A library to control the display of the ESP32-TTGO display. It's recommended to download the library from GitHub as the version provided in the Arduino IDE may not work correctly --> https://www.lilygo.cc/en-pl/products/lilygo%C2%AE-ttgo-t-display-1-14-inch-lcd-esp32-control-board m ![image](https://github.com/thebacons/SolarPanelSunTracker/assets/77930793/a84007d6-6866-4f6d-b1e5-2f3624c783e1) The TFT display is primarily used to show the current azimuth and altitude angles, where altitude represents the sun's angle to the horizon, and azimuth is the angle from the z-axis to the horizon.
5. ESP32Servo.h - A library to rotate the servo. It's a convenient library as it only requires defining the PWM pins, and angle values can be directly input without the need for mapping.
6. 3D Printed model for PoC ![video](https://youtube.com/shorts/ansnRClUkbA?feature=share)

Here are some resources that provide more information on solar panel sun trackers and how to build them:

- Sun Tracking for More Solar Power - Part 1 - The Hardware: https://www.switchdoc.com/2016/05/sun-tracking-solar-power-part-1/
- f2knpw/ESP32_Solar_Tracker: low cost solar panel solution (MPPT + sun tracker). Here is the code for the Solar Tracker - GitHub: https://github.com/f2knpw/ESP32_Solar_Tracker
- DIY Portable Solar Tracker - How To Build Simple Tracker - Progressive Automation: https://www.progressiveautomations.com/blogs/how-to/how-to-build-a-portable-solar-tracker
- Solar Radiation Data Sources, Applications and Network Design - OSTI.GOV: https://www.osti.gov/servlets/purl/6652754
- Arduino Solar Tracker (Single or Dual Axis): 6 Steps (with Pictures) - Instructables: https://www.instructables.com/Arduino-Solar-Tracker-Single-or-Dual-Axis/
- Solar Tracker (ESP32 & MicroPython): 4 Steps - Instructables: https://www.instructables.com/Two-axis-Solar-Tracker-Controlled-by-ESP32-and-Pro/
- Online solar tracker:  [https://www.suncalc.org/#/40.1789,-3.5156,3/2023.09.05/16:21/1/3](https://www.suncalc.org/#/50.718,6.2535,17/2023.09.05/16:25/1/3)

Once the sun sets in the evening, the servos go to sleep until the next morning. A message is displayed at the bottom of the TFT display with sunset time as follows: ![TFT Display](https://github.com/thebacons/SolarPanelSunTracker/blob/main/ESP32_TTGO_CurrentTime_SunPosition_ESPServo_V4/TFT%20Display_1.png)
** Here are the next steps outlined for the solar panel project:

1. Power Supply Integration: Utilize the solar panel to provide power to essential components such as the ESP32 and servos. This ensures that the system can operate autonomously with sustainable energy: -- > https://randomnerdtutorials.com/power-esp32-esp8266-solar-panels-battery-level-monitoring/

2. Power Measurement and Data Logging: Implement a mechanism to measure the power generated by the solar panel system. Store these measured values on an hourly basis. This data logging allows for the calculation of an average of the actual power generated, which can be valuable for estimating the feasibility and performance of large-scale solar projects. Calculating solar power received from a solar panel using an ESP32 involves a few steps. Here's a high-level overview of the process:

Hardware Setup:

- Connect a solar panel to your ESP32. You might need additional components like voltage dividers or analog-to-digital converters depending on your solar panel specifications.
Read Voltage and Current:

- Use the ESP32's analog input pins to measure the voltage and current generated by the solar panel. You'll need to convert these values into digital readings.
Sample Data:

- Continuously sample the voltage and current at regular intervals. ESP32 has built-in ADCs that can help with this.
Calculate Power:

- Calculate the instantaneous power at each sample point using the formula: Power (Watts) = Voltage (Volts) * Current (Amps).
Summation:

- Sum up the instantaneous power values over a period to get the total energy generated. You can integrate this over time to get the energy in watt-hours (Wh).
Display or Store Data:

- You can display the data on a screen, log it to an SD card, or send it to a cloud service for further analysis.

Reference Projects: 
- https://circuitdigest.com/microcontroller-projects/iot-solar-panel-power-monitoring-system-using-esp32-and-thingspeak, 
- https://www.opengreenenergy.com/diy-solar-panel-monitoring-system/
- https://www.seeedstudio.com/blog/2023/06/05/solar-panel-monitoring-system-built-with-xiao-esp32c3/

3. Scale the project up for larger panels and use stepper motors instead. Here are the general steps to get you started:

Hardware Setup:

- You'll need a solar panel or array.
- Attach two stepper motors to the panel's frame.
- Connect the stepper motors to the ESP32 using a driver board (e.g., A4988 or DRV8825).
- Ensure your ESP32 is connected to a power source.

Software Development:

- Develop a program in JavaScript (since you're a JavaScript developer) using the Arduino IDE with the ESP32 board support.
- Implement code to read data from light sensors (like LDRs or photodiodes) to detect the sun's position.
- Calculate the angle at which the solar panel should be positioned based on the sun's location.
- Use the ESP32's GPIO pins to control the stepper motors. Libraries like AccelStepper can help with motor control.
- Implement a loop that continuously adjusts the position of the solar panel to track the sun.

Power Management:
- Consider power management to ensure your system works even during cloudy days or when the sun isn't visible.

Testing and Calibration:
- Test your system to ensure it accurately tracks the sun.
- Calibrate your sensors and motors for optimal performance.

Connectivity:
- If you intend to connect your system to the openai.com API, make sure you have the necessary network connectivity components (like Wi-Fi modules) integrated into your ESP32.

Safety and Maintenance:
- Ensure the system's safety by implementing fail-safes to prevent damage during extreme weather conditions.
- Regularly maintain the system to keep it functioning correctly.
 - https://www.switchdoc.com/2020/06/tutorial-solar-power-selection-guide-for-maker-projects/
 - https://www.az-delivery.de/en/blogs/azdelivery-blog-fur-arduino-und-raspberry-pi/solar-tracker-mit-schrittmotoren-und-oled-display   



Colin Bacon grins,
A name that sizzles and pops,
Tasty synergy.
