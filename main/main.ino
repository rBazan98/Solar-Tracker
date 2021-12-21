#include <Wire.h>
#include <Adafruit_INA219.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>

#define OLED_RESET 4

//Inicializando los dispositivos I2C
Adafruit_SSD1306 display(OLED_RESET); //Display OLED
Adafruit_INA219 ina_A(0x41);// Sensor A
Adafruit_INA219 ina_B(0x44);// Sensor B

//Variables para el Loop
unsigned long previousMillis = 0;
unsigned long interval = 100; //Periodo de refresco
const int chipSelect = 10;

//Valores para el sensor A
float shuntvoltage_A = 0;
float busvoltage_A = 0;
float current_mA_A = 0;
float loadvoltage_A = 0;
float power_A = 0;
float energy_A = 0;

//Valores para el sensor B
float shuntvoltage_B = 0;
float busvoltage_B = 0;
float current_mA_B = 0;
float loadvoltage_B = 0;
float power_B = 0;
float energy_B = 0;
  float delta = 0;

//Columnas
int xA = 0;
int xB = 34;
int xC = xB * 2;
int xD = xB * 3;

int y1 = 0;
int y2 = 8;
int y3 = y2 * 2;
int y4 = y2 * 3;

//Sensores de luz
int topleft;
int topright;
int downleft;
int downright;
int waittime = 1;


void setup() {

//servos
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

//PWM avanzado
  TCCR1A = 0;
  TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
  TCCR1B = 0;
  TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);
  ICR1 = 40000;
  OCR1A = 3000;
  OCR1B = 3600;
  
  // Iniciando comunicación I2C con los tres dispositivos
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  ina_A.begin();
  ina_B.begin();

  //Iniciando comunicación serial
  Serial.begin(9600);
}

void loop() {
//configurando el tiempo de refresco
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;    

//codigo a ejecutar //

    solar_track();  //moviendo hacia el sol
    
    ina219values(); //leyendo consumo
    
    displaydata();  //imprimiendo datos en el display oled


  }
}

void displaydata() {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
                                /*
Display map:
 
y\x ||xA||xB||xC||xD||
    __________________
y1  | A1  B1  C1  D1 |
y2  | A2  B2  C2  D2 |
y3  | A3  B3  C3  D3 |
y4  | A4  B4  C4  D4 |
    TTTTTTTTTTTTTTTTTT

                                */
//Coll A
  display.setCursor(xA, y2);
  display.println(power_A);

  display.setCursor(xA, y3);
  display.println(loadvoltage_A);

  display.setCursor(xA, y4);
  display.println(current_mA_A);

  display.setCursor(xA, y1);
  display.println("src A");

//Coll B
  display.setCursor(xB, y2);
  display.println(power_B);

  display.setCursor(xB, y3);
  display.println(loadvoltage_B);

  display.setCursor(xB, y4);
  display.println(current_mA_B);

  display.setCursor(xB, y1);
  display.println("src B");

//Coll C
  display.setCursor(xC, y2);
  display.println(delta);

  display.setCursor(xC, y3);
  display.println(" -");

  display.setCursor(xC, y4);
  display.println(" -");

  display.setCursor(xC, y1);
  display.println(" /\\%");

//Coll D
  display.setCursor(xD, y2);
  display.println("mW");

  display.setCursor(xD, y3);
  display.println("V");

  display.setCursor(xD, y4);
  display.println("mA");

  display.setCursor(xD, y1);
  display.println("[]");

  display.display();

}

void ina219values() {
  //Valores para sensor A
  shuntvoltage_A = ina_A.getShuntVoltage_mV();
  busvoltage_A = ina_A.getBusVoltage_V();
  current_mA_A = 0.9+ina_A.getCurrent_mA();
  loadvoltage_A = busvoltage_A + (shuntvoltage_A / 1000);
  power_A = loadvoltage_A * current_mA_A;
  energy_A = energy_A + loadvoltage_A * current_mA_A / 3600;
  
  //Valores para sensor B
  shuntvoltage_B = ina_B.getShuntVoltage_mV();
  busvoltage_B = ina_B.getBusVoltage_V();
  current_mA_B = -0.3 - ina_B.getCurrent_mA();
  loadvoltage_B = busvoltage_B + (shuntvoltage_B / 1000);
  power_B = loadvoltage_B * current_mA_B;
  energy_B = energy_B + loadvoltage_B * current_mA_B / 3600;

  //delta
    delta = (power_A - power_B)/power_B;

}

void solar_track() {
  //leyendo valores
  topleft = analogRead(A0);
  topright = analogRead(A1);
  downleft = analogRead(A2);
  downright = analogRead(A3);

  if (topleft > topright) {
    OCR1A = OCR1A + 1;
    delay(waittime);
  }
  if (downleft > downright) {
    OCR1A = OCR1A + 1;
    delay(waittime);
  }
  if (topleft < topright) {
    OCR1A = OCR1A - 1;
    delay(waittime);
  }
  if (downleft < downright) {
    OCR1A = OCR1A - 1;
    delay(waittime);
  }
  if (OCR1A > 4000) {
    OCR1A = 4000;
  }
  if (OCR1A < 2000) {
    OCR1A = 2000;
  }
  if (topleft > downleft) {
    OCR1B = OCR1B - 1;
    delay(waittime);
  }
  if (topright > downright) {
    OCR1B = OCR1B - 1;
    delay(waittime);
  }
  if (topleft < downleft) {
    OCR1B = OCR1B + 1;
    delay(waittime);
  }
  if (topright < downright) {
    OCR1B = OCR1B + 1;
    delay(waittime);
  }
  if (OCR1B > 4200) {
    OCR1B = 4200;
  }
  if (OCR1B < 3000) {
    OCR1B = 3000;
  }
}
