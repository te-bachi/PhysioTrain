/*******************************************************************************
 * PhysioTrain (c) 2017
 * Andreas Bachmann
 * 07.04.2017
 */

#include "PhysioTrain.h"

int i = 0;
int u = 0;
int v = 0;

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
    recordValue    = recordSwitch.getToggleValue();
    startStopValue = startStopButton.getValue();

    if (recordSwitch.getPosition()) {
        vibra.start(2000);
    }
    vibra.tryStop();


    if (startStopValue == HIGH && startStopValue != startStopSaved) {
        startStopSaved = startStopValue;
        model.reinitialize();
    }
    
    if (startStopValue == LOW && startStopValue != startStopSaved) {
        startStopSaved = startStopValue;
    }
    
    stateMachine.run(mode, startStopValue);
    if (stateMachine.getState() != state) {
        state = stateMachine.getState();
        SerialUSB.println(stateMachine.toString());
    }
}

