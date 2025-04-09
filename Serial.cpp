#include <Arduino.h>
/*-----------second case random pixels
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
}*/
/*
#define WIDTH 1000
#define HEIGHT 1000
#define CHUNK_SIZE 1024

void setup() {
  Serial.begin(921600);
  delay(3000);

  Serial.println("START");

  for (int y = 0; y < HEIGHT; y++) {
    byte row[WIDTH];

    for (int x = 0; x < WIDTH; x++) {
      row[x] = map(x, 0, WIDTH - 1, 0, 255);  // от 0 (чёрный) до 255 (белый)
    }

    // Отправляем строку по кускам
    for (int i = 0; i < WIDTH; i += CHUNK_SIZE) {
      int len = min(CHUNK_SIZE, WIDTH - i);
      Serial.write(&row[i], len);
    }
  }

  Serial.println("END");
}*/
//third case fractal
#define WIDTH 1000
#define HEIGHT 1000
#define CHUNK_SIZE 1024
#define MAX_ITER 64  // Количество итераций для точности / контраста

void setup() {
  Serial.begin(921600);
  delay(3000);

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
}

void loop() {
  // ничего не делаем
}
