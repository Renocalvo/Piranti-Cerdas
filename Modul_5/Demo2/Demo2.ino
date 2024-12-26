#define BLYNK_TEMPLATE_ID "TMPL6UZ9dCFnf"
#define BLYNK_TEMPLATE_NAME "Modul5second"
#define BLYNK_AUTH_TOKEN "3G2JHmK8f5LB-5-n8rYJcF3beype76pl"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "Test";
char pass[] = "12345678";

// LDR Pin configuration
const int ldrPin = 34;
const int lampPin = 32; 
BlynkTimer timer;

// Variabel untuk menyimpan status switch dan mode
bool switchStatus = false;  // status switch (ON/OFF)
bool modeAuto = true;       // status mode (Auto/Manual)

// Fungsi untuk mengirim data LDR dan status lampu ke Blynk
void sendData() {
    int ldrValue = analogRead(ldrPin);
    Serial.print("LDR VALUE: ");
    Serial.println(ldrValue);
    
    // Mengirim data LDR ke Blynk
    Blynk.virtualWrite(V0, ldrValue);
    
    // Mengirim status cahaya ke Blynk (Terang atau Gelap)
    String lightStatus = (ldrValue < 500) ? "Terang" : "Gelap";
    Blynk.virtualWrite(V1, lightStatus);

    // Menentukan status lampu berdasarkan mode dan cahaya
    bool lampStatus = false; // Defaultnya mati

    if (modeAuto) {
        // Mode Auto: Lampu menyala jika gelap (LDR < 500)
        lampStatus = (ldrValue < 500);  // Lampu menyala jika LDR < 500
    } else {
        // Mode Manual: Lampu dikendalikan oleh tombol switch
        lampStatus = switchStatus;  // Lampu menyala sesuai tombol switch
    }

    // Kirim status lampu ke Blynk
    Blynk.virtualWrite(V2, lampStatus ? "false" : "true");

    // Mengontrol lampu berdasarkan mode dan kondisi
    digitalWrite(lampPin, lampStatus ? HIGH : LOW); // Menyalakan atau mematikan lampu
}


void setup() {
    // Start serial communication for debugging
    Serial.begin(9600);

    // Initialize Blynk with credentials
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

    // Set the lampPin sebagai output
    pinMode(lampPin, OUTPUT);

    // Set a timer to send LDR data and lamp status every second
    timer.setInterval(1000L, sendData);
}

void loop() {
    Blynk.run(); // Run Blynk
    timer.run(); // Run timer for periodic tasks
}

// Fungsi untuk mengontrol lampu melalui tombol Blynk (switch ON/OFF)
BLYNK_WRITE(V3) {
    switchStatus = param.asInt(); // Mendapatkan nilai dari tombol (ON/OFF)
    
    // Menyimpan status lampu berdasarkan tombol
    if (switchStatus == 1) {
        Blynk.virtualWrite(V2, "false"); // Lampu ON
    } else {
        Blynk.virtualWrite(V2, "true"); // Lampu OFF
    }
}

// Fungsi untuk mengganti mode Auto/Manual melalui tombol di Blynk
BLYNK_WRITE(V4) {
    modeAuto = param.asInt();  // Mendapatkan nilai mode (1 untuk Auto, 0 untuk Manual)
    
    // Memberikan feedback ke Blynk
    if (modeAuto) {
        Blynk.virtualWrite(V5, "Auto Mode");  // Kirim status mode Auto
    } else {
        Blynk.virtualWrite(V5, "Manual Mode");  // Kirim status mode Manual
    }
}