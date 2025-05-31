#include <Arduino.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#elif ESP32
#include <WiFi.h>
#endif
#include "ESPNowW.h"
#include <esp_wifi.h>
#define TOTAL_BYTES 1000000
#define CHUNK_SIZE 250
uint8_t senderMac[6] = {0xcc, 0x7b, 0x5c, 0x35, 0xba, 0xcc};
bool serialReady = false;

void readMacAddress(){
  uint8_t baseMac[6];
  esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
  if (ret == ESP_OK) {
    Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
                  baseMac[0], baseMac[1], baseMac[2],
                  baseMac[3], baseMac[4], baseMac[5]);
  } else {
    Serial.println("Failed to read MAC address");
  }
}

void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  static bool readyReceived = false;
  if (!readyReceived && len == 6 && memcmp(incomingData, "READY?", 6) == 0) {
    memcpy(senderMac, mac, 6);

    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, senderMac, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    esp_now_add_peer(&peerInfo);

    esp_now_send(senderMac, (uint8_t *)"OK", 2);
    readyReceived = true;
    return;
  }

  if (serialReady) {
    Serial.write(incomingData, len);
  }

  esp_now_send(mac, (uint8_t *)"OK", 2);
}

void setup() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK) {
    while (true);
  }

  esp_now_register_recv_cb(onDataRecv);

  Serial.begin(115200);
}

void loop() {
  if (!serialReady && Serial.available()) {
    String input = Serial.readStringUntil('\n');
    if (input == "get") {
      Serial.write("ready");
      serialReady = true;
    }
  }
}

// com3 black
