/* use this website to confirm the results of azimuth etc. 
   http://shadowcalculator.eu/#/lat/50.71788931617736/lng/6.253379345993735

   use this website to check the current unix time is calculting correcly
   https://www.confirado.de/tools/timestamp-umrechner.html

   use this website to get the correct time for the region. This one is for Germanz
   https://www.ntppool.org/zone/de


   see http://www.arduino.cc/en/Tutorial/Sweep for a description of the original code

 * Different servos require different pulse widths to vary servo angle, but the range is 
 * an approximately 500-2500 microsecond pulse every 20ms (50Hz). In general, hobbyist servos
 * sweep 180 degrees, so the lowest number in the published range for a particular servo
 * represents an angle of 0 degrees, the middle of the range represents 90 degrees, and the top
 * of the range represents 180 degrees. So for example, if the range is 1000us to 2000us,
 * 1000us would equal an angle of 0, 1500us would equal 90 degrees, and 2000us would equal 1800
 * degrees.
 * 
 * Circuit: (using an ESP32 Thing from Sparkfun)
 * Servo motors have three wires: power, ground, and signal. The power wire is typically red,
 * the ground wire is typically black or brown, and the signal wire is typically yellow,
 * orange or white. Since the ESP32 can supply limited current at only 3.3V, and servos draw
 * considerable power, we will connect servo power to the VBat pin of the ESP32 (located
 * near the USB connector). THIS IS ONLY APPROPRIATE FOR SMALL SERVOS. 
 * 
 * We could also connect servo power to a separate external
 * power source (as long as we connect all of the grounds (ESP32, servo, and external power).
 * In this example, we just connect ESP32 ground to servo ground. The servo signal pins
 * connect to any available GPIO pins on the ESP32 (in this example, we use pin 18.
 * 
 * In this example, we assume a Tower Pro MG995 large servo connected to an external power source.
 * The published min and max for this servo is 1000 and 2000, respectively, so the defaults are fine.
 * These values actually drive the servos a little past 0 and 180, so
 * if you are particular, adjust the min and max values to match your needs.
 
   
*/

#include <NTPClient.h>
#include <WiFi.h> // for WiFi shield
#include <WiFiUdp.h>
#include <SunPosition.h>
#include <ESP32Servo.h>                                                                                                                                                                                                                                                                   
#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>
#include <math.h>

//Display Setup
#define TFT_GREY 0x5AEB // New colour
TFT_eSPI tft = TFT_eSPI();  // Invoke library

//connect to the internet
char ssid[] = "WaldBachHouse";
char pass[] = "Bacon1234!";
WiFiUDP ntpUDP;
Servo servoAzimuth; //Define two servo objects and create a servo instance for each servo:
Servo servoAltitude; //Define two servo objects and create a servo instance for each servo:


// You can specify the time server pool and the offset (in seconds, can be
// changed later with setTimeOffset() ). Additionally you can specify the
// update interval (in milliseconds, can be changed using setUpdateInterval() ).
//NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000); //de.pool.ntp.org
NTPClient timeClient(ntpUDP, "de.pool.ntp.org", 7200, 60000); //7200 for summer time and 3600 for winter


void setup(){
  Serial.begin(9600);

//Display Setup
  tft.init();
  tft.setRotation(3);

//WiFi Setup
  WiFi.begin(ssid, pass);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.println ( "Trying to connect to WiFi" );
  }

  timeClient.begin();

//Setup servos
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);

  servoAzimuth.attach(12, 500, 2400); //  AZIMUTH_SERVO_PIN 
  servoAltitude.attach(13, 500, 2400); // Altitude_SERVO_PIN 
}

