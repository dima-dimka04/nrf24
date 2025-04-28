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
