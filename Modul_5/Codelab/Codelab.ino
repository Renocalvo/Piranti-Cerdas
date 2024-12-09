#define BLYNK_TEMPLATE_ID "TMPL6W7eiI1Qn"
#define BLYNK_TEMPLATE_NAME "Codelab"
#define BLYNK_AUTH_TOKEN "NTCI_Udaildy6Yo8xUUAUwT7gWC5vFWM"

// Uncomment this line to enable debug prints
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Wi-Fi credentials
char ssid[] = "Hahaha";
char pass[] = "AkbarPalu73";

// LDR Pin configuration
const int ldrPin = 34;
BlynkTimer timer;

// Function to send LDR data to Blynk
void sendData() {
    int ldrValue = analogRead(ldrPin);
    Serial.print("LDR VALUE: ");
    Serial.println(ldrValue);
    Blynk.virtualWrite(V0, ldrValue); // Send data to Blynk Virtual Pin V0
}

void setup() {
    // Start serial communication for debugging
    Serial.begin(9600);

    // Initialize Blynk with credentials
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

    // Set a timer to send LDR data every second
    timer.setInterval(1000L, sendData);
}

void loop() {
    Blynk.run(); // Run Blynk
    timer.run(); // Run timer for periodic tasks
}
