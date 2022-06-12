#ifndef CAR_INIT
#define CAR_INIT
//RFID setup
#include <SPI.h>
#include <MFRC522.h>
#define RFID_RST_PIN 5
#define RFID_SS_PIN 53
MFRC522 *mfrc522;

//TB setup
uint32_t TB_PWMA = 2;
uint32_t TB_PWMB = 3;
uint32_t TB_AIN2 = 14;
uint32_t TB_AIN1 = 15;
uint32_t TB_BIN1 = 16;
uint32_t TB_BIN2 = 17;

//MPU9250 setup
#include "MPU9250.h"
#include <math.h>
MPU9250 IMU(Wire,0x68);
int status;
int calibrated;
double init_GyroZ=0, GyroZ, p_Gyro = 0;
unsigned long t_pre = 0, t_cur = 0;
double dt;

//BT setup
#include <SoftwareSerial.h>
SoftwareSerial BT (63, 65);
uint32_t BT_signal = 46;

//Ultrasonic setup
#include <hcsr04.h>
HCSR04 US_l2(22, 31, 20, 4000);
HCSR04 US_l1(23, 32, 20, 4000);
HCSR04 US_c(24, 33, 20, 4000);
HCSR04 US_r1(39, 43, 20, 4000);
HCSR04 US_r2(19, 35, 20, 4000);

//function constructing
bool UID_detect(){
  if(!mfrc522->PICC_IsNewCardPresent()) return false;
  if(!mfrc522->PICC_ReadCardSerial()) return false;
  for (byte i = 0; i < mfrc522->uid.size; i++) {
    BT.print(mfrc522->uid.uidByte[i] < 0x10 ? "0" : "");
    BT.print(mfrc522->uid.uidByte[i], HEX);
  }
  BT.print("\n");
  mfrc522->PICC_HaltA();
  mfrc522->PCD_StopCrypto1();
  return true;
}

void MotorWriting(double vL, double vR){
  vL *= 0.865;
  if(vL > 0){
    digitalWrite(TB_AIN1, HIGH);
    digitalWrite(TB_AIN2, LOW);
  }
  else if (vL < 0){
    digitalWrite(TB_AIN1, LOW);
    digitalWrite(TB_AIN2, HIGH);
    vL *= -1;
  }
  if(vR > 0){
    digitalWrite(TB_BIN1, LOW);
    digitalWrite(TB_BIN2, HIGH);
  }
  else if (vR < 0){
    digitalWrite(TB_BIN1, HIGH);
    digitalWrite(TB_BIN2, LOW);
    vR *= -1;
  }
  if(vL > 255) vL = 255;
  if(vR > 255) vR = 255;
  analogWrite(TB_PWMA, vL);
  analogWrite(TB_PWMB, vR);
}

void MotorWriting1(double coeff){
  if(coeff > 0){
    MotorWriting(150, 150-coeff);
  }
  else{
    MotorWriting(150+coeff, 150);
  }
}

void SmoothSpeedUP(double &coeff){
  
}

// Fundamental Move function
void Front(double &coeff, double &p_Gyro, double state_Gyro){
  IMU.readSensor();
  GyroZ = IMU.getGyroZ_rads()-init_GyroZ;
  // display the data
  t_cur = micros();
  dt=((double)(t_cur-t_pre))/1000000;
  t_pre = t_cur;
  p_Gyro += GyroZ*dt;
  coeff -= GyroZ;
  MotorWriting1(coeff - 100*(p_Gyro-state_Gyro));
}

void Left(double &p_Gyro){
  IMU.readSensor();
  GyroZ = IMU.getGyroZ_rads()-init_GyroZ;
  t_cur = micros();
  dt=((double)(t_cur-t_pre))/1000000;
  t_pre = t_cur;
  p_Gyro+=GyroZ*dt;
  MotorWriting(-75, 0);
}

void Right(double &p_Gyro){
  IMU.readSensor();
  GyroZ = IMU.getGyroZ_rads()-init_GyroZ;
  t_cur = micros();
  dt=((double)(t_cur-t_pre))/1000000;
  t_pre = t_cur;
  p_Gyro+=GyroZ*dt;
  MotorWriting(75, 0);
}

//Advanced move function
void FrontTime(double &coeff, double &p_Gyro, double ms){
  double t_start = micros();
  double state_Gyro = p_Gyro;
  while(t_cur-t_start < 1000*ms){
    Front(coeff, p_Gyro, state_Gyro);
  }
  MotorWriting(0, 0);
  delay(500);
}

void FrontCmd(double &coeff, double &p_Gyro){
  BT.write("f");
  while(!(BT.available() && BT.read() == 'f')){
    delay(10);
  }
  digitalWrite(BT_signal, LOW);
  double state_Gyro = p_Gyro;
  while(!(BT.available() && BT.read() == 't')){
    Front(coeff, p_Gyro, state_Gyro);
  }
  digitalWrite(BT_signal, HIGH);
  MotorWriting(0, 0);
  delay(500);
}

void LeftGyro(double &p_Gyro, double Gyro){
  double state_Gyro = p_Gyro;
  while((p_Gyro-state_Gyro) > Gyro){
    Left(p_Gyro);
  }
  MotorWriting(0, 0);
  delay(500);
}

void RightGyro(double &p_Gyro, double Gyro){
  double state_Gyro = p_Gyro;
  while((p_Gyro-state_Gyro) < Gyro){
    Right(p_Gyro);
  }
  MotorWriting(0, 0);
  delay(500);
}

void LeftU(double &coeff, double &p_Gyro){
  LeftGyro(p_Gyro, -3.06/2);
  FrontTime(coeff, p_Gyro, 1000);
  LeftGyro(p_Gyro, -3.06/2);
}

void RightU(double &p_Gyro){
  RightGyro(p_Gyro, 3.09);
}

#endif
