//Código para ESP8266
#include <Wire.h>
#include <Adafruit_INA219.h>
#include "SSD1306Wire.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

//Constantes para Blynk
#define BLYNK_TEMPLATE_ID           "TMPLv4XzcfX4"
#define BLYNK_DEVICE_NAME           "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "yid8DIZTWnryOk78yRa2NCoi7KwujYjV"

//Variables de red
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "vodafoneC6F8-RPT";
char pass[] = "karen2020";
BlynkTimer timer;

//obteniendo estado de interruptor V0
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();  // ------>> reescribir en una variable global
  // Update state (Panel en Blynk)
  Blynk.virtualWrite(V1, value);
}

//Accion al realizar la conexion
BLYNK_CONNECTED()
{
}

//envia el tiempo activo (opcional)
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
}



//Inicializando los dispositivos I2C como instancias de sus respectivas librerias
SSD1306Wire display(0x3C, SDA, SCL); //Display OLED
Adafruit_INA219 ina_A(0x41);// Sensor A
Adafruit_INA219 ina_B(0x44);// Sensor B

//Variables para el Loop
unsigned long previousMillis = 0;
unsigned long previousSent = 0;
unsigned long interval = 100; //Periodo de refresco
unsigned long data_interval = 900 * 1000; 
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

int yb = 0;
int y_1 = 0;
int y2 = 18 + yb;
int y3 = (y2-yb) * 2 + yb;
int y4 = (y2-yb) * 3 + yb;

// // Wi-Fi Settings
// const char* ssid = "rBazan POCO X3 Pro"; // your wireless network name (SSID)
// const char* password = "Aa123456"; // your Wi-Fi network password

// WiFiClient client;
// // ThingSpeak Settings
// const int channelID = 000000;
// String writeAPIKey = "V6S2J4FG276MBTYJ"; // write API key for your ThingSpeak Channel
// const char* server = "api.thingspeak.com";
// //const int postingInterval = 600 * 1000; // post data every x seconds

////Sensores de luz
//int topleft;
//int topright;
//int downleft;
//int downright;
//int waittime = 1;


void setup() {
xC = xC - 5; //Correccion columna C del display
  
  // Iniciando comunicación I2C con los tres dispositivos
  display.init();
  ina_A.begin();
  ina_B.begin();

  display.flipScreenVertically();
  
  //Iniciando comunicación serial, Blynk y temporizador
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, myTimerEvent);


}

void loop() {
  
  //eventos de Blynk temporizados
  Blynk.run();
  timer.run();

//configurando el tiempo de refresco
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;    
//======================//
//  codigo a ejecutar   //
//======================//

//  solar_track();  //moviendo hacia el sol (en la otra placa)
    
    ina219values(); //leyendo consumo
    
    displaydata();  //imprimiendo datos en el display oled

    //intervalo en que se envian los datos
    if (currentMillis - previousSent >= data_interval) {
    previousSent = currentMillis;    

    // send_data();

    }
  }
}

void ina219values() {

  //Valores para sensor A
  shuntvoltage_A = ina_A.getShuntVoltage_mV();
  busvoltage_A = ina_A.getBusVoltage_V();
  current_mA_A = ina_A.getCurrent_mA();
  loadvoltage_A = busvoltage_A + (shuntvoltage_A / 1000);
  power_A = loadvoltage_A * current_mA_A;
  energy_A = energy_A + loadvoltage_A * current_mA_A / 3600;
  
  //Valores para sensor B
  shuntvoltage_B = ina_B.getShuntVoltage_mV();
  busvoltage_B =  ina_B.getBusVoltage_V();
  current_mA_B = -ina_B.getCurrent_mA();
  loadvoltage_B = busvoltage_B + (shuntvoltage_B / 1000);
  power_B = loadvoltage_B * current_mA_B;
  energy_B = energy_B + loadvoltage_B * current_mA_B / 3600;

  //delta
    delta = 100*(power_A - power_B)/power_B;
}

void send_data(){
  
  }

void display_mensaje(String mensaje) {
  display.clear();
  display.setColor(WHITE);
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, mensaje);
}


void displaydata() {
  display.clear();
  display.setColor(WHITE);
  display.setFont(ArialMT_Plain_10);
  
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
  display.drawHorizontalLine(0, 15, 128);
//Coll A
  display.drawString(xA, y_1,"src A");
  display.drawString(xA, y2,String(power_A));
  display.drawString(xA, y3,String(loadvoltage_A));
  display.drawString(xA, y4,String(current_mA_A));

//Coll B
  display.drawString(xB, y_1,"src B");
  display.drawString(xB, y2,String(power_B));
  display.drawString(xB, y3,String(loadvoltage_B));
  display.drawString(xB, y4,String(current_mA_B));

//Coll C
  display.drawString(xC, y_1,"   /\\%");
  if  (delta<10){display.drawString(xC, y2, "  " + String(delta));}
  else          {display.drawString(xC, y2, String(delta));}
  display.drawString(xC, y3,"    -");
  display.drawString(xC, y4,"    -");

//Coll D
  display.drawString(xD, y_1, "[ ]");
  display.drawString(xD, y2,"mW");
  display.drawString(xD, y3,"V");
  display.drawString(xD, y4, "mA");

  display.display();
}
