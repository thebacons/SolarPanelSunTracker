# SolarPanelSunTracker
S

This version is a PoC and uses an ESP32 TTGO  version and 2 servo motors to rotate a solar panel so it continuously faces the sun. The Arduino code uses the following main libraries. 
1. WiFiUdp.h - A library to connect to the internet and to the de.pool.ntp.org 
2. NTPClient.h - A library to determine the current date and time. This connects to the Network Time Protocol in Germany, de.pool.ntp.org. See this website for more details --> https://www.ntppool.org/en/#news  The date and time are used with the sunPosition.h library, which needs the exact date and time to determine the sun's position. 
3. SunPosition.h - A library To track the sun's current position. the code has been hardcoded with my current address near Aachen in Germany. The following website was used to validate the results of the sun position angles -> http://shadowcalculator.eu/#/lat/50.753824866570575/lng/6.020683529072293 
4. TFT_eSPI.h - A library to control the display of the ESP32-TTGO display. I just use i to display the current azimuth and altitude angles. The altitude is the angle of the sun to the horizon and the azimuth is the angle in degrees from the z-axis to the horizon.
5. ESP32Servo.h - A library to rotate the servo. This is a great library as I only need to define the pwm pins and the angle values are just entered directly without any mapping.  


A solar panel that constantly faces the sun throughout the day can produce more energy compared to a static position, especially during midday. Here are some benefits of using a solar panel sun tracker:
Increased energy production: By tracking the sun's movement, a solar panel can capture more sunlight throughout the day, leading to higher energy production compared to a fixed position, especially during the morning and afternoon when the sun is at an angle to the panel.
Improved efficiency: Solar panels are most efficient when they are directly facing the sun. By tracking the sun's movement, the panel can maintain optimal orientation, maximizing the amount of sunlight it receives. This results in increased efficiency and more electricity generation.
Longer solar exposure: By rotating to face the sun, the solar panel can have longer exposure to sunlight. This is particularly beneficial during the shorter winter days when the sun's angle is lower, allowing the panel to capture more sunlight and generate more power.
Reduced shading effects: Tracking the sun's movement helps minimize shading effects from nearby objects such as trees or buildings. By adjusting the panel's position, it can avoid being blocked by shadows, which would otherwise reduce the panel's output.
To test the logic of a solar panel sun tracker, an ESP32 TTGO version and two servo motors can be used to rotate a solar panel so it continuously faces the sun. The Arduino code uses several libraries to connect to the internet, determine the current date and time, and track the sun's current position. The code also uses a library to control the display of the ESP32-TTGO display and another library to rotate the servo.
While RC servo motors are too weak to rotate a full solar panel, this setup can be used to measure the increase in power by tracking the sun compared to having the solar panel facing one direction. To estimate the savings based on average solar radiation data for your location, you can use historical weather data to get a more accurate estimation.
Here are some resources that provide more information on solar panel sun trackers and how to build them:
*Sun Tracking for More Solar Power - Part 1 - The Hardware: https://www.switchdoc.com/2016/05/sun-tracking-solar-power-part-1/
*f2knpw/ESP32_Solar_Tracker: low cost solar panel solution (MPPT + sun tracker). Here is the code for the Solar Tracker - GitHub: https://github.com/f2knpw/ESP32_Solar_Tracker
*DIY Portable Solar Tracker - How To Build Simple Tracker - Progressive Automations: https://www.progressiveautomations.com/blogs/how-to/how-to-build-a-portable-solar-tracker
*Solar Radiation Data Sources, Applications and Network Design - OSTI.GOV: https://www.osti.gov/servlets/purl/6652754
*Arduino Solar Tracker (Single or Dual Axis) : 6 Steps (with Pictures) - Instructables: https://www.instructables.com/Arduino-Solar-Tracker-Single-or-Dual-Axis/
*Solar Tracker (ESP32 & MicroPython) : 4 Steps - Instructables: https://www.instructables.com/Two-axis-Solar-Tracker-Controlled-by-ESP32-and-Pro/
