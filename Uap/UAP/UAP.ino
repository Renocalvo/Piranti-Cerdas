#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

// Pin Configuration
#define DHTPIN 4          // DHT11 pin
#define DHTTYPE DHT11     // DHT type
#define MQ135PIN 34       // MQ135 analog pin
#define MQ7PIN 35         // MQ7 analog pin

// WiFi Configuration
const char* ssid = "Jaringan sibuk";
const char* password = "Nenobayuramadhan";
const char* serverUrl = "http://192.168.1.19/PirdasUAP/get_data.php";

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Global variables
float humidity = 0;
float temperature = 0;
int MQ135Value = 0;
int MQ7Value = 0;

void setup() {
    Serial.begin(115200);
    dht.begin();

    // Initialize WiFi
    setupWiFi();
}

void setupWiFi() {
    Serial.print("Connecting to WiFi");
    WiFi.begin(ssid, password);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi Connected!");
        Serial.println("IP: " + WiFi.localIP().toString());
    } else {
        Serial.println("\nWiFi Connection Failed!");
    }
}

void readSensors() {
    // Read DHT11
    float newTemp = dht.readTemperature();
    float newHum = dht.readHumidity();

    // Validate DHT11 readings
    if (!isnan(newTemp) && !isnan(newHum)) {
        temperature = newTemp;
        humidity = newHum;
    } else {
        Serial.println("Failed to read from DHT sensor!");
    }

    // Read MQ135 and MQ7
    MQ135Value = analogRead(MQ135PIN);
    MQ7Value = analogRead(MQ7PIN);
}

void sendDataToServer() {
    if (WiFi.status() == WL_CONNECTED) {
    String url = serverUrl;
    url += "?temperature=" + String(temperature, 2);
    url += "&humidity=" + String(humidity, 2);
    url += "&TVOC=" + String(MQ135Value);
    url += "&CO2=" + String(MQ7Value);

    HTTPClient http;
    http.begin(url);

    int httpResponseCode = http.GET();
    if (httpResponseCode == 200) {
      String response = http.getString();
      Serial.println("Data sent successfully!");
      Serial.println("Response: " + response);
    } else {
      Serial.print("HTTP Request failed. Code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi not connected. Cannot send data.");
  }
}

void printReadings() {
    Serial.println("\n=== Sensor Readings ===");
    Serial.println("Temperature: " + String(temperature, 1) + " °C");
    Serial.println("Humidity: " + String(humidity, 1) + " %");
    Serial.println("MQ135 (TVOC/Smoke): " + String(MQ135Value));
    Serial.println("MQ7 (CO2): " + String(MQ7Value));
    Serial.println("=====================");
}

void loop() {
    // Check WiFi connection
    if (WiFi.status() != WL_CONNECTED) {
        setupWiFi(); 
    }

    // Read sensor data
    readSensors();

    // Print readings to Serial
    printReadings();

    // Send data to server
    sendDataToServer();

    // Wait before next reading
    delay(5000);
}
