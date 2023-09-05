# SolarPanelSunTracker
A solar panel sun tracker, with its tilting turntable and azimuth and altitude control, offers several benefits compared to a static solar panel position:

1.Increased energy production: By constantly adjusting the position of the solar panel to face the sun directly, a sun tracker maximizes the amount of sunlight captured throughout the day. This leads to higher energy production and increased efficiency.

2. Improved efficiency: Sun trackers ensure that the solar panel is always oriented optimally, minimizing the angle of incidence between sunlight and the panel's surface. This allows for better absorption of solar radiation and improved conversion of sunlight into electricity.

3. Extended operational hours: Sun trackers enable the solar panel to track the sun's movement from sunrise to sunset. By following the sun's path, they capture sunlight for a longer duration compared to fixed panels, maximizing electricity generation throughout the day.

4. Enhanced seasonal performance: Sun trackers can accommodate seasonal changes in the sun's path. They can adjust the tilt and orientation of the solar panel to match the sun's position during different times of the year, optimizing energy production in all seasons.

5. Reduced solar panel shading: Sun trackers can actively monitor and mitigate shading issues caused by nearby objects such as trees, buildings, or other structures. By continuously adjusting the position of the solar panel, they can minimize shading and ensure maximum exposure to sunlight.

6. Cost-effectiveness: While sun trackers may require an initial investment, their increased energy production and efficiency, along with reduced shading, can result in higher overall energy savings. The additional electricity generated can offset the cost of the tracker over time.

These benefits make solar panel sun trackers an attractive option for maximizing solar energy generation and optimizing the performance of solar panel systems.


To test the logic of a solar panel sun tracker, an ESP32 TTGO version and two servo motors can be used to rotate a solar panel so it continuously faces the sun. The Arduino code uses several libraries to connect to the internet, determine the current date and time, and track the sun's current position. The code also uses a library to control the display of the ESP32-TTGO display and another library to rotate the servo.

While RC servo motors are too weak to rotate a full solar panel, this setup can be used to measure the increase in power by tracking the sun compared to having the solar panel facing one direction. To estimate the savings based on average solar radiation data for your location, you can use historical weather data to get a more accurate estimation.

This version is merely a PoC and uses an ESP32 TTGO  version and 2 servo motors to rotate a solar panel so it continuously faces the sun. The Arduino code uses the following main libraries. 
1. WiFiUdp.h - A library to connect to the internet and to the de.pool.ntp.org 
2. NTPClient.h - A library to determine the current date and time. This connects to the Network Time Protocol in Germany, de.pool.ntp.org. See this website for more details --> https://www.ntppool.org/en/#news  The date and time are used with the sunPosition.h library, which needs the exact date and time to determine the sun's position. 
3. SunPosition.h - A library To track the sun's current position. the code has been hardcoded with my current address near Aachen in Germany. The following website was used to validate the results of the sun position angles -> http://shadowcalculator.eu/#/lat/50.753824866570575/lng/6.020683529072293 
4. TFT_eSPI.h - A library to control the display of the ESP32-TTGO display. Ennsure to download the library from GitHub as the version provided in the Arduono IDE does not work. --> https://www.lilygo.cc/en-pl/products/lilygo%C2%AE-ttgo-t-display-1-14-inch-lcd-esp32-control-board m  ![image](https://github.com/thebacons/SolarPanelSunTracker/assets/77930793/a84007d6-6866-4f6d-b1e5-2f3624c783e1)
 I just use the TFT to display the current azimuth and altitude angles. The altitude is the angle of the sun to the horizon and the azimuth is the angle in degrees from the z-axis to the horizon.
5. ESP32Servo.h - A library to rotate the servo. This is a great library as I only need to define the pwm pins and the angle values are just entered directly without any mapping.  


Here are some resources that provide more information on solar panel sun trackers and how to build them:

*Sun Tracking for More Solar Power - Part 1 - The Hardware: https://www.switchdoc.com/2016/05/sun-tracking-solar-power-part-1/

*f2knpw/ESP32_Solar_Tracker: low cost solar panel solution (MPPT + sun tracker). Here is the code for the Solar Tracker - GitHub: https://github.com/f2knpw/ESP32_Solar_Tracker

*DIY Portable Solar Tracker - How To Build Simple Tracker - Progressive Automations: https://www.progressiveautomations.com/blogs/how-to/how-to-build-a-portable-solar-tracker

*Solar Radiation Data Sources, Applications and Network Design - OSTI.GOV: https://www.osti.gov/servlets/purl/6652754

*Arduino Solar Tracker (Single or Dual Axis) : 6 Steps (with Pictures) - Instructables: https://www.instructables.com/Arduino-Solar-Tracker-Single-or-Dual-Axis/

*Solar Tracker (ESP32 & MicroPython) : 4 Steps - Instructables: https://www.instructables.com/Two-axis-Solar-Tracker-Controlled-by-ESP32-and-Pro/


Colin Bacon grins,
A name that sizzles and pops,
Tasty synergy.
