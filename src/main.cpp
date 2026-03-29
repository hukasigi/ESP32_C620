#include <Arduino.h>

#include <CAN.h>

#include <Arduino.h>
#include <CAN.h>

void setup() {
    Serial.begin(115200);

    CAN.setPins(4, 5); // RX, TX

    if (!CAN.begin(1000000)) {
        Serial.println("CAN init failed");
        while (1)
            ;
    }

    volatile uint32_t* pREG_IER = (volatile uint32_t*)0x3ff6b010;
    *pREG_IER &= ~(uint8_t)0x10;
}

void loop() {
    int16_t current = 5000; // 最初は弱め

    CAN.beginPacket(0x200);

    for (int i = 0; i < 4; i++) {
        CAN.write(current >> 8);
        CAN.write(current & 0xFF);
    }

    CAN.endPacket();

    delay(10);
}