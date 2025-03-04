# SensorHubRpPICO

SensorHubRpPICO is a project developed as part of a microcontroller course, utilizing the Raspberry Pi Pico microcontroller to integrate various sensors and peripherals, creating a versatile sensor hub.
This project was designed to demonstrate microcontroller programming skills and sensor integration as part of my coursework.
The system facilitates environmental monitoring, user interaction, and data display, making it suitable for applications in IoT, data logging, and educational purposes.

![SensorHubRpPICO Prototype](https://github.com/SecCodeSmith/SensorHubRpPICO/blob/main/IMG_2203.jpg)

## Features

- **Environmental Sensing**:
  - **BME680**: Measures temperature, humidity, pressure, and gas (air quality).
  - **BME688**: An enhanced version of the BME680 sensor with additional features.

- **User Interaction**:
  - **Button Module**: Allows user input for various functions.
  - **Buzzer Module**: Provides audible alerts or feedback.
  - **RGB Diode**: Displays visual indicators using different colors.

- **Display**:
  - **LCD Display with I2C Interface**: Shows sensor readings and system information.

## Hardware Requirements

- **Raspberry Pi Pico**
- **BME680 or BME688 Sensor Module**
- **I2C LCD Display Module**
- **Push Button**
- **Buzzer**
- **RGB LED**
- **Connecting Wires and Breadboard or Custom PCB**

## Software Requirements

- **C++ Development Environment**
- **CMake**
- **Raspberry Pi Pico SDK**

## Setup and Installation

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/SecCodeSmith/SensorHubRpPICO.git
   ```
   

2. **Initialize Submodules**:
   ```bash
   git submodule update --init --recursive
   ```
   

3. **Set Up the Development Environment**:
   - Install the Raspberry Pi Pico SDK by following the official [Raspberry Pi Pico SDK setup guide](https://www.raspberrypi.org/documentation/microcontrollers/c_sdk.html).
   - Ensure that CMake is installed and properly configured.

4. **Build the Project**:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```
   

5. **Upload the Firmware**:
   - Connect the Raspberry Pi Pico to your computer while holding the BOOTSEL button to enter USB mass storage mode.
   - Copy the generated `.uf2` file from the `build` directory to the Pico's storage.

## Usage

- Upon powering up, the system initializes all connected sensors and peripherals.
- Sensor readings are displayed on the LCD screen.
- User interactions through the button can trigger specific functions or navigate through different display modes.
- The buzzer and RGB LED provide audible and visual feedback based on sensor data or user actions.

  
## Customization

- Modify the `main.cpp` file to change the behavior of the sensor hub or add new features.
- Adjust the `Menu.cpp` and `Menu.h` files to customize the user interface and menu options.
- Use the provided Fusion 360 design file (`PCB project v9.f3d`) to create a custom PCB for the project.

## Contributing

Contributions are welcome! Feel free to fork the repository, make improvements, and submit pull requests.

## License

This project is licensed under the MIT License. See the `LICENSE` file for more details.
