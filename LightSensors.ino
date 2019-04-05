#include <QTRSensors.h>
QTRSensorsRC qtrrc((unsigned char[]) {23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53}, 16); // 16 sensor array
//QTRSensorsRC li ((unsigned char[]) {23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 47, 49, 51}, 14); // 14 sensor array 

//ommitted sensors 11 - 15

void setup() {
    Serial.begin(9600);
    Serial.print("test");
    qtrrc.emittersOn();
    for(int i = 0; i < 400; i++) {
        qtrrc.calibrate();
        delay(20);
        Serial.println(i);
         
    }

    Serial.begin(9600);
    Serial.println("finished\n");
    Serial.print("unsigned int CalibratedMinVals[] = {");
    for(int i = 0; i < 16; i ++) {
      Serial.print(qtrrc.calibratedMinimumOn[i]);
      Serial.print(", ");
    }
    Serial.print("};\nunsigned int CalibratedMaxVals[] = {");
    for(int i = 0; i < 16;i ++) {
      Serial.print(qtrrc.calibratedMaximumOn[i]);
      Serial.print(", ");
    }
    Serial.print("};\n");
    Serial.println("Light Differences");
    for(int i = 0; i < 16; i++) {
      Serial.print(i);
      Serial.print(": ");
      Serial.println(qtrrc.calibratedMaximumOn[i] - qtrrc.calibratedMinimumOn[i]);
    }




    

    

   /*
    for(int i = 0; i < 16; i++ ) {
       Serial.print("Minimum Value ");
       Serial.print(i);
       Serial.print(": ");
       Serial.print(qtrrc.calibratedMinimumOn[i]);
       Serial.print("\n");
    }
    for(int i = 0; i < 16; i++ ) {
       Serial.print("Maximum Value ");
       Serial.print(i);
       Serial.print(": ");
       Serial.print(qtrrc.calibratedMaximumOn[i]);
       Serial.print("\n");
    }
    */

}

void loop() {
  //Serial.print("Loop entered");
}
