#define BLYNK_TEMPLATE_ID "TMPL6-v_Gt08s"
#define BLYNK_TEMPLATE_NAME "Codelab6"
#define BLYNK_AUTH_TOKEN "4_4_yepqueig_zrP0CTh2n_sCvs48qwq"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

char ssid[] = "Test1";
char pass[] = "12345678";

// DHT Sensor Pin configuration
#define DHTPIN 15           
#define DHTTYPE DHT11       
DHT dht(DHTPIN, DHTTYPE);

// Buzzer Pin configuration
#define BUZZER_PIN 32       // Pin untuk buzzer

BlynkTimer timer;           // Timer untuk mengirim data secara berkala

bool switchStatus = false;  // Status tombol switch, default OFF (0)

void sendData() {
    if (switchStatus) {
        // Membaca suhu jika tombol switch ON
        float suhu = dht.readTemperature();

        if (isnan(suhu)) {
            Serial.println("Gagal membaca sensor DHT!");
            return;
        }

        // Menampilkan suhu di Serial Monitor
        Serial.print("Suhu: ");
        Serial.print(suhu);
        Serial.println(" °C");

        // Kirim data suhu ke Blynk
        Blynk.virtualWrite(V0, suhu);

        // Mengontrol buzzer berdasarkan suhu
        if (suhu >= 30) {
            // Suhu panas: suara keras
            tone(BUZZER_PIN, 1000);
            Blynk.virtualWrite(V1, "Panas");
        } else if (suhu >= 25 && suhu < 30) {
            // Suhu hangat: suara sedang
            tone(BUZZER_PIN, 500);
            Blynk.virtualWrite(V1, "Hangat");
        } else {
            // Suhu normal: buzzer mati
            noTone(BUZZER_PIN);
            Blynk.virtualWrite(V1, "Normal");
        }
    } else {
        // Jika tombol OFF, buzzer mati dan sensor DHT tidak membaca
        noTone(BUZZER_PIN); // Matikan buzzer
        Blynk.virtualWrite(V1, "Normal"); // Status Normal
    }
}

void setup() {
    Serial.begin(9600);          // Memulai komunikasi serial
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass); // Koneksi ke Blynk
    dht.begin();                 // Inisialisasi sensor DHT
    pinMode(BUZZER_PIN, OUTPUT); // Atur pin buzzer sebagai output

    // Set timer untuk mengirim data setiap 2 detik
    timer.setInterval(1000L, sendData);
}

void loop() {
    Blynk.run();    // Menjalankan fungsi Blynk
    timer.run();    // Menjalankan fungsi timer
}

// Fungsi untuk membaca status tombol di Blynk dan mengubah status switch
BLYNK_WRITE(V2) {
    switchStatus = param.asInt();  // Mendapatkan status tombol switch (ON/OFF)

    if (switchStatus == 1) {
        // Jika tombol switch di ON, buzzer menyala dan sensor DHT berfungsi
        Blynk.virtualWrite(V1, "Panas");
    } else {
        // Jika tombol switch di OFF, buzzer mati dan sensor DHT tidak membaca
        Blynk.virtualWrite(V1, "Normal");
    }
}
