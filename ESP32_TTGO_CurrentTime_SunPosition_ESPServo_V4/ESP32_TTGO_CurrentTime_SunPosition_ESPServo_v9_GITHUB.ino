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
 
   Updates: 
   29.10.2023 - Added Button 0 which now moves azomuth servo from start value to 180-->0-->sart value

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
int pos = 0;    // swipe routine
int y = 0;       // swipe routine
float gearRatio = 0.7;  //  gear ratio for swipe routine
float sx, sy; // Declare variables for Cartesian coordinates
int x0, yy0, x1, yy1; // Declare variables for line coordinates

//connect to the internet
char ssid[] = "Enter your SSID";
char pass[] = "Enter your password";
WiFiUDP ntpUDP;
Servo servoAzimuth; //Define two servo objects and create a servo instance for each servo:
Servo servoAltitude; //Define two servo objects and create a servo instance for each servo:


// You can specify the time server pool and the offset (in seconds, can be
// changed later with setTimeOffset() ). Additionally you can specify the
// update interval (in milliseconds, can be changed using setUpdateInterval() ).
//NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000); //de.pool.ntp.org
NTPClient timeClient(ntpUDP, "de.pool.ntp.org", 7200, 60000); //7200 for summer time and 3600 for winter

//setup buttons on pin 0 and pint 35
// Define the pin number for the button
const int buttonPin35 = 35;
const int buttonPin00 = 0;

// Flag to indicate whether the button was pressed
volatile bool buttonPressed35 = false;
volatile bool buttonPressed00 = false;


// When button 0 is pressed, Define the duration for the swipe motion in milliseconds
const int swipeDuration = 5000; // 5 seconds (adjust as needed)

// Variables to store initial positions of the servos
int initialAzimuthPosition;//not needed?
int initialAltitudePosition;//not needed?
int azimuthValue; // Global variable to store current sun azimuth value
int altitudeValue; // Global variable to store current sun altitude value
// Global variables for azimuth min and max angles (in degrees)
int sunAzimuthMinAngle = 45;
int sunAzimuthMaxAngle = 135;
String getFormattedDateTime(unsigned long epochTime);



// Interrupt service routine to handle button press
void IRAM_ATTR handleButtonPress35() {
  // Store current positions of the servos
  //initialAzimuthPosition = servoAzimuth.read();
  //initialAltitudePosition = servoAltitude.read();
  //Serial.println("initialAzimuthPosition Angle =" + String(initialAzimuthPosition));
  //Serial.println("initialAltitudePosition Angle =" + String(initialAltitudePosition));
  //Serial.println("current AzimuthPosition Angle =" + (azimuthValue));
  //Serial.println("current AltitudePosition Angle =" + (altitudeValue));

  //If button pressed then send a  notification on the TFT displaz
    tft.setTextSize(1);
    tft.println("Please Wait!" );
    //swipe(azimuthValue,altitudeValue);
  // Call the swipe() function when the button is pressed
  // Interrupt service routine to handle button press
  buttonPressed35= true;
}

// Interrupt service routine to handle button press
void IRAM_ATTR handleButtonPress00() {
  // Store current positions of the servos
  //initialAzimuthPosition = servoAzimuth.read();
  //initialAltitudePosition = servoAltitude.read();
  //Serial.println("initialAzimuthPosition Angle =" + String(initialAzimuthPosition));
  //Serial.println("initialAltitudePosition Angle =" + String(initialAltitudePosition));
  //Serial.println("current AzimuthPosition Angle =" + (azimuthValue));
  //Serial.println("current AltitudePosition Angle =" + (altitudeValue));

  //If button pressed then send a  notification on the TFT displaz
    tft.setTextSize(1);
    tft.println("Please Wait!" );
  //swipe(azimuthValue,altitudeValue);
  // Call the swipe() function when the button is pressed
  // Interrupt service routine to handle button press
  buttonPressed00= true;
}

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

  //servoAzimuth.attach(12); //  AZIMUTH_SERVO_PIN 
  //servoAltitude.attach(13); // Altitude_SERVO_PIN 
  // Attach servos to their respective pins
  servoAzimuth.attach(12, 500, 2400);
  servoAltitude.attach(13, 500, 2400);
  
  

  // Setup the button pin as input with internal pull-up resistor
  pinMode(buttonPin35, INPUT_PULLUP);
  pinMode(buttonPin00, INPUT_PULLUP);

   // Attach the interrupt to the button pin (RISING or FALLING depending on your button wiring)
  attachInterrupt(digitalPinToInterrupt(buttonPin35), handleButtonPress35, FALLING);

   // Attach the interrupt to the button pin (RISING or FALLING depending on your button wiring)
  attachInterrupt(digitalPinToInterrupt(buttonPin00), handleButtonPress00, FALLING);
 
 
}



