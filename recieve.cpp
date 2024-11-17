#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(4, 5);
byte num;
const byte address[6] = { 0x31, 0x4E, 0x6F, 0x64, 0x65 };
long weight;

void setup(){
    Serial.begin(115200);
    Serial.println("Setup");
    radio.begin();
    radio.setChannel(76);
    
    radio.setPALevel(RF24_PA_LOW);
    radio.setDataRate(RF24_1MBPS);
    radio.openReadingPipe(1, address);
    radio.startListening();
    radio.printDetails();
}

void loop(){
    
    if (radio.available()){
        radio.read(&weight, sizeof(weight));
        //Serial.println(weight);
        //if (num == 117){
            //Serial.println("good");
            //delay(500);
        //}
        //else{
            //Serial.println(num);
        //}
    }
    else{
        Serial.println("not");
    }
}