void loop() {
  timeClient.update();
  int unixTime = timeClient.getEpochTime();
  
  int currentDay = timeClient.getDay();
  unixTime = (timeClient.getEpochTime());

// Adjusted for daylight saving time (DST) so remove to actual unix time
// see https://randomnerdtutorials.com/esp32-ntp-client-date-time-arduino-ide/
  String currentTime = timeClient.getFormattedTime();
  

//Get the sun poistion details and print e.g. 50.717967480114794, 6.253510201268341 (https://nsrdb.nrel.gov/data-viewer)
 // Solar Irradiance (kWh/m²/Day) for Europe is typically < 3.0)
  SunPosition sun(50.43, 6.15, unixTime-7200);//( longitude,latitude (zweifall), unixTime)
  
// declare variables
  int azimuthValue = sun.azimuth(); // Get azimuth value from SunPosition
  int altitudeValue = sun.altitude(); // Get altitude value from SunPosition
  int sunAzimuthMinAngle = sun.azimuthMin();
  int sunAzimuthMaxAngle = sun.azimuthMax();

 
//Determine the sunRise time
 int sunRiseHour = (sun.sunrise() / 60) + 2;//add summer time
 int sunRiseMinutes = sun.sunrise() % 60;
 String sunRiseString = String(sunRiseHour) + ":" + String(sunRiseMinutes);
 

 //Determine the sunSet time 
 int sunSetHour = (sun.sunset() / 60)  + 2;//add summer time
 int sunSetMinutes = sun.sunset() % 60;
 String sunSetString = (String(sunSetHour) + ":" + String(sunSetMinutes));
 
// Get the current timestamp
  unsigned long epochTime = timeClient.getEpochTime();

// Convert timestamp to date and time
  String currentDateTime = getFormattedDateTime(epochTime);

//Print the details to the serial Moitor
  Serial.println("currentDay = " + String(currentDateTime)  + "--> Current Sun Altitude Value = " + String(altitudeValue) + " | Current Sun Azimuth Value= " + String(azimuthValue) + " | SundRise = " + sunRiseString + " @ MinAngle = " + sunAzimuthMinAngle + " | SunSet = " + sunSetString + " @ MaxAngle = " + sunAzimuthMaxAngle );
  

// TFT Dispay Setup
// Fill screen with grey so we can see the effect of printing with and without a background colour defined
  tft.fillScreen(TFT_GREY);
  
// Set "cursor" at top left corner of display (0,0) and select font 2
// (cursor will move to next line automatically during printing with 'tft.println'
//  or stay on the line is there is room for the text with tft.print)
  tft.setCursor(0, 0, 2);

// Set the font colour to be white with a black background, set text size multiplier to 1
  tft.setTextColor(TFT_WHITE,TFT_GREY);  

// Set the font colour to be yellow with no background, set to font 7
  tft.setTextColor(TFT_WHITE,TFT_GREY); 
  tft.setTextSize(1);

//TFT Display Text show the current time and sun positions
  tft.setTextSize(2);
  tft.println("Sun Position");
  tft.setTextSize(1);
  tft.println(currentDateTime);
  tft.setTextSize(1);
  tft.print("Altitude = ");
  tft.setTextSize(2);
  tft.println(String(altitudeValue));
  tft.setTextSize(1);
  tft.print("Azimuth = ");
  tft.setTextSize(2);
  tft.println(String(azimuthValue));
  
// Set the servo positions
//If the azimuth is less than the sunRise agnle or > Sunset value then just wait until next morning at SunSet value
   if ( azimuthValue < sun.azimuthMin() || azimuthValue > sun.azimuthMax())  {
    azimuthValue = sun.azimuthMin() ; // the starting point for the sunRize
    tft.setTextSize(1);
   tft.println("Gone to sleep as sun set @ " + sunSetString);
  }
  servoAzimuth.write(azimuthValue);
 
 //At SunSet don't go below horizon
 if (altitudeValue < 0) {
   altitudeValue = 0;
  }  
  servoAltitude.write(altitudeValue);
  delay(10000);
}

String getFormattedDateTime(unsigned long epochTime) {
  // Calculate and display the date and time as a String
  // The NTPClient library does not provide a direct method to retrieve the date. However, you can extract the date from the timestamp obtained using the getEpochTime() function.
  // this function getFormattedDateTime() is therefore added to convert the epoch timestamp to a formatted date and time string. 
  // The date is calculated using the timestamp and then combined with the current time obtained from the getFormattedTime() function. 
  // The getFormattedDateTime() function is used to retrieve the current date and time, which is then printed to the Serial Monitor.
  // The month variable however was iniitally declared as a byte data type, but that cannot handle floating-point calculations. To fix this issue, I needed to change the data type of the month variable to int
  // To avoid an error in calculation of the month I used the floor() function from the math.h library to round down the result of the floating-point division
  // the month variable is declared as an int data type, allowing it to handle the floating-point calculations correctly.  // to convert the result of the floating-point division to an integer before applying the modulo operation
  // the calculation of the daysSince1900 variable. The correct calculation should take into account the leap years. Here's the updated code with the correct date calculatio
  unsigned long daysSince1900 = epochTime / 86400L;
  unsigned long unixTime = (daysSince1900 * 86400L) + (86400L * 365 * 70) + (86400L * 17);
  unsigned long year = 1970;
  unsigned long daysInYear = 365;
  while (daysSince1900 >= daysInYear) {
    year++;
    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
      daysSince1900 -= 366;
      daysInYear = 366;
    } else {
      daysSince1900 -= 365;
      daysInYear = 365;
    }
  }
  
  int month = 1;
  unsigned long daysInMonth = 31;
  while (daysSince1900 >= daysInMonth) {
    month++;
    if (month == 2) {
      if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
        daysSince1900 -= 29;
        daysInMonth = 29;
      } else {
        daysSince1900 -= 28;
        daysInMonth = 28;
      }
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
      daysSince1900 -= 30;
      daysInMonth = 30;
    } else {
      daysSince1900 -= 31;
      daysInMonth = 31;
    }
  }
  
  byte day = daysSince1900 + 1;

  // Get the current time
  String currentTime = timeClient.getFormattedTime();

  // Format the date and time
  String formattedDateTime = String(year) + "-" + String(month) + "-" + String(day) + " " + currentTime;

  return formattedDateTime;
}