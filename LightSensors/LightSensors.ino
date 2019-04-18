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

// Practice Field Calibration 2019-04-16
unsigned int CalibratedMinVals[] = {736, 260, 348, 356, 180, 176, 268, 176, 440, 348, 260, 256, 348, 348, 348, 440, };
unsigned int CalibratedMaxVals[] = {1280, 436, 528, 620, 344, 256, 436, 256, 712, 436, 428, 264, 436, 436, 436, 536, };

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
