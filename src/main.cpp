/*
 * Projeto desenvolvido por Nícolas Ribeiro L. S. Domiciano
 * © 2026 - Todos os direitos reservados.
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

/* === Configurações === */
#include "config/WifiConfig.h"
#define RXD2 34
#define TXD2 26
#define DE_RE 25

/* === Módulos === */
#include "network/MyWifi.h"
#include "sensors/FluxoSensor.h"
#include "sensors/TempSensor.h"
#include "inversor/InverterIPX075.h"

/* === Objetos === */
MyWiFi wifi;
FluxoSensor fluxoSensor;
TempSensor tempSensor;
InverterIPX075 inverter(Serial2, 8, RXD2, TXD2, DE_RE);

/* === Web Server === */
WebServer server(80);

/* === Controle de tempo === */
unsigned long lastSensorRead = 0;
const unsigned long sensorInterval = 1000;

void setupRoutes()
{
    server.on("/", HTTP_GET, []()
              {
    File file = LittleFS.open("/index.html", "r");
    if (!file) {
        server.send(500, "text/plain", "index.html não encontrado");
        return;
    }
    server.streamFile(file, "text/html");
    file.close(); });

    server.on("/telemetry", HTTP_GET, []()
              {
        StaticJsonDocument<256> doc;
        doc["fluxo"] = fluxoSensor.getFluxo();
        doc["temperatura"] = tempSensor.getTemperatura();
        doc["frequencia"] = inverter.getFrequencia();
        doc["status"] = inverter.isRunning() ? "RUN" : "STOP";
        String json;
        serializeJson(doc, json);
        server.send(200, "application/json", json); });

    server.on("/run", HTTP_POST, []()
              {
        inverter.run();
        server.send(200, "text/plain", "RUN"); });

    server.on("/stop", HTTP_POST, []()
              {
        inverter.stop();
        server.send(200, "text/plain", "STOP"); });

    server.on("/setFreq", HTTP_POST, []()
              {
        if (!server.hasArg("value")) {
            server.send(400, "text/plain", "Missing value");
            return;
        }
        float freq = server.arg("value").toFloat();
        inverter.setFrequencia(freq);
        server.send(200, "text/plain", "OK"); });
        
    server.on("/reverse", HTTP_POST, []()
              {
    inverter.runReverse();
    server.send(200, "text/plain", "REVERSE"); });
}

/* ========================================================= */

void setup()
{
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n=== ESP32 Stockflow ===");
    /* WiFi */
    wifi.begin();
    /* Sensores */
    fluxoSensor.begin(27);
    tempSensor.begin(4);
    /* Inversor */
    inverter.begin();
    /* Web Server */

    if (!LittleFS.begin())
    {
        Serial.println("❌ Falha ao montar LittleFS");
        return;
    }
    Serial.println("✅ LittleFS montado com sucesso");
    setupRoutes();
    server.begin();
    Serial.println("WebServer iniciado");
    Serial.println("Sistema pronto!");
}

/* ========================================================= */

void loop()
{
    server.handleClient();
    if (millis() - lastSensorRead >= sensorInterval)
    {
        lastSensorRead = millis();

        fluxoSensor.update();
        tempSensor.update();
        // inverter.update(); // use quando quiser telemetria ativa

        Serial.print("Fluxo: ");
        Serial.print(fluxoSensor.getFluxo());
        Serial.print(" L/min | ");

        Serial.print("Temp: ");
        Serial.print(tempSensor.getTemperatura());
        Serial.print(" °C | ");

        Serial.print("Freq Inversor: ");
        Serial.print(inverter.getFrequencia());
        Serial.print(" Hz | ");

        Serial.print("Status: ");
        Serial.println(inverter.isRunning() ? "RUN" : "STOP");
    }
}

/*
 * Projeto desenvolvido por Nícolas Ribeiro L. S. Domiciano
 * © 2026 - Todos os direitos reservados.
 */