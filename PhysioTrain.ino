/*******************************************************************************
 * PhysioTrain (c) 2017
 * Andreas Bachmann
 * 07.04.2017
 * 
 * https://github.com/te-bachi/PhysioTrain
 */

#include "PhysioTrain.h"

int i = 0;
int u = 0;
int v = 0;

void setup() {
    PhysioTrain::begin();
    SerialUSB.println("Done!");
}

void loop() {
    bool imuLowerUpdate = false;
    bool imuUpperUpdate = false;
    
    //SerialUSB.println("loop");
    cli.update();
    
    imuLowerUpdate = imuLower.update();
    if (imuLowerUpdate) {
        q1 = imuLower.getQuaternion();
    }

    imuUpperUpdate = imuUpper.update();
    if (imuUpperUpdate) {
        q2 = imuUpper.getQuaternion();
    }
    
    if (imuLowerUpdate || imuUpperUpdate) {
        model.update();
    }
    
    I2CMux::selectGpioExpander();
    mode           = modeSwitch.getValue();
    recordValue    = recordSwitch.getToggleValue();
    startStopValue = startStopButton.getValue();

    //if (recordSwitch.getPosition()) {
    //    vibra.start(2000);
    //}
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
        SerialUSB.print(stateMachine.toString());
        if (stateMachine.getState() == StateMachine::State::ERROR) {
                SerialUSB.print(" mode=");
                SerialUSB.print(mode.toString());
                SerialUSB.print(" record=");
                SerialUSB.print(recordValue);
                SerialUSB.print(" start=");
                SerialUSB.print(startStopValue);
        }
        SerialUSB.println("");
    }

    if (v % 120 == 0) {
        accu.checkLevel();
    }
    v++;
}
 
