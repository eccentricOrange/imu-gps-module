#include <Arduino.h>

#include "Wire.h"

const int IMU_ADDRESS = 0x68;  // Alternatively set AD0 to HIGH  --> Address = 0x69

int accelerometerReadings[3];
int accelerometerMaximums[3];
int accelerometerMinimums[3];

int gyroscopeReadings[3];

int temperatureReading;

void initializeIMU(TwoWire* wireInterface, int address) {
    (*wireInterface).beginTransmission(address);
    (*wireInterface).write(0x6B);  // PWR_MGMT_1 register
    (*wireInterface).write(0);     // wake up!
    (*wireInterface).endTransmission(true);
}

void readIMUValues(
    TwoWire* wireInterface,
    int address,
    int accelerometer[3],
    int gyroscope[3],
    int* temperature
) {
    (*wireInterface).beginTransmission(IMU_ADDRESS);
    (*wireInterface).write(0x3B);             // starting with register 0x3B (ACCEL_XOUT_H)
    (*wireInterface).endTransmission(false);  // the parameter indicates that the Arduino will send a restart.
                                  // As a result, the connection is kept active.
    (*wireInterface).requestFrom(IMU_ADDRESS, 14, true);  // request a total of 7*2=14 registers

    // "(*wireInterface).read()<<8 | (*wireInterface).read();" means two registers are read and stored in the same int16_t
    // variable
    for (int i = 0; i < 3; i++) {
        accelerometer[i] = (*wireInterface).read() << 8 | (*wireInterface).read();
    }

    *temperature = (*wireInterface).read() << 8 | (*wireInterface).read();

    for (int i = 0; i < 3; i++) {
        gyroscope[i] = (*wireInterface).read() << 8 | (*wireInterface).read();
    }
}

void getPeaks(int* maximum, int* minimum, int reading) {
    if (reading > *maximum) {
        *maximum = reading;
    }

    if (reading < *minimum) {
        *minimum = reading;
    }
}

void setup() {
    Serial.begin(115200);

    Wire.begin();
    initializeIMU(&Wire, IMU_ADDRESS);

    readIMUValues(
        &Wire,
        IMU_ADDRESS,
        accelerometerReadings,
        gyroscopeReadings,
        &temperatureReading
    );

    for (int i = 0; i < 3; i++) {
        accelerometerMaximums[i] = accelerometerReadings[i];
        accelerometerMinimums[i] = accelerometerReadings[i];
    }
}

void loop() {
    readIMUValues(
        &Wire,
        IMU_ADDRESS,
        accelerometerReadings,
        gyroscopeReadings,
        &temperatureReading
    );

    for (int i = 0; i < 3; i++) {
        getPeaks(
            &accelerometerMaximums[i],
            &accelerometerMinimums[i],
            accelerometerReadings[i]
        );
    }

    Serial.printf("X(%d, %d, %d)| ", accelerometerReadings[0], accelerometerMaximums[0], accelerometerMinimums[0]);
    Serial.printf("Y(%d, %d, %d)| ", accelerometerReadings[1], accelerometerMaximums[1], accelerometerMinimums[1]);
    Serial.printf("Z(%d, %d, %d)\n", accelerometerReadings[2], accelerometerMaximums[2], accelerometerMinimums[2]);

    delay(100);
}

// After rotating the IMU around a lot, the following data was obtained.
// X(59012, 65472, 164)| Y(32767, 32767, 32767)| Z(3508, 65532, 32)