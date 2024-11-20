#include <Wire.h>
#include <MPU6050_light.h>

MPU6050 mpu(Wire);
const int ldrPin = 34;  // Pin analog untuk LDR (ADC1_0 pada ESP32)

void setup() {
    Serial.begin(115200);           // Mengatur komunikasi serial
    Wire.begin();                   // Inisialisasi I2C
    
    // Inisialisasi MPU6050
    Serial.println("Inisialisasi MPU6050...");
    byte status = mpu.begin();
    if (status != 0) {
        Serial.print("MPU6050 gagal terhubung, kode error: ");
        Serial.println(status);
        while (1); // Berhenti jika gagal
    } else {
        Serial.println("MPU6050 terhubung.");
    }

    mpu.calcOffsets(); // Menghitung offset untuk kalibrasi

    delay(1000);
}

void loop() {
    // Membaca data dari sensor LDR
    int ldrValue = analogRead(ldrPin);

    // Mengklasifikasikan intensitas cahaya dari nilai LDR
    if (ldrValue > 2000) {
        Serial.println("Keadaan: Gelap");
    } else if (ldrValue > 500 && ldrValue <= 2000) {
        Serial.println("Keadaan: Redup");
    } else if (ldrValue > 50 && ldrValue <= 500) {
        Serial.println("Keadaan: Cahaya Terang");
    } else {
        Serial.println("Keadaan: Sangat Terang");
    }


    // Memperbarui pembacaan dari sensor MPU6050
    mpu.update();

    // Mengklasifikasikan kondisi kemiringan sensor berdasarkan nilai sumbu X dan Y
    if (mpu.getAngleX() > 10) {
        Serial.println("Sensor miring: Kiri");
    } else if (mpu.getAngleX() < -10) {
        Serial.println("Sensor miring: Kanan");
    }

    if (mpu.getAngleY() > 10) {
        Serial.println("Sensor miring: Depan");
    } else if (mpu.getAngleY() < -10) {
        Serial.println("Sensor miring: Belakang");
    }

    // Menampilkan nilai mentah dari LDR dan kemiringan untuk debugging
    Serial.print("Nilai LDR: ");
    Serial.println(ldrValue);

    Serial.print("Kemiringan X: ");
    Serial.print(mpu.getAngleX());
    Serial.print(" \tKemiringan Y: ");
    Serial.print(mpu.getAngleY());
    Serial.print(" \tKemiringan Z: ");
    Serial.println(mpu.getAngleZ());

    delay(1000); // Delay 1 detik untuk memperlambat pembacaan
}
