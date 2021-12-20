#include <Wire.h>
#include <Adafruit_INA219.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>

#define OLED_RESET 4

//Inicializando dispositivos I2C
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
float energy_A = 0;

//Valores para el sensor B
float shuntvoltage_B = 0;
float busvoltage_B = 0;
float current_mA_B = 0;
float loadvoltage_B = 0;
float energy_B = 0;

void setup() {
  // Iniciando comunicación I2C con los tres dispositivos
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  ina_A.begin();
  ina_B.begin();

  //Iniciando comunicación serial
  Serial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    
    ina219values();
    displaydata();

//    Serial.print("================");
//    Serial.print("\n");

//    Serial.print(shuntvoltage);
//    Serial.print("\n");
//
//    Serial.print(busvoltage);
//    Serial.print("\n");
//
//    Serial.print(current_mA);
//    Serial.print("\n");
//
//    Serial.print(loadvoltage);
//    Serial.print("\n");
//
//    Serial.print(energy);
//    Serial.print("\n");
//
//    Serial.print("================");
//    Serial.print("\n");

//
//  shuntvoltage = ina219.getShuntVoltage_mV();
//  busvoltage = ina219.getBusVoltage_V();
//  current_mA = ina219.getCurrent_mA();
//  loadvoltage = busvoltage + (shuntvoltage / 1000);
//  energy = energy + loadvoltage * current_mA / 3600;

  }
}

void displaydata() {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(loadvoltage_A);
  display.setCursor(35, 0);
  display.println("V");
  display.setCursor(50, 0);
  display.println(current_mA_A);
  display.setCursor(95, 0);
  display.println("mA");
  display.setCursor(0, 10);
  display.println(loadvoltage_A * current_mA_A);
  display.setCursor(65, 10);
  display.println("mW");
  display.setCursor(0, 20);
  display.println(energy_A);
  display.setCursor(65, 20);
  display.println("mWh");
  display.display();
}

void ina219values() {
  //Valores para sensor A
  shuntvoltage_A = ina_A.getShuntVoltage_mV();
  busvoltage_A = ina_A.getBusVoltage_V();
  current_mA_A = ina_A.getCurrent_mA();
  loadvoltage_A = busvoltage_A + (shuntvoltage_A / 1000);
  energy_A = energy_A + loadvoltage_A * current_mA_A / 3600;

  //Valores para sensor B
  shuntvoltage_B = ina_B.getShuntVoltage_mV();
  busvoltage_B = ina_B.getBusVoltage_V();
  current_mA_B = ina_B.getCurrent_mA();
  loadvoltage_B = busvoltage_B + (shuntvoltage_B / 1000);
  energy_B = energy_B + loadvoltage_B * current_mA_B / 3600;
  
}
