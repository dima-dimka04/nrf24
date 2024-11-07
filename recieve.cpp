#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(12, 14);
byte counter;
byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"};

void setup(){
    Serial.begin(115200);
    radio.begin();
    radio.setAutoAck(1);
    radio.setRetries(0, 10);
    radio.enableAckPayload();
    radio.setPayloadSize(32);

    radio.openReadingPipe(1, address[0]);
    radio.setChannel(0x08);
    radio.setPALevel (RF24_PA_MAX);
    radio.setDataRate(RF24_250KBPS);
    radio.powerUp();
    radio.startListening();
}

void loop(){
    radio.powerUp();
    byte pipeN, gotByte; 
    while (radio.available(&pipeN)){
        radio.read(&gotByte, 1);
        Serial.println("Recieve: "); Serial.println(gotByte);
    }
}