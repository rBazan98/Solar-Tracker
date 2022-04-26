//Código para ESP8266
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include "SSD1306Wire.h"

//Librerias blink
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

//#define OLED_RESET 4

//Inicializando los dispositivos I2C como instancias de sus respectivas librerias
SSD1306Wire display(0x3C, SDA, SCL); //Display OLED
Adafruit_INA219 ina_A(0x41);// Sensor A
Adafruit_INA219 ina_B(0x44);// Sensor B

//Variables para el Loop
unsigned long previousMillis = 0;
unsigned long previousSent = 0;
unsigned long interval = 100; //Periodo de refresco
unsigned long tspk_interval = 900 * 1000; 
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

// Wi-Fi Settings
const char* ssid = "rBazan POCO X3 Pro"; // your wireless network name (SSID)
const char* password = "Aa123456"; // your Wi-Fi network password

WiFiClient client;
// ThingSpeak Settings
const int channelID = 000000;
String writeAPIKey = "V6S2J4FG276MBTYJ"; // write API key for your ThingSpeak Channel
const char* server = "api.thingspeak.com";
//const int postingInterval = 600 * 1000; // post data every x seconds

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
  
  //Iniciando comunicación serial
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  
}

void loop() {
//configurando el tiempo de refresco
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;    


//codigo a ejecutar //

//  solar_track();  //moviendo hacia el sol
    
    ina219values(); //leyendo consumo
    
    displaydata();  //imprimiendo datos en el display oled

    if (currentMillis - previousSent >= tspk_interval) {
    previousSent = currentMillis;    

//    send_data();
    send_data_2();
    
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

void send_data_2(){

  char auth[] = "wwtWbdKhe0a8HEfe9SdpC_9Wtwc-9eaK";
  char ssid[] = "HUAWEI Mate 20 lite";
  char pass[] = "952898fca99e";

  Blynk.run();
  Blynk.virtualWrite(V1,power_A);
  Blynk.virtualWrite(V2,power_B);
  
}


void send_data(){
    WiFi.begin(ssid, password);
    
  while (WiFi.status() != WL_CONNECTED) {
    display_mensaje("Conectando...");
    Serial.println("Conectando");
    delay(500);
  }
    Serial.println("Conectado!");

  if (client.connect(server, 80)) {

    // Measure Analog Input (A0)
    float h = 10.5;
    float t = 7.7;
    
    // Construct API request body
    String body = "&field1=";
           body += String(power_A);
           body += "&field2=";
           body += String(power_B);
           body += "&field3=";
           body += String(delta);
           
    client.println("POST /update HTTP/1.1");
    client.println("Host: api.thingspeak.com");
    client.println("User-Agent: ESP8266 (nothans)/1.0");
    client.println("Connection: close");
    client.println("X-THINGSPEAKAPIKEY: " + writeAPIKey);
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("Content-Length: " + String(body.length()));
    client.println("");
    client.print(body);
    Serial.println("Datos enviados :)");
  }
  client.stop();

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
