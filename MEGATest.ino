#include <QTRSensors.h>

QTRSensorsRC qtrrc((unsigned char[]) {23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53}, 16);

unsigned int sensorValues[16];  
int average[16];
double offFromAverage[16];
double offFromAverageBy[16];

int displayLoop = 0;

byte lineLocation[1]; 

void setup() {
  // put your setup code here, to run once:
  lineLocation[0] = 1;
  Serial.begin(9600);
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
  lineLocation[0] = 0; 
  
  if(displayLoop < 16) {
     for(int i = 0; i < 16; i++) {
      average[i] = (average[i] + sensorValues[i]) / 2;  
    }
    displayLoop++;  
  }
  if(displayLoop == 16) {
    Serial.print("Average Set!");
    Serial.print("\n");
    displayLoop++;  
  }

  for(int i = 0; i < 16; i++) {
    offFromAverage[i] = (double)sensorValues[i] / average[i];
    offFromAverageBy[i] = abs(offFromAverage[i] - 1.0); 
    if(offFromAverage[i] > offFromAverage[lineLocation[0]] ) {
      lineLocation[0] = i; 
    } 
  }

  //Serial.write(lineLocation);
  Serial.write(lineLocation[0]);
  //Serial.print(lineLocation[0]);
  //Serial.print("\n"  );
  //Serial.write("Hello!");
  
  delay(500); 
  
}
