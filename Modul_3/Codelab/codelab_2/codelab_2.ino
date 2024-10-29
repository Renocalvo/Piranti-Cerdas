#include <WiFi.h>

// WiFi credentials
const char* ssid = "Yoe_Dean";
const char* password = "12345678";

void setup() {
  // Start the serial communication
  Serial.begin(9600);

  // Attempt to connect to WiFi network
  WiFi.begin(ssid, password);

  // Wait until the device connects to WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);  // Wait 1 second
    Serial.println("Connecting to WiFi...");
  }

  // Confirm successful connection
  Serial.println("Connected to WiFi");
}

void loop() {
  // Placeholder for code to be executed repeatedly
}
