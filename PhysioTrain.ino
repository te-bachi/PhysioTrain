

#include "config.h"

#include "PhysioTrainLib.h"

CLI  cli;
IMU* imu;

const int calibSize = 10;
EulerAngle angle[calibSize];
EulerAngle  calib;
bool isCalib = false;
int i = 0;
int u = 0;

void setup() {
    cli.begin();
    
    // Set up serial log port
    imu = IMU::instance();
    imu->begin();

}

void loop() {
    //Serial.println("loop");
    cli.update();
    if (imu->update()) {
        u++;
        //if (u % 10 == 0) {
            Acceleration a = imu->getAcceleration();
            //EulerAngle e = imu->getEulerAngle();
            Quaternion q = imu->getQuaternion();
            
            String str = "";
            
            str += String(a.getX(), 4) + ", ";
            str += String(a.getY(), 4) + ", ";
            str += String(a.getZ(), 4) + ", ";

            /*
            str += String(e.getRoll(),  4) + ", ";
            str += String(e.getPitch(), 4) + ", ";
            str += String(e.getYaw(),   4) + ", ";
            */
            
            str += String(q.getW(), 4) + ", ";
            str += String(q.getX(), 4) + ", ";
            str += String(q.getY(), 4) + ", ";
            str += String(q.getZ(), 4) + "\r\n";
            
            Serial.print(str);
        //}
    }
}