/********************************************* 
                Main Loop 
**********************************************/
void loop() {
  // Check if the button was pressed
  

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
  
// declare & update current sun position variables
  //int azimuthValue = sun.azimuth(); // Get azimuth value from SunPosition
  //int altitudeValue = sun.altitude(); // Get altitude value from SunPosition
 sunAzimuthMinAngle = sun.azimuthMin(); //assign to variable
 sunAzimuthMaxAngle = sun.azimuthMax(); //assign to variable
 azimuthValue = sun.azimuth(); //assign to variable
altitudeValue = sun.altitude();//assign to variable
  
 
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
// Set "cursor" at top left corner of display (0,0) and select font 2

// Fill screen with grey so we can see the effect of printing with and without a background colour defined
 tft.fillScreen(TFT_BLACK);
  

// Set the font colour to be white with a black background, set text size multiplier to 1
  tft.setTextColor(TFT_WHITE,TFT_BLACK); 


/*TFT Display Text show the current time and sun positions
Here are the approximate line increments for different font sizes:

For text size 1, the height of the font is approximately 8 pixels. So, the next line position would be y + 8.
For text size 2, the height of the font is approximately 16 pixels. So, the next line position would be y + 16.
For text size 3, the height of the font is approximately 24 pixels. So, the next line position would be y + 24.
*/
  //TFT Header Text
  tft.setTextSize(2);
  tft.setCursor(0, 0, 2);
  tft.print("Sun Position");

  
  // Next Line
  tft.setTextSize(1);
  tft.setCursor(0, 35, 1); //Y=10 and text size 1
  tft.print(currentDateTime);
  
  // Next Line
   tft.setTextSize(1);
  tft.setCursor(0, 50, 1); 
  tft.print("Altitude: ");
   tft.setCursor(60, 50, 2); 
  tft.setTextSize(2);
  tft.print(String(altitudeValue));
  //tft.fillCircle(200, 55, 25, TFT_GREEN);
  //tft.fillCircle(200, 55, 20, TFT_BLACK);
  
  // Next Line
  tft.setTextSize(1);
  tft.setCursor(0, 85, 1); 
  tft.print("Azimuth: ");
  tft.setCursor(60, 85, 2); 
  tft.setTextSize(2);
  tft.print(String(azimuthValue));
   
  





  // Draw a circle with center at (200, 67) and radius 55
    tft.fillCircle(180, 67, 55, TFT_BLUE);
    tft.fillCircle(180, 67, 53, TFT_BLACK);

    // Grey out the specified azimuth range
 // Grey out the specified azimuth range
 // drawFilledCircleSector(200, 67, 55, sunAzimuthMinAngle , sunAzimuthMaxAngle , TFT_BLUE);

// Draw lines radiating from the center of the circle
    for(int i = 0; i < 360; i += 10) {
        // Convert polar coordinates to Cartesian coordinates
        sx = cos((i - 90) * 0.0174532925);
        sy = sin((i - 90) * 0.0174532925);
        
        // Calculate the starting point of the line (x0, yy0) and the ending point of the line (x1, yy1)
        x0 = sx * 60 + 180; // x-coordinate of the starting point
        yy0 = sy * 60 + 67;  // y-coordinate of the starting point
        x1 = sx * 45 + 180; // x-coordinate of the ending point (shorter length)
        yy1 = sy * 45 + 67;  // y-coordinate of the ending point (shorter length)

        // Draw the line on the TFT display using the calculated points
        tft.drawLine(x0, yy0, x1, yy1, TFT_WHITE);
    }


    // Calculate azimuth angle in degrees (replace this with your actual azimuth value)  
    // Convert azimuth angle to radians
    float azimuthRad = (azimuthValue- 90) * 0.0174532925;
   
    // Calculate endpoint coordinates based on azimuth direction and circle radius
    int lineLength = 55; // Length of the line from the center to the endpoint
    int endpointX = 180 + cos(azimuthRad) * lineLength;
    int endpointY = 67 + sin(azimuthRad) * lineLength;

    // Draw a line from the center of the circle to the endpoint in the azimuth direction
    tft.drawLine(180, 67, endpointX, endpointY, TFT_RED);
  
 //display daylight
  //void drawFilledCircleSector(int32_t x, int32_t y, int32_t radius, float startAngle, float endAngle, uint32_t color) {
   // Draw lines radiating from the center of the circle
    for(int i = sunAzimuthMaxAngle ; i < sunAzimuthMinAngle + 360 ; i += 1) {
    // Convert polar coordinates to Cartesian coordinates
    sx = cos((i - 90) * 0.0174532925);
    sy = sin((i - 90) * 0.0174532925);
    



    // Calculate the starting point of the line (x0, yy0) and the ending point of the line (x1, yy1)
    x0 = sx * 60 + 180; // x-coordinate of the starting point
    yy0 = sy * 60 + 67;  // y-coordinate of the starting point
    x1 = sx * 45 + 180; // x-coordinate of the ending point (shorter length)
    yy1 = sy * 45 + 67;  // y-coordinate of the ending point (shorter length)

    // Draw the line on the TFT display using the calculated points
    tft.drawLine(x0, yy0, x1, yy1, TFT_GREY); // Use TFT_BLACK to represent night hours
    //tft.drawLine(180, 67, x1, yy1, TFT_GREY); // Use TFT_BLACK to represent night hours
}


  
// Set the servo positions
//If the azimuth is less than the sunRise agnle or > Sunset value then just wait until next morning at SunSet value
   if ( azimuthValue < sun.azimuthMin() || azimuthValue > sun.azimuthMax())  {
   // azimuthValue = sun.azimuthMin() ; // the starting point for the sunRize
    //If our of range due to sunset, then send a  notification on the TFT displaz
   // tft.setTextSize(1);
   // tft.println("Gone to sleep as sun set @ " + sunSetString);
  }
  int servoAngle=map(azimuthValue,sunAzimuthMinAngle, sunAzimuthMaxAngle, 1800, 0);
  
   
  //Serial.println("ServoAngle = " + servoAngle + "  azimuthValue = " + azimuthValue);

  servoAzimuth.write(servoAngle);
 
 //At SunSet don't go below horizon
 if (altitudeValue < 0) {
   altitudeValue = 0;
  }  
  servoAltitude.write(altitudeValue);


  
    

  if (buttonPressed00) {
    // Call the swipe function or perform any action here
    //swipe(initialAzimuthPosition, initialAltitudePosition);
    //Print the details to the serial Moitor
    tft.println("Swipe = " + String(azimuthValue) +" : " + String(altitudeValue));
    Serial.println("PreSwipe - Current Sun Altitude Value = " + String(altitudeValue) + " | Current Sun Azimuth Value= " + String(azimuthValue)  );
    servoAzimuth.write(0);
  delay(2000);
  servoAzimuth.write(180);
  delay(2000);
   servoAzimuth.write(azimuthValue);
    // Reset the buttonPressed flag
  buttonPressed00 = false;
  }

  if (buttonPressed35) {
    // Call the swipe function or perform any action here
    //swipe(initialAzimuthPosition, initialAltitudePosition);
    //Print the details to the serial Moitor
    tft.println("35 = " + String(azimuthValue) +" : " + String(altitudeValue));
    Serial.println("PreSwipe - Current Sun Altitude Value = " + String(altitudeValue) + " | Current Sun Azimuth Value= " + String(azimuthValue)  );
    swipe(azimuthValue, altitudeValue);//parse currrent start values to the swipe()
    // Reset the buttonPressed flag
    buttonPressed35 = false;
  }
  // Detach servos during the delay period to save power
  //servoAzimuth.detach();
  //servoAltitude.detach();

  // Delay for 10 seconds
  delay(10000);

  // Reattach servos before moving them again
  // servoAzimuth.attach(13, 500, 2400);
  // servoAltitude.attach(12, 500, 2400);
}



