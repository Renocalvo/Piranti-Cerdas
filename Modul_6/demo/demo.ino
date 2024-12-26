#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <MPU6050_tockn.h>  // Pastikan Anda menggunakan pustaka tockn

// Pin konfigurasi untuk LED dan Buzzer
#define LED_PIN 13           // Pin untuk LED
#define BUZZER_PIN 12        // Pin untuk Buzzer

// Koneksi WiFi
const char* ssid = "Jaringan sibuk";
const char* password = "Nenobayuramadhan";
const char* serverUrl = "http://192.168.1.4/Pirdas6/connection.php";  // URL untuk HTTP request

MPU6050 mpu(Wire);  // Inisialisasi MPU6050 dengan Wire

float pitch = 0, roll = 0;  // Variabel untuk sudut pitch dan roll
const float alpha = 0.98;   // Konstanta untuk filter komplementer
unsigned long previousTime = 0;  // Untuk menghitung waktu antar iterasi

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  // Konfigurasi pin LED dan Buzzer
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);   // LED awal mati
  digitalWrite(BUZZER_PIN, LOW); // Buzzer awal mati

  // Koneksi ke WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi Connected!");
  Serial.println("IP Address: " + WiFi.localIP().toString());

  // Inisialisasi MPU6050
  mpu.begin();
  mpu.calcGyroOffsets(true);  // Kalibrasi gyro
  Serial.println("MPU6050 berhasil diinisialisasi!");
}

void loop() {
  mpu.update();

  // Hitung waktu antar iterasi
  unsigned long currentTime = millis();
  float dt = (currentTime - previousTime) / 2000.0;  // Dalam detik
  previousTime = currentTime;

  // Membaca data dari accelerometer dan gyroscope
  float accelPitch = atan2(mpu.getAccY(), mpu.getAccZ()) * 180.0 / PI;
  float accelRoll = atan2(mpu.getAccX(), mpu.getAccZ()) * 180.0 / PI;

  // Integrasikan data gyroscope
  float gyroPitch = pitch + mpu.getGyroX() * dt;
  float gyroRoll = roll + mpu.getGyroY() * dt;

  // Filter komplementer untuk menghitung sudut pitch dan roll
  pitch = alpha * gyroPitch + (1 - alpha) * accelPitch;
  roll = alpha * gyroRoll + (1 - alpha) * accelRoll;

  String status = "";

  // Logika deteksi berdasarkan tabel kalibrasi
  if (pitch >= -15 && pitch <= 15 && roll >= -15 && roll <= 15) {
    // Kondisi Normal (Datar)
    status = "Datar";
    digitalWrite(LED_PIN, HIGH);  // LED menyala
    digitalWrite(BUZZER_PIN, LOW);  // Buzzer mati
} else {
    // Kondisi Miring
    status = "Miring";
    digitalWrite(LED_PIN, LOW);   // LED mati
    digitalWrite(BUZZER_PIN, HIGH);  // Buzzer menyala
}

  // Menampilkan data ke Serial Monitor
  Serial.print("Pitch: ");
  Serial.print(pitch);
  Serial.print(" | Roll: ");
  Serial.print(roll);
  Serial.print(" | Status: ");
  Serial.println(status);

  // Mengirim data ke server
  if (WiFi.status() == WL_CONNECTED) {
    String url = serverUrl;
    url += "?sumbu_x=" + String(pitch, 2);
    url += "&sumbu_y=" + String(roll, 2);
    url += "&status=" + status;

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

  delay(1000);  // Tunggu 100ms sebelum iterasi berikutnya
}
