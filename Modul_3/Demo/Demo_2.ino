#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// Informasi WiFi
const char* ssid = "Jaringan sibuk";
const char* password = "Nenobayuramadhan";

// MPU6050 dan Buzzer
Adafruit_MPU6050 mpu;
#define BUZZER_PIN 23

// Ambang batas sudut
const float THRESHOLD_ANGLE = 30.0;

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);

  // Inisialisasi WiFi
  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nTerhubung ke WiFi!");

  // Inisialisasi MPU6050
  if (!mpu.begin()) {
    Serial.println("MPU6050 tidak ditemukan!");
    while (1);
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.println("MPU6050 siap!");
}

void loop() {
  // Baca data dari sensor
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Hitung sudut
  float angleX = atan2(a.acceleration.y, a.acceleration.z) * 180 / PI;
  float angleY = atan2(a.acceleration.x, a.acceleration.z) * 180 / PI;

  // Tampilkan data di Serial Monitor
  Serial.print("Sudut X: ");
  Serial.print(angleX);
  Serial.print(" | Sudut Y: ");
  Serial.println(angleY);

  // Periksa apakah sudut melebihi ambang batas
  if (abs(angleX) > THRESHOLD_ANGLE || abs(angleY) > THRESHOLD_ANGLE) {
    // Tentukan arah dan bunyi buzzer
    if (angleX > THRESHOLD_ANGLE) {
      Serial.println("Bersandar ke kanan!");
      buzzerBeep(500);  // Bunyi panjang
    } else if (angleX < -THRESHOLD_ANGLE) {
      Serial.println("Bersandar ke kiri!");
      buzzerBeep(100);  // Bunyi pendek
    } else if (angleY > THRESHOLD_ANGLE) {
      Serial.println("Miring ke depan!");
      buzzerBeep(300);  // Bunyi sedang
    } else if (angleY < -THRESHOLD_ANGLE) {
      Serial.println("Miring ke belakang!");
      buzzerBeep(700);  // Bunyi lebih panjang
    }
  } else {
    digitalWrite(BUZZER_PIN, LOW);  // Matikan buzzer jika tidak ada peringatan
  }

  delay(500);  // Tunggu 0.5 detik sebelum membaca ulang
}

// Fungsi untuk mengatur bunyi buzzer
void buzzerBeep(int duration) {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(duration);
  digitalWrite(BUZZER_PIN, LOW);
}
