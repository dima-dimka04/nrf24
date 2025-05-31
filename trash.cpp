void loop() {
  static uint32_t expectedPacketID = 0;
  static uint32_t crcErrors = 0;
  static uint32_t packetLoss = 0;
  static uint32_t totalPackets = 0;

  if (radio.available()) {
    while (radio.available()) {
      int length = radio.getPayloadSize();
      Serial.print("Принято ");
      Serial.print(length);
      Serial.print(" байт: ");

      if (length != sizeof(receivedPacket)) {
        Serial.println("НЕВЕРНЫЙ размер пакета!");
        return;
      }

      radio.read(&receivedPacket, sizeof(receivedPacket));
      totalPackets++;

      // Проверка CRC
      if (checksum((unsigned char *)&receivedPacket, &receivedPacket.crc) != receivedPacket.crc) {
        Serial.println("CRC ОШИБКА!");
        crcErrors++;
        return;
      }

      // Проверка правильной последовательности
      if (receivedPacket.packetID != expectedPacketID) {
        Serial.print("Ошибка последовательности! Ожидал: ");
        Serial.print(expectedPacketID);
        Serial.print(", получил: ");
        Serial.println(receivedPacket.packetID);
        packetLoss++;
      }

      expectedPacketID = receivedPacket.packetID + 1;

      // Вывод данных каждого пакета
      Serial.print("Пакет ID: ");
      Serial.print(receivedPacket.packetID);
      Serial.print(" Данные: ");
      for (int i = 0; i < PACKET_DATA_SIZE; i++) {
        Serial.print(receivedPacket.data[i]);
        Serial.print(" ");
      }
      Serial.println(" [CRC OK]");
    }
  }

  // Периодический вывод статистики
  static uint32_t lastMillis = 0;
  if (millis() - lastMillis > 5000) {
    lastMillis = millis();
    Serial.println("\n--- Статистика ---");
    Serial.print("Принято пакетов: ");
    Serial.println(totalPackets);
    Serial.print("Ошибок CRC: ");
    Serial.println(crcErrors);
    Serial.print("Ошибок последовательности (потеря пакетов): ");
    Serial.println(packetLoss);
    Serial.println("------------------\n");
  }
}

/*
#include <Arduino.h>*/
/*
#define TOTAL_BYTES 1000000
#define CHUNK_SIZE 1024

void setup() {
  Serial.begin(115200);  // высокая скорость передачи
  delay(3000);           // пауза для открытия порта на ПК

  Serial.println("START");  // заголовок для синхронизации на ПК
  randomSeed(analogRead(0));

  for (int i = 0; i < TOTAL_BYTES; i += CHUNK_SIZE) {
    byte chunk[CHUNK_SIZE];

    // Генерация данных: градиент
    for (int j = 0; j < CHUNK_SIZE; j++) {
      chunk[j] = random(0, 256);
    }

    Serial.write(chunk, CHUNK_SIZE);  // отправка порции
    delay(1);  // пауза (при необходимости)
  }

  Serial.println("END");  // конец передачи
}снизу шахматная доска*/ 
/*
#define WIDTH 1000
#define HEIGHT 1000
#define CHUNK_SIZE 64

void waitForGetCommand() {
  while (true) {
    if (Serial.available()) {
      String cmd = Serial.readStringUntil('\n');
      if (cmd == "get") break;
    }
  }
}

void setup() {
  Serial.begin(115200);
  for (int y = 0; y < HEIGHT; y++) {
    byte row[WIDTH];

    // Инвертируем Y, чтобы нижняя строка массива была внизу изображения
    int flippedY = HEIGHT - 1 - y;
    int cellY = flippedY / 125;

    for (int x = 0; x < WIDTH; x++) {
      int cellX = x / 125;

      // Левая нижняя клетка должна быть чёрной
      if ((cellX + cellY) % 2 == 0) {
        row[x] = 0;    // чёрная
      } else {
        row[x] = 255;  // белая
      }
    }

    for (int i = 0; i < WIDTH; i += CHUNK_SIZE) {
      int len = min(CHUNK_SIZE, WIDTH - i);
      Serial.write(&row[i], len);
    }
  }

  Serial.println("END");
}*/

// градиент
/*
void setup() {
  Serial.begin(921600);
  delay(3000);
  waitForGetCommand();
  Serial.println("START");

  for (int y = 0; y < HEIGHT; y++) {
    byte row[WIDTH];

    for (int x = 0; x < WIDTH; x++) {
      row[x] = row[x] = (byte)(((float)x / (WIDTH - 1)) * 255); //map(x, 0, WIDTH - 1, 0, 255);  // от 0 (чёрный) до 255 (белый)
    }

    // Отправляем строку по кускам
    for (int i = 0; i < WIDTH; i += CHUNK_SIZE) {
      int len = min(CHUNK_SIZE, WIDTH - i);
      Serial.write(&row[i], len);
    }
  }

  Serial.println("END");
}*/
/*
#define WIDTH 1000
#define HEIGHT 1000
#define CHUNK_SIZE 1024
#define MAX_ITER 64  // Количество итераций для точности / контраста

void setup() {
  Serial.begin(921600);
  delay(5000);

  Serial.println("START");

  for (int py = 0; py < HEIGHT; py++) {
    byte row[WIDTH];

    for (int px = 0; px < WIDTH; px++) {
      // Преобразуем координаты пикселя в комплексную плоскость
      float x0 = map(px, 0, WIDTH - 1, -2.5f * 1000, 1.0f * 1000) / 1000.0f;
      float y0 = map(py, 0, HEIGHT - 1, -1.0f * 1000, 1.0f * 1000) / 1000.0f;

      float x = 0.0f;
      float y = 0.0f;
      int iteration = 0;

      while (x*x + y*y <= 4.0f && iteration < MAX_ITER) {
        float xtemp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtemp;
        iteration++;
      }

      // Чем больше итераций — тем темнее точка
      byte value = map(iteration, 0, MAX_ITER, 255, 0);
      row[px] = value;
    }

    // Отправляем строку по частям
    for (int i = 0; i < WIDTH; i += CHUNK_SIZE) {
      int len = min(CHUNK_SIZE, WIDTH - i);
      Serial.write(&row[i], len);
    }
  }

  Serial.println("END");
}*/
/*
void loop() {
  // ничего не делаем
}*/
