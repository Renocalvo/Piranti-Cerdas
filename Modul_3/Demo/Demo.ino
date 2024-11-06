#include <WiFi.h>

const char* ssid = "monyt";
const char* password = "bismillah";

void setup() {
  Serial.begin(9600);

  WiFi.mode(WIFI_STA); 
  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan ke jaringan: ");
  Serial.println(ssid);

  int timeout = 10000;
  int elapsed = 0;
  while (WiFi.status() != WL_CONNECTED && elapsed < timeout) {
    delay(500);
    Serial.print(".");
    elapsed += 500;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.print("Terhubung ke jaringan: ");
    Serial.println(ssid);
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println();
    Serial.println("Koneksi gagal! Mencari jaringan terbuka...");

    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    delay(1000);

    WiFi.mode(WIFI_STA);
    delay(1000);

     int n = WiFi.scanNetworks();
  Serial.println("Scan done");
  if (n == 0) {
    Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    Serial.println("Nr | SSID | RSSI | CH | Encryption");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.printf("%2d", i + 1);
      Serial.print(" | ");
      Serial.printf("%-32.32s", WiFi.SSID(i).c_str());
      Serial.print(" | ");
      Serial.printf("%4ld", WiFi.RSSI(i));
      Serial.print(" | ");
      Serial.printf("%2ld", WiFi.channel(i));
      Serial.print(" | ");
      switch (WiFi.encryptionType(i)) {
        case WIFI_AUTH_OPEN:            Serial.print("open"); break;
        case WIFI_AUTH_WEP:             Serial.print("WEP"); break;
        case WIFI_AUTH_WPA_PSK:         Serial.print("WPA"); break;
        case WIFI_AUTH_WPA2_PSK:        Serial.print("WPA2"); break;
        case WIFI_AUTH_WPA_WPA2_PSK:    Serial.print("WPA+WPA2"); break;
        case WIFI_AUTH_WPA2_ENTERPRISE: Serial.print("WPA2-EAP"); break;
        case WIFI_AUTH_WPA3_PSK:        Serial.print("WPA3"); break;
        case WIFI_AUTH_WPA2_WPA3_PSK:   Serial.print("WPA2+WPA3"); break;
        case WIFI_AUTH_WAPI_PSK:        Serial.print("WAPI"); break;
        default:                        Serial.print("unknown");
      }
      Serial.println();
      delay(10);
    }
  }
        Serial.println("");
        
    bool connected = false;

    // Try connecting to an open network if available
    for (int i = 0; i < n; i++) {
      if (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) {
        String openSSID = WiFi.SSID(i);
        Serial.print("Mencoba terhubung ke jaringan terbuka: ");
        Serial.println(openSSID);

        WiFi.begin(openSSID.c_str());
        elapsed = 0;

        while (WiFi.status() != WL_CONNECTED && elapsed < timeout) {
          delay(500);
          Serial.print(".");
          elapsed += 500;
        }

        if (WiFi.status() == WL_CONNECTED) {
          Serial.println();
          Serial.print("Terhubung ke jaringan terbuka: ");
          Serial.println(openSSID);
          Serial.print("IP Address: ");
          Serial.println(WiFi.localIP());
          connected = true;
          break;
        }
      }
    }

    if (!connected) {
      Serial.println("Tidak ada jaringan terbuka yang tersedia.");
    }
  }
}

void loop() {
  // The loop function can remain empty for this example.
}