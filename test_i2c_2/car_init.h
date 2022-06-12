#ifndef CAR_INIT
#define CAR_INIT
//RFID setup
#include <SPI.h>
#include <MFRC522.h>
#define RFID_RST_PIN 5
#define RFID_SS_PIN 53
MFRC522 *mfrc522;

//BT setup
#include <SoftwareSerial.h>
SoftwareSerial BT (13, 12);

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
#endif
