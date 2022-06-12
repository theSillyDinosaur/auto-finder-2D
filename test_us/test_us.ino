#include <hcsr04.h>

HCSR04 US_l2(22, 31, 20, 4000);
HCSR04 US_l1(23, 32, 20, 4000);
HCSR04 US_c(24, 33, 20, 4000);
HCSR04 US_r1(39, 43, 20, 4000);
HCSR04 US_r2(19, 35, 20, 4000);

void setup(){
  Serial.begin(9600);
}

void loop(){
  Serial.println(US_l2.distanceInMillimeters());
}
