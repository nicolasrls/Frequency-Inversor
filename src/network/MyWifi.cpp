#include "MyWifi.h"
#include <WiFi.h>
#include "config/WifiConfig.h"

void MyWiFi::begin() {
  Serial.println("📡 Iniciando WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  unsigned long start = millis();

  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - start > WIFI_TIMEOUT_MS) {
      Serial.println("\n❌ Falha ao conectar ao WiFi");
      return;
    }
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✅ WiFi conectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

bool MyWiFi::isConnected() {
  return WiFi.status() == WL_CONNECTED;
}

/*
 * Projeto desenvolvido por Nícolas Ribeiro L. S. Domiciano
 * © 2026 - Todos os direitos reservados.
 */