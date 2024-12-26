#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <MPU6050_tockn.h>  // Pastikan Anda menggunakan pustaka tockn

// Pin konfigurasi untuk LED dan Buzzer
#define LED_PIN 13           // Pin untuk LED
#define BUZZER_PIN 12        // Pin untuk Buzzer

// Koneksi WiFi
const char* ssid = "monyt";
const char* password = "bismillah";
const char* serverUrl = "http://192.168.186.231/Pirdas6/connection.php";  // URL untuk HTTP request

MPU6050 mpu(Wire);  // Inisialisasi MPU6050 dengan Wire

// Variabel untuk nilai referensi
float referenceX = 0, referenceY = 0, referenceZ = 0;  
const float tolerance = 0.2;  // Toleransi 20% dari nilai referensi, bisa disesuaikan

// Deadband: nilai di sekitar referensi yang dianggap datar
const float deadband = 0.05;  // Zona mati 0.05 dari nilai referensi

// Variabel untuk sudut pitch dan roll
float pitch = 0, roll = 0;  

unsigned long previousTime = 0;  // Untuk menghitung waktu antar iterasi

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  // Koneksi WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi Connected!");
  Serial.println("IP Address: " + WiFi.localIP().toString());

  // Inisialisasi MPU6050
  mpu.begin();  // Panggil tanpa memeriksa nilai pengembalian

  // Periksa apakah sensor berhasil diinisialisasi
  Serial.println("MPU6050 berhasil diinisialisasi!");
  
  // Kalibrasi Gyro
  mpu.calcGyroOffsets(true);  // Kalibrasi gyro
  
  // Delay untuk memastikan sensor stabil
  delay(1000);

  // Ambil nilai referensi akselerometer
  referenceX = mpu.getAccX();
  referenceY = mpu.getAccY();
  referenceZ = mpu.getAccZ();

  // Tampilkan nilai referensi
  Serial.println("\nReferensi X: " + String(referenceX));
  Serial.println("Referensi Y: " + String(referenceY));
  Serial.println("Referensi Z: " + String(referenceZ));

  delay(3000);  // Tunggu 3 detik sebelum memulai program
}



void loop() {
  mpu.update();

  // Hitung waktu antar iterasi
  unsigned long currentTime = millis();
  float dt = (currentTime - previousTime) / 2000.0;  // Dalam detik
  previousTime = currentTime;

  // Membaca data dari accelerometer
  float accelX = mpu.getAccX();
  float accelY = mpu.getAccY();
  float accelZ = mpu.getAccZ();

  // Logika untuk menentukan apakah dalam posisi datar atau miring
  String status = "";
  
  // Menghitung selisih antara nilai saat ini dan nilai referensi
  float deltaX = abs(accelX - referenceX);
  float deltaY = abs(accelY - referenceY);
  float deltaZ = abs(accelZ - referenceZ);

  // Jika perubahan lebih besar dari toleransi dan di luar deadband, maka dianggap miring
  if ((deltaX > referenceX * tolerance || deltaY > referenceY * tolerance || deltaZ > referenceZ * tolerance) &&
      (deltaX > deadband || deltaY > deadband || deltaZ > deadband)) {
    status = "Miring";
    digitalWrite(LED_PIN, HIGH);   // LED mati
    digitalWrite(BUZZER_PIN, HIGH);  // Buzzer menyala
  } else {
    status = "Datar";
    digitalWrite(LED_PIN, LOW);  // LED menyala
    digitalWrite(BUZZER_PIN, LOW);  // Buzzer mati
  }

  // Menampilkan data ke Serial Monitor
  Serial.print("AccX: ");
  Serial.print(accelX);
  Serial.print(" | AccY: ");
  Serial.print(accelY);
  Serial.print(" | AccZ: ");
  Serial.print(accelZ);
  Serial.print(" | Status: ");
  Serial.println(status);

  // Mengirim data ke server
  if (WiFi.status() == WL_CONNECTED) {
    String url = serverUrl;
    url += "?sumbu_x=" + String(accelX, 2);
    url += "&sumbu_y=" + String(accelY, 2);
    url += "&sumbu_z=" + String(accelZ, 2);
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

  delay(1000);  
}
