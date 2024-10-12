// Pin untuk Modul LDR dan LED
const int ldrPin = 34;   // Pin GPIO 34 untuk Modul LDR (input analog)
const int ledPin = 5;    // Pin PWM untuk LED

// Variabel untuk menyimpan nilai pembacaan LDR dan kecerahan LED
int ldrValue = 0;
int ledBrightness = 0;

// Threshold untuk mendeteksi kondisi cerah, redup, atau gelap
const int thresholdCerah = 0;     // Kondisi cerah (nilai sangat kecil)
const int thresholdRedup = 2500;  // Kondisi redup
const int thresholdGelap = 4000;  // Kondisi gelap

void setup() {
  Serial.begin(9600);  // Memulai komunikasi serial

  // Set pin LED sebagai output
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Membaca nilai LDR dari modul pada pin GPIO 34
  ldrValue = analogRead(ldrPin);

  // Cetak nilai LDR untuk debugging
  Serial.print("Nilai LDR: ");
  Serial.println(ldrValue);

  // Tentukan apakah ruangan sedang cerah, redup, atau gelap berdasarkan nilai LDR
  if (ldrValue > thresholdCerah && ldrValue < thresholdRedup) {
    Serial.println("Sedang cerah.");
    ledBrightness = 0;  // LED mati atau sangat redup
  } else if (ldrValue > thresholdRedup && ldrValue < thresholdGelap) {
    Serial.println("Sedang redup.");
    ledBrightness = 128;  // LED setengah terang
  } else if (ldrValue > thresholdGelap) {
    Serial.println("Sedang gelap.");
    ledBrightness = 255;  // LED sepenuhnya terang
  } else {
    Serial.println("Nilai tidak terdefinisi.");  // Jika nilainya lebih dari thresholdGelap
  }

  // Atur kecerahan LED dengan nilai PWM
  analogWrite(ledPin, ledBrightness);


}
