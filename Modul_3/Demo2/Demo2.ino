#include <Wire.h>
#include <MPU6050_tockn.h>  // Library Tockn untuk MPU6050
#include <WiFi.h>  // Library WiFi untuk ESP32

MPU6050 mpu(Wire);
const char* ssid = "A50";        // Ganti dengan SSID WiFi Anda
const char* password = ""; // Ganti dengan password WiFi Anda

int buzzerPin = 23; // Pin untuk Buzzer
float threshold = 30.0;  // Ambang batas sudut
float initialAngleX, initialAngleY, initialAngleZ;  // Sudut awal (kalibrasi sisi depan)

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);  // SDA di GPIO 21, SCL di GPIO 22
  mpu.begin();
  mpu.calcGyroOffsets(true);  // Kalibrasi sensor

  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  connectToWiFi();
  calibrateInitialPosition();  // Membaca dan menyimpan sudut awal (sisi depan)
}

void loop() {
  mpu.update();  // Memperbarui data sensor

  if (WiFi.status() == WL_CONNECTED) {
    float angleX = mpu.getAngleX();
    float angleY = mpu.getAngleY();
    float angleZ = mpu.getAngleZ();

    displayAngles(angleX, angleY, angleZ);
    checkAngleThreshold(angleX, angleY, angleZ);

    delay(500);  // Jeda 500ms
  }
}

void connectToWiFi() {
  Serial.print("\nMenghubungkan ke WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" Terhubung!");
}

void calibrateInitialPosition() {
  // Membaca sudut awal saat perangkat dinyalakan
  mpu.update();
  initialAngleX = mpu.getAngleX();
  initialAngleY = mpu.getAngleY();
  initialAngleZ = mpu.getAngleZ();

  Serial.println("Kalibrasi selesai. Sudut awal (Sisi Depan):");
  displayAngles(initialAngleX, initialAngleY, initialAngleZ);
}

void displayAngles(float angleX, float angleY, float angleZ) {
  Serial.print("Sudut X: ");
  Serial.print(angleX);
  Serial.print(" | Sudut Y: ");
  Serial.print(angleY);
  Serial.print(" | Sudut Z: ");
  Serial.println(angleZ);
}

void checkAngleThreshold(float angleX, float angleY, float angleZ) {
  // Hitung selisih sudut dari posisi awal
  float deltaX = angleX - initialAngleX;
  float deltaY = angleY - initialAngleY;
  float deltaZ = angleZ - initialAngleZ;

  if (deltaX > threshold) {
    playTone(1000, "Kanan");  // Nada untuk arah kanan
  } else if (deltaX < -threshold) {
    playTone(1500, "Kiri");  // Nada untuk arah kiri
  } else if (deltaY > threshold) {
    playTone(2000, "Depan");  // Nada untuk arah depan
  } else if (deltaY < -threshold) {
    playTone(2500, "Belakang");  // Nada untuk arah belakang
  } else if (abs(deltaZ) > threshold) {
    playTone(3000, "Rotasi");  // Nada untuk rotasi di sumbu Z
  }
}

void playTone(int frequency, const char* direction) {
  Serial.print("Arah: ");
  Serial.println(direction);
  tone(buzzerPin, frequency);  
  delay(1000);  
  noTone(buzzerPin);  
}
