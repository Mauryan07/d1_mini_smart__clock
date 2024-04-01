# D1 Mini Smart Clock

Welcome to the D1 Mini Smart Clock project! This innovative clock harnesses the power of the ESP8266-based D1 Mini module, DHT22 temperature and humidity sensor, and SSD1306 OLED display to bring you a versatile and energy-efficient timekeeping solution.

## Key Features

- **Energy-Efficient Operation**: Unlike conventional clocks that rely on constant internet connectivity for time synchronization, our smart clock utilizes its local processing power to maintain accurate timekeeping. This results in lower energy consumption and greater autonomy.
- **Offline Time Sync**: You can say goodbye to worries about internet downtime affecting your clock's accuracy. Our smart clock doesn't need a constant internet connection to keep time in sync, ensuring reliable performance even in offline environments.
- **Intuitive Touch Interface**: Interact with your clock effortlessly using the built-in touch interface,to wake the screen.
- **Enhanced Visual Features**: Enjoy a visually captivating experience with features such as time tracking of specified durations and a dynamic second bar on the display. These visual elements not only add flair to your clock but also provide useful information at a glance.
- **Temperature and Humidity Monitoring**: Stay informed about your environment with real-time monitoring of temperature and humidity levels. The integrated DHT22 sensor delivers accurate readings, allowing you to create a comfortable indoor environment.
- **OLED Display**: Immerse yourself in the crisp and vibrant display provided by the SSD1306 OLED screen. Whether it's day or night, the high contrast and readability ensure that you can always check the time with ease.

## Getting Started

### Components Used

- ESP8266 D1 Mini Module
- DHT22 Temperature and Humidity Sensor
- SSD1306 OLED Display (128x64 pixels)

### Dependencies

- [Arduino IDE](https://www.arduino.cc/en/software)
- [ESP8266 Board Manager](https://github.com/esp8266/Arduino#installing-with-boards-manager)
- [Adafruit_SSD1306 Library](https://github.com/adafruit/Adafruit_SSD1306)
- [DHT Sensor Library](https://github.com/adafruit/DHT-sensor-library)

### Setup

1. Connect the D1 Mini Module, DHT22 sensor, and SSD1306 display as per the provided instructions.
2. Install Arduino IDE and the ESP8266 board manager.
3. Install the necessary libraries using the Library Manager in Arduino IDE.
4. Open the `smart_clock.ino` file in Arduino IDE.
5. Configure your Wi-Fi SSID and password in the code.
6. Upload the code to your D1 Mini module.
7. Your D1 Mini Smart Clock is now ready to use!

## Usage

Once the code is uploaded and the connections are set up correctly, your smart clock will begin displaying the current time, date, temperature, and humidity readings. Feel free to customize the display and functionality according to your preferences by modifying the code.

## Contributing

Contributions are highly appreciated! If you have ideas for improvements or new features, please fork this repository, make your changes, and submit a pull request. Additionally, bug reports and feature requests are always welcome.

## License

This project is licensed under the [GNU General Public License v3.0](LICENSE). Feel free to modify and distribute the code in accordance with the terms of the license.

