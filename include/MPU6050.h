/* MPU6050.h - Code for using the MPU6050 accelerometer and gyroscope.
Much of this was sourced from https://wolles-elektronikkiste.de/en/mpu6050-accelerometer-and-gyroscope
*/

#if !defined(MPU6060_h)
#define MPU6060_h

#include <Arduino.h>
#include <Wire.h>

class MPU6050 {
    private:
        TwoWire* _wireInterface;
        int _address;
        const int16_t _ACCELEROMETER_MAXIMUMS[3] = {6350, 32767, 14450};
        const int16_t _ACCELEROMETER_MINIMUMS[3] = {-26350, -32768, -18400};

    public:
        MPU6050(TwoWire* wireInterface, int address);
        void initialize();
        void readValues(
            int16_t accelerometer[3],
            int16_t gyroscope[3],
            int16_t* temperature
        );
        void printAccelerometerPeaks(Stream* serialInterface, int16_t accelerometerReadings[3]);
        float* correctAccelerometer(int16_t accelerometerReadings[3]);
};  // class MPU6050

MPU6050::MPU6050(TwoWire* wireInterface, int address) {
    _wireInterface = wireInterface;
    _address = address;
}  // MPU6050::MPU6050

void MPU6050::initialize() {
    (*_wireInterface).beginTransmission(_address);
    (*_wireInterface).write(0x6B);  // PWR_MGMT_1 register
    (*_wireInterface).write(0);     // wake up!
    (*_wireInterface).endTransmission(true);
}  // MPU6050::initialize

void MPU6050::readValues(int16_t accelerometer[3], int16_t gyroscope[3], int16_t* temperature) {
    (*_wireInterface).beginTransmission(_address);
    (*_wireInterface).write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
    (*_wireInterface).endTransmission(false);  // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
    (*_wireInterface).requestFrom(_address, 14, true);  // request a total of 7*2 = 14 registers

    // "(*_wireInterface).read()<<8 | (*_wireInterface).read();" means two registers are read and
    // stored in the same int16_t variable

    // Read the registers:
    // X: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
    // Y: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
    // Z: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
    for (size_t i = 0; i < 3; i++) {
        accelerometer[i] = (*_wireInterface).read() << 8 | (*_wireInterface).read();
    }

    // Temperature: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
    *temperature = (*_wireInterface).read() << 8 | (*_wireInterface).read();

    // X: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
    // Y: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
    // Z: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)
    for (size_t i = 0; i < 3; i++) {
        gyroscope[i] = (*_wireInterface).read() << 8 | (*_wireInterface).read();
    }
}  // MPU6050::readValues

void MPU6050::printAccelerometerPeaks(Stream* serialInterface, int16_t accelerometerReadings[3]) {
    // initialize to extreme values to prevent false positives
    static int16_t accelerometerMaximums[3] = {-32768, -32768, -32768};
    static int16_t accelerometerMinimums[3] = {32767, 32767, 32767};

    for (size_t i = 0; i < 3; i++) {
        if (accelerometerReadings[i] > accelerometerMaximums[i]) {
            accelerometerMaximums[i] = accelerometerReadings[i];
        }

        if (accelerometerReadings[i] < accelerometerMinimums[i]) {
            accelerometerMinimums[i] = accelerometerReadings[i];
        }
    }

    (*serialInterface).printf("X(%d, %d, %d) | ", accelerometerReadings[0], accelerometerMaximums[0], accelerometerMinimums[0]);
    (*serialInterface).printf("Y(%d, %d, %d) | ", accelerometerReadings[1], accelerometerMaximums[1], accelerometerMinimums[1]);
    (*serialInterface).printf("Z(%d, %d, %d)\n", accelerometerReadings[2], accelerometerMaximums[2], accelerometerMinimums[2]);
}  // MPU6050::printAccelerometerPeaks

float* MPU6050::correctAccelerometer(int16_t accelerometerReadings[3]) {
    static float correctedAccelerometerReadings[3];

    for (size_t i = 0; i < 3; i++) {
        correctedAccelerometerReadings[i] = (float)(accelerometerReadings[i] - _ACCELEROMETER_MINIMUMS[i]) / (_ACCELEROMETER_MAXIMUMS[i] - _ACCELEROMETER_MINIMUMS[i]) * 2 - 1;
    }

    return correctedAccelerometerReadings;
}  // MPU6050::correctAccelerometer

#endif // MPU6060_h
