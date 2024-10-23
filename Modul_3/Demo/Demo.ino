#include <WiFi.h>

const char* ssid = "B";
const char* password = "indrabyu21";


void connectToOpenNetwork() {
  Serial.println("Mencari jaringan WiFi terbuka...");
  int n = WiFi.scanNetworks();  

  for (int i = 0; i < n; i++) {
    if (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) {  
      const char* openSSID = WiFi.SSID(i).c_str();
      Serial.print("Menghubungkan ke jaringan terbuka: ");
      Serial.println(openSSID);

      WiFi.begin(openSSID);  
      while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
      }

      Serial.println("\nTerhubung ke WiFi: ");
      Serial.println(openSSID);
      return;  
    }
  }
  Serial.println("Tidak ditemukan jaringan terbuka.");
}

void setup() {
  Serial.begin(9600);  
  WiFi.mode(WIFI_STA); 

  Serial.print("Menghubungkan ke SSID: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);  

  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
    delay(1000);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nTerhubung ke WiFi!");
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
  } else {
    Serial.println("\nGagal terhubung ke jaringan WiFi dengan SSID & password.");
    connectToOpenNetwork();
  }
}

void loop() {
  // Program utama yang berjalan terus-menerus
}
