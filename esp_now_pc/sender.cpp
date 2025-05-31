#include <Arduino.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#elif ESP32
#include <WiFi.h>
#endif
#include "ESPNowW.h"
#include <esp_wifi.h>

#define CHUNK_SIZE 250
#define TOTAL_BYTES 1000000
enum State { WAIT_FOR_OK, SEND_BLOCK, DONE };
State state = WAIT_FOR_OK;
unsigned long startTime = 0;
unsigned long endTime = 0;
uint32_t currentIndex = 0;
uint16_t blockID = 0;
bool ackReceived = false;
uint8_t receiverMac[6] = {0xcc, 0x7b, 0x5c, 0x35, 0xc2, 0xb0};
uint8_t dataBlock[CHUNK_SIZE];

void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  if (len >= 2 && incomingData[0] == 'O' && incomingData[1] == 'K') {
    ackReceived = true;
  }
}

// получить MAC-адрес
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

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK) {
    while (true);
  }
  readMacAddress();
  esp_now_register_recv_cb(onDataRecv);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMac, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Ошибка добавления peer!");
  }

  delay(1000);
  esp_now_send(receiverMac, (uint8_t *)"READY?", 6);
}

void loop() {
  static unsigned long lastAttempt = 0;

  if (state == WAIT_FOR_OK && ackReceived) {
    ackReceived = false;
    state = SEND_BLOCK;
    startTime = millis();
  }

  if (state == SEND_BLOCK && currentIndex < TOTAL_BYTES) {
    for (int i = 0; i < CHUNK_SIZE; i++) {
      dataBlock[i] = 0;
    }

    esp_now_send(receiverMac, dataBlock, CHUNK_SIZE);
    ackReceived = false;
    lastAttempt = millis();

    while (!ackReceived && millis() - lastAttempt < 1000) {
      delay(1);
    }

    if (ackReceived) {
      currentIndex += CHUNK_SIZE;
      blockID++;
      ackReceived = false;
    }
  }

  if (currentIndex >= TOTAL_BYTES && state != DONE) {
    state = DONE;
    endTime = millis();  // завершение секундомера
    Serial.print("Передача завершена за ");
    Serial.print((endTime - startTime) / 1000.0);
    Serial.println(" секунд");
  }
  Serial.println(state);
} //com4 white
