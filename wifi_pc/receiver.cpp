#include <WiFi.h>

const char* ssid = "Redmi";
const char* password = "123456079";
bool serialReady = false;
WiFiServer server(1234);

void setup() {
  Serial.begin(921600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  server.begin();
}

void loop() {
  if (!serialReady && Serial.available()) {
    String input = Serial.readStringUntil('\n');
    if (input == "get") {
      Serial.write("ready");
      serialReady = true;
    }
  }

  if (serialReady) {
    WiFiClient client = server.available();
    if (client) {
      while (client.connected()) {
        while (client.available()) {
          uint8_t buf[1000];
          int len = client.read(buf, sizeof(buf));
          Serial.write(buf, len);
        }
      }
      client.stop();
    }
  }
}
