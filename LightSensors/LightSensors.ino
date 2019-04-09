#include "QTRSensors.h"

//Positioned returned to the RIO
unsigned int position;

//last position returned to the RIO
unsigned int lastPosition;

//number of loops to average
#define AVERAGE_LOOPS 3

#define SCALING_VALUE 2

//deadband
#define DEAD_BAND 10

//light sensor minimum calibration value - debug only
//#define MIN_LIGHT_CAL 0

//light sensor minimum calibration value - debug only
//#define MAX_LIGHT_CAL 15000

//Light Calibration Arrays - debug only
//unsigned int CalibratedMinVals[] = {MIN_LIGHT_CAL, MIN_LIGHT_CAL, MIN_LIGHT_CAL, MIN_LIGHT_CAL, MIN_LIGHT_CAL, MIN_LIGHT_CAL, MIN_LIGHT_CAL, MIN_LIGHT_CAL, MIN_LIGHT_CAL, MIN_LIGHT_CAL, MIN_LIGHT_CAL, MIN_LIGHT_CAL, MIN_LIGHT_CAL, MIN_LIGHT_CAL, MIN_LIGHT_CAL, MIN_LIGHT_CAL}; 
//unsigned int CalibratedMaxVals[] = {MAX_LIGHT_CAL, MAX_LIGHT_CAL, MAX_LIGHT_CAL, MAX_LIGHT_CAL, MAX_LIGHT_CAL, MAX_LIGHT_CAL, MAX_LIGHT_CAL, MAX_LIGHT_CAL, MAX_LIGHT_CAL, MAX_LIGHT_CAL, MAX_LIGHT_CAL, MAX_LIGHT_CAL, MAX_LIGHT_CAL, MAX_LIGHT_CAL, MAX_LIGHT_CAL, MAX_LIGHT_CAL}; 



// Midwest Real Field Calibration - 2019-3-8
unsigned int CalibratedMinVals[] = {688, 424, 424, 604, 512, 176, 684, 700, 768, 596, 768, 600, 768, 852, 764, 844}; 
unsigned int CalibratedMaxVals[] = {1392, 768, 852, 1408, 944, 344, 1408, 1500, 1584, 1212, 1572, 1292, 1492, 1492, 1296, 1212}; 


// Midwest OLD Field Calibration - 2019-3-8 - Debug Only
//unsigned int CalibratedMinVals[] = {424, 352, 644, 348, 216, 216, 216, 212, 356, 420, 420, 420, 420, 492, 700, 636}; 
//unsigned int CalibratedMaxVals[] = {1072, 920, 1388, 772, 344, 416, 416, 344, 700, 784, 864, 852, 776, 1008, 1412, 1100}; 

//Light sensor array locations
QTRSensorsRC lightSensors((unsigned char[]) {23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53}, 16);

//Array of sensor values
unsigned int sensorValues[16];

char returnSensorValues[17];

char linePositionByte; 

void setup() 
{
    //Initialize Positions
    position = 0;
    lastPosition = 0;

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
    //loop to sum position readings together
    for( unsigned int i=0 ; i < AVERAGE_LOOPS ; i++)
    {
    position += readSensors();    
    }


    //average values
    position = position / AVERAGE_LOOPS;


    //check if the differance from the last position to the current position is less than the deadband
    if( DEAD_BAND < abs( position - lastPosition ) )
    {
    //If the values are in the deadband,then we return the last position 
    linePositionByte = lastPosition / SCALING_VALUE;
    }
    else
    {
    //If the values are NOT in the deadband, then we return the new position 
    linePositionByte = position / SCALING_VALUE;    

    //set the last position to the current position;
    lastPosition = position;
    }

     */

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

    // char to write to the serial bus
    // FIXME: SCALING_VALUE probably should be different for linePositionByte.
    linePositionByte = position / SCALING_VALUE; 

    // Our protocol for messages from line sensor to Arduino:
    // byte 0: 0xFF 
    // bytes 1-16: values for light sensors (all <0xFF)
    // byte 17: the QTRR library's determination of line position from the data
    //          (<0xFF)
    // So the only byte that is 0xFF is the start byte, allowing for easy
    // synchronization.

    //clamp value to 254 max
    if(linePositionByte > 254)
    {
        linePositionByte = 254;
    }


    for (unsigned char i = 0; i < 16; i++)
    {
        returnSensorValues[i] = sensorValues[i] / SCALING_VALUE;      

        //clamp value to 254 max
        if(returnSensorValues[i] > 254)
        {
            returnSensorValues[i] = 254;
        }

    }

    returnSensorValues[16] = linePositionByte;

    Serial.write(-1);
    Serial.write(returnSensorValues, 17);   


    // write the char to the serial bus
    // Serial.write(linePositionByte);

    // delay of 5ms --- this seems to help, don't take it out. Maybe it doesn't
    // have to be 5 whole ms.
    delay(5);
}

unsigned int readSensors()
{
    return lightSensors.readLine(sensorValues, QTR_EMITTERS_ON, true);  
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
