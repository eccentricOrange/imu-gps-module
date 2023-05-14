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


___

# Setting up WSL2 to use USB devices
The first problem to sort out was to get WSL2 to see the USB device (which in my case, was connected to COM5 on Windows 11). This could be sorted out by using a tool called [`usbipd-win`](https://github.com/dorssel/usbipd-win/wiki/WSL-support). In short:
1.  Install the tool in Windows

    ```powershell
    winget install usbipd-win
    ```

1.  Add it to your PATH if it is not already there. It should be, so first [check your path](https://www.computerhope.com/issues/ch000549.htm) (you're looking for `PATH` under both the user and system variables). If it is not there, add it. If it is there but the tool is not working, try restarting your computer.
1.  Install the Linux tools

    ```bash
    sudo apt install linux-tools-virtual hwdata
    sudo update-alternatives --install /usr/local/bin/usbip usbip `ls /usr/lib/linux-tools/*/usbip | tail -n1` 20
    ```

1.  Give the appropriate WSL distro access to the USB device (from an admin shell in Windows)

    ```powershell
    usbipd wsl list  # list devices and note you device's BUSID
    usbipd wsl attach --busid 4-4  # replace 4-4 with your device's BUSID
    ```

    If needed, specify the distro like this:
    ```powershell
    usbipd wsl attach --busid 4-4 --distribution Ubuntu-20.04
    ```

1.  In your Linux shell, run the following command.

    ```bash
    dmesg | tail
    ```

    Look for an entry for the name of your device and the port. In my case, it was `/dev/ttyUSB0`.

    You need to allow yourself to use this device. You can do this using `chmod`: [\[source\]](https://www.instructables.com/ESP32-Development-on-Windows-Subsystem-for-Linux/)
    
    ```bash
    sudo chmod 0666 /dev/ttyUSB0
    ```

1.  At this point, you should be ready to use PlatformIO, ESP-IDF, or whatever you like.

# Hardware setup

| Component | Pin/Wire | ESP32 pin |
| --- | --- | --- |
| MPU 6050 | VCC | 3.3V |
| MPU 6050 | GND | GND |
| MPU 6050 | SCL | 22 |
| MPU 6050 | SDA | 21 |
| GPS | VCC (Red) | 5V |
| GPS | GND (Black) | GND |
| GPS | TX (Green) | RX2 |
| GPS | RX (Yellow) | TX2 |


# Getting started with micro-ROS
Beware this step. It is lots and lots of downloading. So plan in advance; set up a good internet connection and have a good amount of time to spare (or do it while you're going to do something else). Especially the ESP32-IDF is a long download.

1.  **Install ROS**

    For the most part, just follow the docs: https://docs.ros.org/en/humble/Installation/Ubuntu-Install-Debians.html

    I faced a lot of troubles with dependencies, and here is what I learnt to resolve them:
    *   Do not create a Python virtual environment, or use something like [`pyenv`](https://github.com/pyenv/pyenv) (ideally, don't even have it installed). Just let it use the system Python.
    *   Update your Ubuntu before installing ROS.
    *   Install the full ROS2 system using the "Desktop Install" option.

        ```bash
        sudo apt install ros-humble-desktop
        ```

    *   Install the development tools.

        ```bash
        sudo apt install ros-dev-tools
        ```

    *   It's a good idea to add the ROS2 environment setup script to your shell's rc file. In my case, I added the following as the last line of my `~/.bashrc` file.

        ```bash
        source /opt/ros/humble/setup.bash
        ```

    *   Having a fresh Ubuntu 22 install helps significantly.

1.  **Setup micro-ROS**
    
    The micro-ROS Linux tutorial is a solid starting point: https://micro.ros.org/docs/tutorials/core/first_application_linux/

    But make sure you've got ROS installed properly first, or you're going to end up just fighting dependencies.