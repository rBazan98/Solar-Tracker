#include <Servo.h>
int LED = 2;
Servo servo1;
void setup() {
servo1.attach(A0);
}
void loop() {
//LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); 
     digitalWrite(LED, HIGH);
     delay(1000);
     digitalWrite(LED, LOW);
     
}
