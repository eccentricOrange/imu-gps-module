#include <Arduino.h>
#include "Wire.h"
#include "imu.h"
// #include <MPU6050.h>



MPU6050 mpu6050(&Wire, 0x68);

int16_t accelerometer[3], gyroscope[3];
float* correctedAccelerometer;
int16_t temperature;

void setup() {
    Serial.begin(115200);

    pinMode(21, INPUT_PULLUP);
    pinMode(22, INPUT_PULLUP);

    Wire.begin(21, 22, 5000);
    
    mpu6050.initialize();
}

void loop() {
    mpu6050.readValues(accelerometer, gyroscope, &temperature);
    correctedAccelerometer = mpu6050.correctAccelerometer(accelerometer);

    Serial.printf(
        "AX:%f,AY:%f,AZ:%f\n",
        correctedAccelerometer[0],
        correctedAccelerometer[1],
        correctedAccelerometer[2]
    );

    delay(100);
}