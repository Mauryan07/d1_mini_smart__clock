#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

const char* ssidList[] = {"MotoG", "0range", "WiFi_AP_3"};
const char* passwordList[] = {"oooooooo", "oooooooo", "oooooooo"};
const int numAccessPoints = sizeof(ssidList) / sizeof(ssidList[0]);
bool connectedToWifi = false;

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 19800;  // GMT+5:30
const int daylightOffset_sec = 0;

const int DHTPin = D5;

const int touchPin = D0;

const int ledPin = LED_BUILTIN;

Adafruit_SSD1306 display(128, 32, &Wire, -1);


DHT dht(DHTPin, DHT22);


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, gmtOffset_sec, daylightOffset_sec);

// Variables for time and temperature
unsigned long lastSyncTime = 0;
unsigned long syncInterval = 600000;          
unsigned long lastTempUpdate = 0;
unsigned long tempUpdateInterval = 20000;      
unsigned long displayWakeTime = 0;
unsigned long displayWakeDuration = 60000;    

// Temperature and humidity variables
float temperature = 0.0;
float humidity = 0.0;

void setup() {
 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  
  display.ssd1306_command(SSD1306_SETCONTRAST);
  display.ssd1306_command(10);  // contrast (0-255)

  // Attempt to connect to Wi-Fi access points
  for (int i = 0; i < numAccessPoints; i++) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("Connecting to: ");
    display.println(ssidList[i]);
    display.display();

    WiFi.begin(ssidList[i], passwordList[i]);
    int retries = 0;
    while (WiFi.status() != WL_CONNECTED && retries < 20) {
      delay(500);
      retries++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      connectedToWifi = true;
      break; // Connected to an access point, no need to try others
    } else {
      WiFi.disconnect();
    }
  }

  if (!connectedToWifi) {
    while (1) {
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("Failed to connect to Wi-Fi!");
      display.display();
      delay(2000);
      display.clearDisplay();
      display.display();
      delay(1000);
    }
  }

  dht.begin();

  timeClient.begin();
  timeClient.update();

  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);

  pinMode(ledPin, OUTPUT);

  pinMode(touchPin, INPUT);
}

void loop() {
  unsigned long currentTime = millis();
  if (currentTime - lastSyncTime >= syncInterval) {
    lastSyncTime = currentTime;
    updateTime();

    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);

    delay(5000);
  }

  bool isConnected = WiFi.status() == WL_CONNECTED;

  if (currentTime - lastTempUpdate >= tempUpdateInterval) {
    lastTempUpdate = currentTime;
    readTemperature();
  }

  bool isTouched = digitalRead(touchPin) == HIGH;

  if (isTouched) {
    displayWakeTime = currentTime;
    turnOnDisplay();
  } else if (currentTime - displayWakeTime >= displayWakeDuration) {
    turnOffDisplay();
  }

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print(temperature, 1);
  display.println("C");

  display.setCursor(98, 0);
  display.print(humidity, 1);
  display.println("%");

  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(20, 9);
  display.print(formatTime(timeClient.getHours(), timeClient.getMinutes()));

  int secondsElapsed = timeClient.getSeconds();
  int progressBarWidth = map(secondsElapsed, 0, 59, 0, 128);
  display.fillRect(0, 31, progressBarWidth, 1, WHITE);

  int totalMinutes = timeClient.getHours() * 60 + timeClient.getMinutes();
  int lineStartX = map(totalMinutes, 300, 1440, 0, 128);
  if (totalMinutes >= 300) {
    display.drawFastHLine(lineStartX, 7, 128 - lineStartX, WHITE);
  }

  display.display();

  digitalWrite(ledPin, isConnected ? LOW : HIGH);

  delay(10);
}

void updateTime() {

  timeClient.update();
}
 
void readTemperature() {

  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
}

void turnOnDisplay() {

  display.ssd1306_command(SSD1306_DISPLAYON);
}

void turnOffDisplay() {

  display.ssd1306_command(SSD1306_DISPLAYOFF);
}

String formatTime(int hours, int minutes) {
  String timeString = "";

  if (hours < 10) {
    timeString += "0";
  }
  timeString += hours;
  timeString += ":";

  if (minutes < 10) {
    timeString += "0";
  }
  timeString += minutes;

  return timeString;
}
