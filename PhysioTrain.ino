/*******************************************************************************
 * PhysioTrain (c) 2017
 * Andreas Bachmann
 * 07.04.2017
 */

#include "PhysioTrain.h"

int i = 0;
int u = 0;
int v = 0;

Quaternion q1;
Quaternion q2;


void setup() {
    PhysioTrain::begin();
    SerialUSB.println("Done!");
}

void loop() {
    
    //SerialUSB.println("loop");
    cli.update();
    if (imuLower.update()) {
        q1 = imuLower.getQuaternion();
    }

    if (imuUpper.update()) {
        q2 = imuUpper.getQuaternion();
    }

    //String line = q1;
    if (u % 20 == 0) {
      String line;
      
      line   = String("[ ");
      line  += String(q1.getW(), 4) + ", ";
      line  += String(q1.getX(), 4) + ", ";
      line  += String(q1.getY(), 4) + ", ";
      line  += String(q1.getZ(), 4);
      line  += String(" ] ");
      

      line  += String("[ ");
      line  += String(q2.getW(), 4) + ", ";
      line  += String(q2.getX(), 4) + ", ";
      line  += String(q2.getY(), 4) + ", ";
      line  += String(q2.getZ(), 4);
      line  += String(" ]");
      SerialUSB.println(line);
    }
}

