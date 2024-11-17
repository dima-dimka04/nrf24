#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(4, 5);

const byte address[6] = { 0x31, 0x4E, 0x6F, 0x64, 0x65 };
byte num = 117;
long weight = 13301;

void setup(){
    radio.begin();
    Serial.begin(115200);
    radio.setChannel(76);
    radio.setPALevel(RF24_PA_LOW);
    radio.setDataRate(RF24_1MBPS);
    radio.openWritingPipe(address);
    radio.stopListening();
    radio.printDetails();
}

void loop(){
    if(radio.write(&weight, sizeof(weight)))
    {
        Serial.println("Sent successfully");
    }
    else {
        Serial.println("Send failed");
    }
    delay(1000);
}
