#include <servo.h>
Servo servo1;

void setup() {
servo1.attach(A0);

}

void loop() {
servo1.write(0);
}
