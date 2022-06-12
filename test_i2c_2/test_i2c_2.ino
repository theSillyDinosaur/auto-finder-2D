#include "MPU9250.h"
#include <math.h>

// an MPU9250 object with the MPU-9250 sensor on I2C bus 0 with address 0x68
MPU9250 IMU(Wire,0x68);
int status;
int calibrated;

double init_GyroZ=0;
unsigned long t_pre = 0, t_cur = 0;

void setup() {
  // serial to display data
  Serial.begin(115200);
  while(!Serial) {}

  // start communication with IMU 
  status = IMU.begin();
  calibrated = IMU.calibrateAccel();
  
  // setting the accelerometer full scale range to +/-8G 
  IMU.setAccelRange(MPU9250::ACCEL_RANGE_8G);
  // setting the gyroscope full scale range to +/-500 deg/s
  IMU.setGyroRange(MPU9250::GYRO_RANGE_500DPS);
  // setting DLPF bandwidth to 20 Hz
  IMU.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_20HZ);
  // setting SRD to 19 for a 50 Hz update rate
  int Hz = 1000;
  IMU.setSrd(1000/Hz-1);
  
  // re:0
  t_pre = micros();
  int count = 0;
  do{
    IMU.readSensor();
    count++;
    init_GyroZ+=IMU.getGyroZ_rads();
  }while(micros()-t_pre>500000);
  init_GyroZ/=count;
}
double AccelX, AccelY, GyroZ;
double p_Gyro=0;
double dt;
int n = 100;

void loop() {
  // read the sensor
  IMU.readSensor();
  GyroZ = IMU.getGyroZ_rads()-init_GyroZ;
  // display the data
  t_cur = micros();
  dt=((double)(t_cur-t_pre))/1000000;
  t_pre = t_cur;
  p_Gyro+=GyroZ*dt;
  Serial.println(p_Gyro);
}
