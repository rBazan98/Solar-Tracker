#include <Wire.h>  
#include "SSD1306.h"

int t = 0;

SSD1306  display(0x3C, D2, D1); //Address set here 0x3C that I found in the scanner, and pins defined as D2 (SDA/Serial Data), and D5 (SCK/Serial Clock).

void setup()   {                
  display.init();
  display.setColor(WHITE);
  display.setFont(ArialMT_Plain_16);
  display.flipScreenVertically();
  display.drawString(0, 0, String(t));
  display.display();
}

void loop() {
t = t + 1;
  display.clear();
  display.drawString(0, 0, String(t));
    display.display();
   
}
