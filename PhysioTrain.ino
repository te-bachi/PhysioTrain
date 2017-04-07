

#include "config.h"

#include "PhysioTrainLib.h"
/*
#include "EulerAngle.h"
#include "IMU.h"
#include "CLI.h"
#include "Quaternion.h"
*/

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
        if (!isCalib) {
            if (millis() > 2000) {
                angle[i] = EulerAngle(imu->getRoll(), imu->getPitch(), imu->getYaw());
                i++;
                if (i >= calibSize) {
                    float roll  = 0.0f;
                    float pitch = 0.0f;
                    float yaw   = 0.0f;
                    for (int k = 0; k < calibSize; k++) {
                        roll  += angle[k].getRoll();
                        pitch += angle[k].getPitch();
                        yaw   += angle[k].getYaw();
                    }
                    roll /= calibSize;
                    pitch /= calibSize;
                    yaw /= calibSize;
                    calib = EulerAngle(roll, pitch, yaw);
                    isCalib = true;
                }
            }
        } else {
            u++;
            if (u % 100 == 0) {
                Acceleration a = imu->getQuaternion();
                Quaternion q = imu->getQuaternion();
                //String a_e = "e: " + String(imu->getRoll(), 4) + ", " + String(imu->getPitch(), 4) + ", " + String(imu->getYaw(), 4) + "\r\n";
                String a_q = "q: " + String(q._w, 4) + " + " + String(q._x, 4) + " + " + String(q._y, 4) + " + " + String(q._z, 4) + "\r\n";
                //String b = "ref: " + String(calib.getRoll()) + " " + String(calib.getPitch()) + ", " + String(calib.getYaw()) + "\r\n";
                //String c = "rel: " + String(calib.getRoll() - imu->getRoll()) + " " + String(calib.getPitch() - imu->getPitch()) + ", " + String(calib.getYaw() - imu->getYaw()) + "\r\n";
                Serial.print(a_e);
                Serial.print(a_q);
                //Serial.print(b);
                //Serial.print(c);
            }
        }
    }
}
