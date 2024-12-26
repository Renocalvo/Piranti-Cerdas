#define BLYNK_TEMPLATE_ID "TMPL66EVtCfzY"
#define BLYNK_TEMPLATE_NAME "Modul5"
#define BLYNK_AUTH_TOKEN "JSyvL3UseovomJniHSgVnLtBbwQHZNGe"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "Jaringan sibuk";
char pass[] = "Nenobayuramadhan";

// LDR Pin configuration
const int ldrPin = 34;
const int lampPin = 32; 
BlynkTimer timer;

// Variabel untuk menyimpan status switch
bool switchStatus = false; // defaultnya OFF

// Fungsi untuk mengirim data LDR dan status lampu ke Blynk
void sendData() {
    int ldrValue = analogRead(ldrPin);
    Serial.print("LDR VALUE: ");
    Serial.println(ldrValue);
    
    // Mengirim data LDR ke Blynk
    Blynk.virtualWrite(V0, ldrValue);
    
    // Mengirim status cahaya ke Blynk (Terang atau Gelap)
    String lightStatus = (ldrValue < 500) ? "Gelap" : "Terang";
    Blynk.virtualWrite(V1, lightStatus);

    // Menentukan status lampu (ON atau OFF) berdasarkan kondisi
    bool lampStatus = (ldrValue < 500) && switchStatus; // Lampu menyala jika gelap dan switch ON
    Blynk.virtualWrite(V2, lampStatus ? "true" : "false");
    
    // Mengontrol lampu berdasarkan kondisi cahaya dan status switch
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
        Blynk.virtualWrite(V2, "true"); // Lampu ON
    } else {
        Blynk.virtualWrite(V2, "false"); // Lampu OFF
    }
}
