#include <Arduino.h>

#include "Wire.h"

const int MPU6050_ADDR = 0x68;  // Alternatively set AD0 to HIGH  --> Address = 0x69

void initializeIMU(TwoWire wireInterface, int address) {
    wireInterface.beginTransmission(address);
    wireInterface.write(0x6B);  // PWR_MGMT_1 register
    wireInterface.write(0);
    wireInterface.endTransmission(true);
}

void readIMUValues(
    TwoWire wireInterface,
    int address,
    int* accelerometer[3],
    int* gyroscope[3],
    int* temperature
) {
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(0x3B);             // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);  // the parameter indicates that the Arduino will send a restart.
                                  // As a result, the connection is kept active.
    Wire.requestFrom(MPU6050_ADDR, 14, true);  // request a total of 7*2=14 registers

    // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same int16_t
    // variable
    for (int i = 0; i < 3; i++) {
        *accelerometer[i] = Wire.read() << 8 | Wire.read();
    }

    *temperature = Wire.read() << 8 | Wire.read();

    for (int i = 0; i < 3; i++) {
        *gyroscope[i] = Wire.read() << 8 | Wire.read();
    }
}

void setup() {
    Serial.begin(115200);
    
    Wire.begin();
    initializeIMU(Wire, MPU6050_ADDR);
}

void loop() {
    
}