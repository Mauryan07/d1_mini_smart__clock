/******************************************************
 * Project: NTP clock
 * Features:
 *  - Display temperature, humidity, and time on an OLED display
 *  - Automatically synchronize time with NTP server
 *  - Read temperature and humidity from DHT22 sensor
 *  - Connect to Wi-Fi network for NTP synchronization
 *  - Control built-in LED based on Wi-Fi connection status
 *  - Wake up the display for 1 minute when the touch sensor is triggered
 ******************************************************/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

// Replace with your network credentials
const char* ssid = "0range";
const char* password = "oooooooo";

// NTP Server details
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 19800;  // GMT+5:30
const int daylightOffset_sec = 0;

// Pin for DHT22 sensor
const int DHTPin = D5;

// Pin for touch sensor
const int touchPin = D0;

// Pin for built-in LED
const int ledPin = LED_BUILTIN;

// Initialize the OLED display
Adafruit_SSD1306 display(128, 32, &Wire, -1);

// Initialize the DHT sensor
DHT dht(DHTPin, DHT22);

// Initialize the NTP client
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, gmtOffset_sec, daylightOffset_sec);

// Variables for time and temperature
unsigned long lastSyncTime = 0;
unsigned long syncInterval = 600000;          // 10 minutes
unsigned long lastTempUpdate = 0;
unsigned long tempUpdateInterval = 20000;      // 5 seconds
unsigned long displayWakeTime = 0;
unsigned long displayWakeDuration = 60000;    // 1 minute

// Temperature and humidity variables
float temperature = 0.0;
float humidity = 0.0;

void setup() {
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  // Configure DHT sensor
  dht.begin();

  // Initialize display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  // Connect to NTP server
  timeClient.begin();
  timeClient.update();

  // Turn off Wi-Fi to save power
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);

  // Set LED pin as output
  pinMode(ledPin, OUTPUT);

  // Set touch sensor pin as input
  pinMode(touchPin, INPUT);
}

void loop() {
  // Update time from NTP server at the specified interval
  unsigned long currentTime = millis();
  if (currentTime - lastSyncTime >= syncInterval - 1000 && currentTime - lastSyncTime < syncInterval) {
    // Turn on Wi-Fi 1 second before sync interval
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
  }
  if (currentTime - lastSyncTime >= syncInterval) {
    lastSyncTime = currentTime;
    updateTime();

    // Turn off Wi-Fi after updating time
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
  }

  // Update temperature and humidity at the specified interval
  if (currentTime - lastTempUpdate >= tempUpdateInterval) {
    lastTempUpdate = currentTime;
    readTemperature();
  }

  // Check if touch sensor is triggered
  bool isTouched = digitalRead(touchPin) == HIGH;

  // Update display state based on touch sensor
  if (isTouched) {
    // Turn on display and set wake time
    displayWakeTime = currentTime;
    turnOnDisplay();
  } else if (currentTime - displayWakeTime >= displayWakeDuration) {
    // Turn off display if wake duration has elapsed
    turnOffDisplay();
  }

  // Display temperature, humidity, and time
  displayTemperature();
  displayHumidity();
  displayTime();

  display.display();

  // Check Wi-Fi connection status and control LED
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(ledPin, LOW);  // Turn on LED
  } else {
    digitalWrite(ledPin, HIGH); // Turn off LED
  }

  delay(10);
}

void updateTime() {
  // Update time from NTP server
  timeClient.update();
}

void readTemperature() {
  // Read temperature and humidity from DHT sensor
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
}

void displayTemperature() {
  // Display temperature on the OLED display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print(temperature, 1);
  display.println("C");
}

void displayHumidity() {
  // Display humidity on the OLED display
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(98, 0);
  display.print(humidity, 1);
  display.println("%");
}

void displayTime() {
  // Display time on the OLED display
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(20, 10);

  int hours = timeClient.getHours();
  int minutes = timeClient.getMinutes();

  if (hours < 10) {
    display.print("0");
  }
  display.print(hours);
  display.print(":");
  if (minutes < 10) {
    display.print("0");
  }
  display.print(minutes);
}

void turnOnDisplay() {
  // Turn on the display
  display.ssd1306_command(SSD1306_DISPLAYON);
}

void turnOffDisplay() {
  // Turn off the display
  display.ssd1306_command(SSD1306_DISPLAYOFF);
}
