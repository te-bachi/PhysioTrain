/*******************************************************************************
 * PhysioTrain (c) 2017
 * Andreas Bachmann
 * 07.04.2017
 */

#include "wiring_private.h" // pinPeripheral() function
#include "config.h"

#include "PhysioTrainLib.h"

CLI             cli;
IMU             imu(IMU_SELECT_INTERNAL);
IMU             muxImu(IMU_SELECT_EXTERNAL);
KinematicModel  model;

TwoWire muxWire(&sercom1, 11, 13);

int i = 0;
int u = 0;
int v = 0;

String q1;
String q2;


void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    cli.begin();

    Wire.begin();
    muxWire.begin();        // join i2c bus (address optional for master)
    pinPeripheral(11, PIO_SERCOM);
    pinPeripheral(13, PIO_SERCOM);
    
    delay(5000);
    Serial.println("Hallo");
    
    // Set up serial log port
    imu.begin();
    muxImu.begin();
    Serial.println("Hallo");

    //model.begin(&Wire, &imu);
    Serial.println("Hallo");
}
void loop() {
    Position p(1.0f, 2.0f, 3.0f);
    
    //Serial.println("loop");
    cli.update();
    if (imu.update()) {
        u++;
        if (u % 20 == 0) {
            Quaternion q = imu.getQuaternion();

            //String q1;
            q1   = String("[ ");
            q1  += String(q.getW(), 4) + ", ";
            q1  += String(q.getX(), 4) + ", ";
            q1  += String(q.getY(), 4) + ", ";
            q1  += String(q.getZ(), 4);
            q1  += String(" ]");
            
            //Serial.print(q1);
        }
    }

    if (muxImu.update()) {
        v++;
        if (v % 20 == 0) {
            Quaternion q = muxImu.getQuaternion();

            q2   = String("[ ");
            q2  += String(q.getW(), 4) + ", ";
            q2  += String(q.getX(), 4) + ", ";
            q2  += String(q.getY(), 4) + ", ";
            q2  += String(q.getZ(), 4);
            q2  += String(" ]");
            
        }
    }

    //String line = q1;
    if (u % 20 == 0) {
      String line = q1 + " " + q2 + "\r\n";
      Serial.print(line);
    }
}
