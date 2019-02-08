#include <QTRSensors.h>

QTRSensorsRC qtrrc((unsigned char[]) {23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53}, 16);

unsigned int sensorValues[16];  

int prevAverage[16];

int average[16];

double offFromAverage[16];

int displayLoop = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);
  Serial.print("test\n");
  qtrrc.emittersOn();
  qtrrc.calibrate();
  qtrrc.read(sensorValues);
  for(int i = 0; i < 16; i++) {
    average[i] = sensorValues[i]; 
  }
}

void loop() {
  qtrrc.read(sensorValues);
  for(int i = 0; i < 16; i++) {
    average[i] = (average[i] + sensorValues[i]) / 2;
    offFromAverage[i] = (double)sensorValues[i] / average[i];
  }
  
  
  
  
  
  for(int i = 0; i < 16; i++) {
    Serial.print("\n");    
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(sensorValues[i]);
    Serial.print("\n");    
    Serial.print("Average ");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(offFromAverage[i]);

    
  }
  
  
  delay(500);
  
}
