#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <MPU6050_tockn.h>  // Pastikan Anda menggunakan pustaka tockn

const char* ssid = "Yoe_Dean";          // SSID WiFi
const char* password = "12345678";    // Password WiFi
const char* serverUrl = "http://192.168.254.231/Pirdas-Modul4/connection.php";

MPU6050 mpu(Wire);  // Inisialisasi MPU6050 dengan Wire

void setup() {
    Serial.begin(115200);
    delay(1000);

    // Koneksi ke jaringan WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Serial.println("WiFi Connected!");
    Serial.println("SSID: " + String(WiFi.SSID()));
    Serial.println("Local IP: " + WiFi.localIP().toString());

    // Inisialisasi MPU6050
    Wire.begin();
    mpu.begin();
    mpu.calcGyroOffsets(true);  // Hitung offset gyro untuk akurasi
    Serial.println("MPU6050 berhasil diinisialisasi!");
}

void loop() {
    mpu.update();  // Perbarui data dari MPU6050

    float x = mpu.getGyroAngleX();  // Sudut rotasi pada sumbu X
    float y = mpu.getGyroAngleY();  // Sudut rotasi pada sumbu Y

    String arah = "";
    float angka_sumbu = 0;

    // Klasifikasi arah berdasarkan data MPU6050
    if (x > 10) {
        arah = "kiri";
        angka_sumbu = x;
    } else if (x < -10) {
        arah = "kanan";
        angka_sumbu = x;
    } else if (y > 10) {
        arah = "depan";
        angka_sumbu = y;
    } else if (y < -10) {
        arah = "belakang";
        angka_sumbu = y;
    } else {
        arah = "stabil";
    }

    // Menampilkan data ke Serial Monitor
    Serial.print("Sudut X: ");
    Serial.print(x);
    Serial.print(" | Sudut Y: ");
    Serial.print(y);
    Serial.print(" | Arah: ");
    Serial.println(arah);

    // Mengirim data ke server menggunakan HTTP GET
    if (arah != "stabil" && WiFi.status() == WL_CONNECTED) {
        String url = serverUrl;
        url += "?arah=" + arah;
        url += "&angka_sumbu=" + String(angka_sumbu);

        HTTPClient http;
        http.begin(url);

        int httpResponseCode = http.GET();
        if (httpResponseCode == 200) {
            String response = http.getString();
            Serial.println("Data sent successfully!");
            Serial.println("Response: " + response);
        } else {
            Serial.print("Error code: ");
            Serial.println(httpResponseCode);
        }

        http.end();  // Mengakhiri koneksi HTTP
    }

    delay(10000);  // Tunggu 1 detik sebelum membaca ulang
}
