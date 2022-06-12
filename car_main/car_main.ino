#include "car_init.h"
#include <hcsr04.h>
#include "MPU9250.h"
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //RFID setup
  SPI.begin();
  mfrc522 = new MFRC522(RFID_SS_PIN, RFID_RST_PIN);
  mfrc522->PCD_Init();

  //TB setup
  pinMode(TB_PWMA, OUTPUT);
  pinMode(TB_PWMB, OUTPUT);
  pinMode(TB_AIN1, OUTPUT);
  pinMode(TB_AIN2, OUTPUT);
  pinMode(TB_BIN1, OUTPUT);
  pinMode(TB_BIN1, OUTPUT);

  //BT setup
  BT.begin(9600);

  //MPU 9250 setup
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
  IMU.setSrd(1000 / Hz - 1);
  // re:0
  delay(1000);
  t_pre = micros();
  int count = 0;
  do {
    IMU.readSensor();
    count++;
    init_GyroZ += IMU.getGyroZ_rads();
  } while (micros() - t_pre > 500000);
  init_GyroZ /= count;
  BT.write("s");
  while(!(BT.available() && BT.read() == 'a')) delay(10);
  digitalWrite(BT_signal, HIGH);
}

double coeff = 10;
int n = 0;
double t_pre2;
double state_Gyro;

void loop() {
  FrontCmd(coeff, p_Gyro);
  RightU(p_Gyro);
  FrontCmd(coeff, p_Gyro);
  LeftU(coeff, p_Gyro);
}
