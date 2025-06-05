// wifi test sender
#include <WiFi.h>

const char* ssid = "Redmi";
const char* password = "123456079";
const char* server_ip = "192.168.43.74";
const uint16_t server_port = 1234;
WiFiClient client;
#define WIDTH 1000
#define HEIGHT 1000
#define BLOCK_SIZE 1000

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Подключение к Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi подключён");

  if (!client.connect(server_ip, server_port)) {
    Serial.println("Ошибка подключения к серверу.");
    while (true);
  }

  Serial.println("Соединение установлено. Начало передачи...");

  byte row[WIDTH];
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      row[x] = map(x, 0, WIDTH - 1, 0, 255);  // градиент
    }
    client.write(row, BLOCK_SIZE);
  }

  Serial.println("Передача завершена.");
  client.stop();
}

void loop() {}
