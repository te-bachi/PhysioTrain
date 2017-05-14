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

Mode mode;
bool recordValue;
bool startStopValue;
bool startStopSaved = false;

//Output
#define LadeBalkenLow_Pin         14
#define LadeBalkenHIGH_Pin        15  //LED Rechts

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
    
    I2CMux::selectGpioExpander();
    mode           = modeSwitch.getValue();
    recordValue    = recordSwitch.getValue();
    startStopValue = startStopButton.getValue();


    if (startStopValue == HIGH && startStopValue != startStopSaved) {
        startStopSaved = startStopValue;
        model.reinitialize();
    }
    
    if (startStopValue == LOW && startStopValue != startStopSaved) {
        startStopSaved = startStopValue;
    }
    
    if (startStopValue == HIGH) {

        if (u % 20 == 0) {

            I2CMux::selectRtc();
            DateTime now = rtc.now();
            
            String line;
      
            model.update();
            Position pUpper = model.getUpperArmPosition();
            Position pLower = model.getLowerArmPosition();
      
            line   = String("[ ");
            //line  += String(millis());
            
            line  += String(now.year());
            line  += "/";
            line  += String(now.month());
            line  += ('/');
            line  += String(now.day());
            line  += (" ");
            line  += String(now.hour());
            line  += (':');
            line  += String(now.minute());
            line  += (':');
            line  += String(now.second());
            line  += String(" ] ");
            
            line  += String("[ ");
            line  += String(q1.getW(), 4) + ", ";
            line  += String(q1.getX(), 4) + ", ";
            line  += String(q1.getY(), 4) + ", ";
            line  += String(q1.getZ(), 4);
            line  += String(" ] ");
            
            line  += String("[ ");
            line  += String(pUpper.getX(), 4) + ", ";
            line  += String(pUpper.getY(), 4) + ", ";
            line  += String(pUpper.getZ(), 4);
            line  += String(" ] ");
            
            line  += String("[ ");
            line  += String(q2.getW(), 4) + ", ";
            line  += String(q2.getX(), 4) + ", ";
            line  += String(q2.getY(), 4) + ", ";
            line  += String(q2.getZ(), 4);
            line  += String(" ] ");
            
            line  += String("[ ");
            line  += String(pLower.getX(), 4) + ", ";
            line  += String(pLower.getY(), 4) + ", ";
            line  += String(pLower.getZ(), 4);
            line  += String(" ] ");
            
            line  += String(" mode=");
            line  += String(mode.toString());
            
            line  += String(" record=");
            line  += String(recordValue);
            line  += String(" start=");
            line  += String(startStopValue);
            
            SerialUSB.println(line);
            
        }
        u++;
    }
}

