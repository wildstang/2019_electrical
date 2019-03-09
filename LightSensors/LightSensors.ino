#include "QTRSensors.h"

//Positioned returned to the RIO
unsigned int position;


//light sensor minimum calibration value - debug only
//#define MIN_LIGHT_CAL 0

//light sensor minimum calibration value - debug only
//#define MAX_LIGHT_CAL 15000

//Light Calibration Arrays - debug only
//unsigned int CalibratedMinVals[] = {MIN_LIGHT_CAL, MIN_LIGHT_CAL, MIN_LIGHT_CAL, MIN_LIGHT_CAL, MIN_LIGHT_CAL, MIN_LIGHT_CAL, MIN_LIGHT_CAL, MIN_LIGHT_CAL, MIN_LIGHT_CAL, MIN_LIGHT_CAL, MIN_LIGHT_CAL, MIN_LIGHT_CAL, MIN_LIGHT_CAL, MIN_LIGHT_CAL, MIN_LIGHT_CAL, MIN_LIGHT_CAL}; 
//unsigned int CalibratedMaxVals[] = {MAX_LIGHT_CAL, MAX_LIGHT_CAL, MAX_LIGHT_CAL, MAX_LIGHT_CAL, MAX_LIGHT_CAL, MAX_LIGHT_CAL, MAX_LIGHT_CAL, MAX_LIGHT_CAL, MAX_LIGHT_CAL, MAX_LIGHT_CAL, MAX_LIGHT_CAL, MAX_LIGHT_CAL, MAX_LIGHT_CAL, MAX_LIGHT_CAL, MAX_LIGHT_CAL, MAX_LIGHT_CAL}; 



// Midwest Real Field Calibration - 2019-3-8
unsigned int CalibratedMinVals[] = {280, 352, 724, 352, 280, 216, 280, 352, 352, 284, 420, 420, 280, 420, 496, 420}; 
unsigned int CalibratedMaxVals[] = {348, 492, 956, 572, 416, 344, 416, 492, 492, 420, 648, 644, 420, 568, 648, 492}; 


// Midwest OLD Field Calibration - 2019-3-8 - Debug Only
//unsigned int CalibratedMinVals[] = {424, 352, 644, 348, 216, 216, 216, 212, 356, 420, 420, 420, 420, 492, 700, 636}; 
//unsigned int CalibratedMaxVals[] = {1072, 920, 1388, 772, 344, 416, 416, 344, 700, 784, 864, 852, 776, 1008, 1412, 1100}; 

//Light sensor array locations
QTRSensorsRC lightSensors((unsigned char[]) {23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53}, 16);

//Array of sensor values
unsigned int sensorValues[16];

char linePositionByte; 

void setup() 
{
  //Initialize Position
  position = 0;

  //Define Serial speed baud
  Serial.begin(9600);
  Serial.print("Arduino Activated\n");

  //Manual calibration mode uses real time data to auotmaticly calibrate
  //manualCalibrate(); 

  //auto calibrate uses the calibration values previously measured and loaded into the arrays
  autoCalibrate(CalibratedMaxVals, CalibratedMinVals);  
}

void loop() 
{  
  // read calibrated sensor values and obtain a measure of the line position from 0 to 15000
  position = lightSensors.readLine(sensorValues, QTR_EMITTERS_ON, true); 

  /*
    for (unsigned char i = 0; i < 16; i++)
    {
      Serial.print(sensorValues[i]);
      Serial.print(' ');
    }
   Serial.println();
   Serial.print("POSITION");
   Serial.println(); 
   */  
     
   //char to write to the serial bus
   linePositionByte = position / 60; 

   //write the char to the serial bus
   Serial.write(linePositionByte);

   //delay of 20ms makes this work
   delay(20);
}

void manualCalibrate()
{
  digitalWrite(13, HIGH);    // turn on Arduino's LED to indicate we are in calibration mode
  for (int i = 0; i < 400; i++)  // make the calibration take about 10 seconds
  {
    lightSensors.calibrate();
  }
  digitalWrite(13, LOW);     // turn off Arduino's LED to indicate we are through with calibration

  // print the calibration minimum values measured when emitters were on
    for (int i = 0; i < 16; i++)
  {
    Serial.print(lightSensors.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();

  // print the calibration maximum values measured when emitters were on
    for (int i = 0; i < 16; i++)
  {
    Serial.print(lightSensors.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
}

void autoCalibrate(unsigned int *maxValsList, unsigned int *minValsList)
{
  lightSensors.calibratePreset(maxValsList, minValsList);

  // print the calibration minimum values measured when emitters were on
    for (int i = 0; i < 16; i++)
  {
    Serial.print(lightSensors.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();

  // print the calibration maximum values measured when emitters were on
    for (int i = 0; i < 16; i++)
  {
    Serial.print(lightSensors.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
}
