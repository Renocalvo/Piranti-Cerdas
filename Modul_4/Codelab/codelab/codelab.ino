#include <Arduino.h>
#include <HTTPClient.h>  // Library for HTTP requests
#include <WiFi.h>        // Library for WiFi functionality

#define SENSORPIN 34     // Define pin for LDR sensor
#define LEDPIN 25        // Define pin for LED

// WiFi credentials
const char* ssid = "Yoe_Dean";
const char* password = "12345678";

// Server URL for sending data
const char* serverName = "http://192.168.254.231/Pirdas-Modul4/connection.php";

void setup() {
  Serial.begin(9600);           // Start serial communication
  pinMode(LEDPIN, OUTPUT);      // Set LED pin as output
  delay(1000);                  // Wait for a second

  // Connect to Wi-Fi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {   // Wait until connected
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Display WiFi information
  String wifiName = WiFi.SSID();                   // Get WiFi SSID
  String localWiFiIP = WiFi.localIP().toString();  // Get local IP address
  Serial.println();
  Serial.println("Connected to " + wifiName);
  Serial.println("Local IP WIFI: " + localWiFiIP);
}

void loop() {
  int ldr_value = analogRead(SENSORPIN);  // Read LDR sensor value

  // Check if LDR value is above threshold
  if (ldr_value < 1000) {
    Serial.println("Cahaya Terang ");     // Print "Bright Light"
    digitalWrite(LEDPIN, LOW);            // Turn off LED
    Serial.print("Intensitas Cahaya: ");  // Print light intensity
    Serial.print(ldr_value);
    Serial.println();
  } else {
    Serial.println("Cahaya Gelap ");      // Print "Dark Light"
    digitalWrite(LEDPIN, HIGH);           // Turn on LED
    Serial.print("Intensitas Cahaya: ");  // Print light intensity
    Serial.print(ldr_value);
    Serial.println();
  }
  
  delay(1000);  // Wait 1 second before next read
  
  // Prepare URL with LDR value
  String url = serverName;
  url += "?ldr_value=";
  url += String(ldr_value);

  // HTTP request to send data
  HTTPClient http;
  http.begin(url);                     // Begin HTTP connection with URL
  int httpResponseCode = http.GET();   // Send GET request
  
  // Check HTTP response
  if (httpResponseCode == 200) {       // Check if response is OK
    Serial.println("Data sent successfully");
  } else {
    Serial.print("Error code: ");      // Print error code if not successful
    Serial.println(httpResponseCode);
  }

  http.end();          // End HTTP connection
  delay(1000);        // Wait 15 seconds before next HTTP request
}

