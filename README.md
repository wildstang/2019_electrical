# 2019_line_sensor
Arduino code for line sensor.

This master branch contains the calibration and line sensor folder used at practice after 7 rivers.  It has been tested to be fuctional, and should be the basis for any modifications to the line sensor program moving forwards. 

Calibration - To calibrate the light sensors, download and run the calibration program on the arduino app.  Before deploying to the Arduino, make sure you are deploying to an Arduino Mega on the correct port, which can be set in the tools tab in the app.  Click the arrow button to deploy, and open the serial monitor in the tools tab.  When numbers begin to count up, run the robot back and forth over the white line so that all sensors can see the line and carpet.  AFter the countup reaches x, the serial monitor will  print the variable declarations that should be copy and pasted into the line sensor program, replacing the previous variables.  The serial monitors will also show the differnce between the lightest and darkest values for each sensor, so a value of 0 means a faulty sensor. 
AFTER CALIBRATING, MAKE SURE TO RE-DELPOY THE NEW CALIBRATED SENSOR CODE TO THE ARDUINO
*note, the <QRTSensors> header will only work if your computer has that library installed, if you get a compile error saying you computer cannot find this library, you may need to include the "qtrsensor.cpp" and "qtrsensor.h" files located in the main line sensor folder, as well as changing <QTRSensor> to "QTRSensors.h"

