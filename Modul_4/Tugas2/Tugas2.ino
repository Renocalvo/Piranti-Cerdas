#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <MPU6050.h>

const char* ssid = "your_ssid";         
const char* password = "your_password";  
const char* serverUrl = "http://your_server_ip/save_sensor_data.php";

MPU6050 mpu(Wire);

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    // Menunggu koneksi WiFi
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Menghubungkan ke WiFi...");
    }
    Serial.println("Terhubung ke WiFi");

    Wire.begin();
    mpu.begin();
}

void loop() {
    mpu.update();

    // Membaca data sumbu X dan Y dari MPU6050
    float x = mpu.getAngleX();
    float y = mpu.getAngleY();

    String arah = "";
    float angka_sumbu = 0;

    // Mengklasifikasikan kemiringan
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
    }

    // Kirim data hanya jika ada arah yang valid
    if (arah != "") {
        if (WiFi.status() == WL_CONNECTED) {
            HTTPClient http;
            http.begin(serverUrl);
            http.addHeader("Content-Type", "application/json");

            // Menyiapkan data JSON
            String jsonData = "{\"angka_sumbu\":" + String(angka_sumbu) + ",\"arah\":\"" + arah + "\"}";

            // Mengirimkan request POST
            int httpResponseCode = http.POST(jsonData);

            // Cek status response dari server
            if (httpResponseCode > 0) {
                String response = http.getString();
                Serial.println("Response dari server:");
                Serial.println(response);
            } else {
                Serial.print("Error mengirim data: ");
                Serial.println(httpResponseCode);
            }

            http.end();
        }
    }

    delay(10000); // Kirim setiap 5 detik (sesuaikan sesuai kebutuhan)
}

