#include <ESP8266WiFi.h>

// Wi-Fi Settings
const char* ssid = "rBazan POCO X3 Pro"; // your wireless network name (SSID)
const char* password = "Aa123456"; // your Wi-Fi network password

WiFiClient client;
// ThingSpeak Settings
const int channelID = 000000;
String writeAPIKey = "V6S2J4FG276MBTYJ"; // write API key for your ThingSpeak Channel
const char* server = "api.thingspeak.com";
const int postingInterval = 600 * 1000; // post data every x seconds

void setup() {
  
  WiFi.begin(ssid, password);


  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {  
  if (client.connect(server, 80)) {


    
    // Measure Analog Input (A0)
    float h = 10.5;
    float t = 7.7;
    

    // Construct API request body
    String body = "&field1=";
           body += String(t);
           body += "&field2=";
           body += String(h);
           



    client.println("POST /update HTTP/1.1");
    client.println("Host: api.thingspeak.com");
    client.println("User-Agent: ESP8266 (nothans)/1.0");
    client.println("Connection: close");
    client.println("X-THINGSPEAKAPIKEY: " + writeAPIKey);
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("Content-Length: " + String(body.length()));
    client.println("");
    client.print(body);
  }
  client.stop();


  // wait and then post again
  delay(postingInterval);

}
