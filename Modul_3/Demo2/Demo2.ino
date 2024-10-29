#include <Wire.h>
#include <MPU6050_tockn.h>
#include <WiFi.h>

// Informasi WiFi
const char* ssid = "Jaringan sibuk";
const char* password = "Nenobayuramadhan";

// Deklarasi sensor dan buzzer
MPU6050 mpu(Wire);
#define BUZZER_PIN 23

// Ambang batas sudut
const float THRESHOLD_ANGLE = 1.0;

void setup() {
  Serial.begin(115200);
  Wire.begin();
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
  mpu.begin();
  mpu.calcGyroOffsets(true);  // Kalibrasi sensor
  Serial.println("MPU6050 siap!");
}

void loop() {
  mpu.update();  // Perbarui data sensor

  // Baca sudut kemiringan pada sumbu X dan Y
  float angleX = mpu.getAngleX();
  float angleY = mpu.getAngleY();

  // Tampilkan data sudut di Serial Monitor
  Serial.print("Sudut X: ");
  Serial.print(angleX);
  Serial.print(" | Sudut Y: ");
  Serial.println(angleY);

  // Debugging: Pastikan nilai sudut tidak selalu sama
  if (angleX == 0 && angleY == 0) {
    Serial.println("Sensor tidak membaca perubahan.");
  }

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
