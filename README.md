# SolarPanelSunTracker
Solar Panel Sun Tracker for ESP32 TTGO

Rotating a solar panel to constantly face the sun throughout the day has several benefits compared to a static position, especially during midday. Here are some advantages:

Increased energy production: By tracking the sun's movement, a solar panel can capture more sunlight throughout the day. This leads to higher energy production compared to a fixed position, especially during the morning and afternoon when the sun is at an angle to the panel.

Improved efficiency: Solar panels are most efficient when they are directly facing the sun. By tracking the sun's movement, the panel can maintain optimal orientation, maximizing the amount of sunlight it receives. This results in increased efficiency and more electricity generation.

Longer solar exposure: By rotating to face the sun, the solar panel can have longer exposure to sunlight. This is particularly beneficial during the shorter winter days when the sun's angle is lower, allowing the panel to capture more sunlight and generate more power.

Reduced shading effects: Tracking the sun's movement helps minimize shading effects from nearby objects such as trees or buildings. By adjusting the panel's position, it can avoid being blocked by shadows, which would otherwise reduce the panel's output.

Overall, rotating a solar panel to constantly face the sun can significantly increase its power output and energy production compared to a static position, especially during midday when the sun is at its highest point in the sky.

This version is a PoC and uses an ESP32 TTGO  version and 2 servo motors to rotate a solar panel so it continuously faces the sun. The codes uses the folling main libraries. 
1. WiFiUdp.h - A library to connect to the internet and to the de.pool.ntp.org 
2. NTPClient.h - A library to determine the current date and time. This connects to the Network Time Protocol in Germany, de.pool.ntp.org. See this website for more details --> https://www.ntppool.org/en/#news  The date and time are used with the sunPosition.h library, which needs the exact date and time to determine the sun's position. 
3. SunPosition.h - A library To track the sun's current position. the code has been hardcoded with my current address near Aachen in Germany. The following website was used to validate the results of the sun position angles -> http://shadowcalculator.eu/#/lat/50.753824866570575/lng/6.020683529072293 
4. TFT_eSPI.h - A library to control the display of the ESP32-TTGO display. I just use i to display the current azimuth and altitude angles. The altitude is the angle of the sun to the horizon and the azimuth is the angle in degrees from the z-axis to the horizon.


RC servo motors are obviously too weak to rotate a full solar panel, but I just want to test whether the logic worked as a PoC, where I just used a small solar panel to measure the increase in power by tracking the sun compared to having the solar panel facing one direction. 



To estimate the savings based on average solar radiation data for your location,  you can use this approach with the historical weather data you have to get a more accurate estimation.

Determine the average solar radiation for your location: Solar radiation data is typically measured in kilowatt-hours per square meter per day (kWh/m²/day). You can find historical solar radiation data for your location from reliable sources like national meteorological agencies or solar energy organizations.

Calculate the average daily energy production: Multiply the average solar radiation by the efficiency of your solar panel system. Assuming an average efficiency of around 15-20%, you can estimate the average daily energy production of your 100-watt solar panel.

Calculate monthly and annual energy production: Multiply the average daily energy production by the number of days in each month to get the monthly energy production. Then, sum up the monthly energy production to obtain the total energy production for the year.

Calculate savings: Multiply the total energy production by the cost of electricity per kilowatt-hour in your area to estimate the savings. This will depend on your local electricity rates.

Remember, this estimation assumes 100% production on sunny days and no production on cloudy days. Actual weather conditions and variations in solar radiation will affect the accuracy of the estimate.


Based on the average daylight hours in Europe, we can estimate the total additional power for a 100-watt solar panel tracking the sun's direction.
Assuming an average daily solar radiation of 4 kWh/m²/day and an average of 9 hours of daylight per day in Aachen, we can calculate the monthly energy production using the average daily solar radiation and multiply it by the average number of daylight hours for each month. Then, we can sum up the monthly energy production values to calculate the total additional power for the complete year based on the average daylight hours in Aachen.
Here is an example calculation:
Monthly Energy Production:
January: 0.6 kWh/day * 31 days * 9 hours = 167.4 kWh
February: 0.6 kWh/day * 28 days * 9 hours = 151.2 kWh (or 29 days in a leap year)
March: 0.6 kWh/day * 31 days * 9 hours = 167.4 kWh
April: 0.6 kWh/day * 30 days * 9 hours = 162 kWh
May: 0.6 kWh/day * 31 days * 9 hours = 167.4 kWh
June: 0.6 kWh/day * 30 days * 9 hours = 162 kWh
July: 0.6 kWh/day * 31 days * 9 hours = 167.4 kWh
August: 0.6 kWh/day * 31 days * 9 hours = 167.4 kWh
September: 0.6 kWh/day * 30 days * 9 hours = 162 kWh
October: 0.6 kWh/day * 31 days * 9 hours = 167.4 kWh
November: 0.6 kWh/day * 30 days * 9 hours = 162 kWh
December: 0.6 kWh/day * 31 days * 9 hours = 167.4 kWh
Total Yearly Energy Production:
Sum of Monthly Energy Production for all twelve months: 1997.8 kWh
Therefore, the total estimated additional power for a 100-watt solar panel tracking the sun's direction using the code in Aachen, Germany is approximately 1997.8 kWh per year. Please note that this is an estimation and actual energy production can vary based on specific weather conditions and solar radiation values for your location.
Sources of data
Solcast: https://solcast.com
SpringerLink: https://www.springer.com
Deutscher Wetterdienst: https://www.dwd.de
OpenWeatherMap: https://openweathermap.org
National Solar Radiation Database (NSRDB): https://nsrdb.nrel.gov
