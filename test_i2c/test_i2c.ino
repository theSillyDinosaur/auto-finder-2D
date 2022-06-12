/*
new_version_of_Basic_I2C.ino
Brian R Taylor
brian.taylor@bolderflight.com

Copyright (c) 2017 Bolder Flight Systems

Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
and associated documentation files (the "Software"), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, 
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is 
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or 
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"

// an MPU9250 object with the MPU-9250 sensor on I2C bus 0 with address 0x68
MPU6050 IMU(0x68);

int16_t ax, ay, az, gx, gy, gz;

int mean_ax, mean_ay, mean_az, mean_gx, mean_gy, mean_gz, state = 0;
int ax_offset, ay_offset, az_offset, gx_offset, gy_offset, gz_offset;

void setup() {
  // join I2C bus (I2Cdev library doesn't do this automatically)
  Wire.begin();
  // COMMENT NEXT LINE IF YOU ARE USING ARDUINO DUE
  TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz). Leonardo measured 250kHz.

  // initialize serial communication
  Serial.begin(115200);

  // initialize device
  IMU.initialize();

  // wait for ready
  while (Serial.available() && Serial.read()); // empty buffer again

  // verify connection
  Serial.println(IMU.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  delay(1000);
  // reset offsets
  IMU.setXAccelOffset(-5826);
  IMU.setYAccelOffset(-2884);
  IMU.setZAccelOffset(441);
  IMU.setXGyroOffset(163);
  IMU.setYGyroOffset(56);
  IMU.setZGyroOffset(1);
}
int16_t a_Accel[3]={0, 0, 0};
int16_t GyroX, GyroY, GyroZ;
double p_Gyro=0;
double dt;
int n = 100;
double t_cur, t_pre;

void loop() {
  // read the sensor
  IMU.getMotion6(&a_Accel[0], &a_Accel[1], &a_Accel[2], &GyroX, &GyroY, &GyroZ);
  // display the data
  t_cur = micros();
  dt=((double)(t_cur-t_pre))/1000000;
  t_pre = t_cur;
  p_Gyro+=GyroZ*dt;
  Serial.println(GyroZ);
  
}
