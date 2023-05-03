# Task definition

## Current infra available to RoverX

**GPS:** neo 8m

**IMU:** mpu 6050, mpu 9250


## Task statement

Make a stand-alone module that provides IMU and GPS data to the main controller.

*   Jetson Nano/Orin can subscribe to the relevant topics. I will use my Raspberry Pi for testing this.
*   The module will publish the data on the topics
*   The module should not require additional configuration in the existing system
*   The ultimate goal shall be to make a PCB that can be used as plug-and-play (though it will only provide raw data such as latitude and longitude).

## Plan

Infrastructure:
*   **Microcontroller:** ESP32
*   **Main controller:** Raspberry Pi 4
*   **ROS2 version:** Humble
*   **IMU:** MPU 9250
*   **GPS:** u-blox NEO 8M

### Week 1
1.  Research on good IMU and GPS. With existing knowledge, 8M is a good GPS but this knowledge may be out-dated. Need to look up about IMU. I plan to try and find info on IMUs used in smartphones because these are proven to be reliable/precise.
1.  First make a basic system to get both systems' data to a microcontroller (such as ESP32).
1.  Next make a reliable system to send some (dummy) data from microcontroller to main controller.
1.  Next, integrate the two ideas. This will be a prototype (like breadboard or jumper cables).

### Week 2
1.  At this stage, I will try and understand if the circuit needs additional components (such as a USB-to-UART bridge, resistors, capacitors etc). I might need to seek help for this stage.
1.  Transplant the design to a prototyping PCB.
1.  [CANCELLED] ~~Next, I will try and make a PCB with CAD. I will try and make a PCB that can be used as a plug-and-play module. I will try and finish this by third week.~~
1.  [CANCELLED] ~~Fourth week will be manufacturing and testing.~~

### Week 3
1.  Apply filtering

A full week is being allocated to this because I have virtually no idea about filtering. I will have to learn from scratch. Event if I use external libraries, I want to at least know the high-level concept.

### Week 4
1.  Research which ROS2 protocols/models are good.
1.  If pub/sub model doesn't turn out to be the best, migrate to whatever is the best (probably actionlib).