/********************************************* 
               swipe()
**********************************************/
/*In this code, when the button is pressed, the handleButtonPress() function stores the current positions of the servos. 
The swipe() function then gradually moves the servos from their initial positions to 180 degrees over the specified swipeDuration. 
The interpolation factor determines the position of the servos at each step of the motion. The delay(15) controls 
the speed of the motion; you can adjust this value to make the motion faster or slower.*/
void swipe(int startAzimuth, int startAltitude) {
  //send notifications to TFT
  tft.setTextSize(1);
  tft.println("Entered Swipe Routine");


// azimuth swipe  servo back from start -->180-->0-->start degress)

  // Swipe azimuth servo to 180 degrees
  for (int angle = startAzimuth; angle <= 180; angle++) {
    servoAzimuth.write(angle);
    delay(15); // Delay to control the speed of the motion
  }

  // Swipe azimuth servo to 0 degrees
  for (int angle = 180; angle >= 0; angle--) {
    servoAzimuth.write(angle);
    delay(15); // Delay to control the speed of the motion
  }

// Swipe azimuth servo back to start degrees
  for (int angle = 0; angle <= startAzimuth; angle++) {
    servoAzimuth.write(angle);
    delay(15); // Delay to control the speed of the motion
  }

// altitude swipe  servo back from start -->180-->0-->start degress

  // Swipe altitude servo to 180 degrees
  for (int angle = startAltitude; angle <= 180; angle++) {
    servoAltitude.write(angle);
    delay(15); // Delay to control the speed of the motion
  }

  // Swipe altitude servo back to 0 degrees
  for (int angle = 180; angle >= 0; angle--) {
    servoAltitude.write(angle);
    delay(15); // Delay to control the speed of the motion
  }

  // Swipe altitude servo back to 0 degrees
  for (int angle = 0; angle >= startAltitude; angle++) {
    servoAltitude.write(angle);
    delay(15); // Delay to control the speed of the motion
  }

  // Move the servos back to the initial positions
  servoAzimuth.write(startAzimuth);
  servoAltitude.write(startAltitude);


  tft.setTextSize(1);
  tft.println("Leaving Swipe Routine");
  delay(2000);
}


/********************************************* 
         getFormattedDateTime()
**********************************************/

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


void drawFilledCircleSector(int32_t x, int32_t y, int32_t radius, float startAngle, float endAngle, uint32_t color) {
    // Convert angles from degrees to radians
    startAngle = radians(360 - startAngle); // Reverse the angle to match TFT display orientation
    endAngle = radians(360 - endAngle);     // Reverse the angle to match TFT display orientation

    // Calculate center coordinates of the circle
    int32_t cx = x;
    int32_t cy = y;

    // Draw lines to fill the circle sector
    for (float angle = startAngle; angle <= endAngle; angle += radians(1)) {
        // Calculate endpoint coordinates based on angle and radius
        int32_t x1 = cx + cos(angle) * radius;
        int32_t y1 = cy + sin(angle) * radius;

        // Draw a line from the center to the calculated endpoint
        tft.drawLine(cx, cy, x1, y1, color);
    }
}
