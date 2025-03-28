/*----------First-ESP32-board-is-server-------*/
#include "WiFi.h"
#include "ESPAsyncWebServer.h"

// Set your access point network credentials
const char* ssid = "Redmi";
const char* password = "123456079";

typedef struct struct_message {
    char a[32];
    int b;
    float c;
    bool d;
  } struct_message;
struct_message myData;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String readChar() {
  return String(myData.a);
}

String readInt() {
  return String(myData.b);
}

String readbool() {
  return String(myData.d);
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.println();
  
  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/char", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readChar().c_str());
  });
  server.on("/int", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readInt().c_str());
  });
  server.on("/bool", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readbool().c_str());
  });
  // Start server
  server.begin();
}
 
void loop(){
  
}

/*----------Second-ESP32-board-is-client-------*/

#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Redmi";
const char* password = "123456079";

//Your IP address or domain name with URL path
const char* serverNameTemp = "http://192.168.4.1/char";
const char* serverNameHumi = "http://192.168.4.1/int";
const char* serverNamePres = "http://192.168.4.1/bool";

String _char;
String _int;
String _bool;

unsigned long previousMillis = 0;
const long interval = 5000; 

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  unsigned long currentMillis = millis();
  
  if(currentMillis - previousMillis >= interval) {
     // Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED ){ 
      _char = httpGETRequest(serverNameTemp);
      _int = httpGETRequest(serverNameHumi);
      _bool = httpGETRequest(serverNamePres);
      Serial.println("Temperature: " + _char + " *C - Humidity: " + _int + " % - Pressure: " + _bool + " hPa");
      // save the last HTTP GET Request
      previousMillis = currentMillis;
    }
    else {
      Serial.println("WiFi Disconnected");
    }
  }
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "--"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}
