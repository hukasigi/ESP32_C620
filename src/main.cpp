#include <Arduino.h>

#include <CAN.h>

void onReceive(int packetSize);

void setup() {
    Serial.begin(115200);

    CAN.setPins(4, 5); // RX, TX

    if (!CAN.begin(1000000)) {
        Serial.println("CAN init failed");
        while (1)
            ;
    }

    CAN.onReceive(onReceive);

    volatile uint32_t* pREG_IER = (volatile uint32_t*)0x3ff6b010;
    *pREG_IER &= ~(uint8_t)0x10;
}

uint16_t angle;
int16_t  speed;
int16_t  current;
uint8_t  temp;

void loop() {
    static int16_t motor_current = 5000; // 最初は弱め

    CAN.beginPacket(0x200);

    for (int i = 0; i < 4; i++) {
        CAN.write(motor_current >> 8);
        CAN.write(motor_current & 0xFF);
    }

    CAN.endPacket();

    Serial.print("angle: ");
    Serial.print(angle);
    Serial.print(" speed: ");
    Serial.print(speed);
    Serial.print(" current: ");
    Serial.print(current);
    Serial.print(" temp: ");
    Serial.println(temp);

    delay(10);
}

void onReceive(int packetSize) {
    int id = CAN.packetId();

    if (id == 0x204) {
        uint8_t data[8];

        for (int i = 0; i < 8; i++) {
            data[i] = CAN.read();
        }

        angle   = (data[0] << 8) | data[1];
        speed   = (data[2] << 8) | data[3];
        current = (data[4] << 8) | data[5];
        temp    = data[6];
    }
}