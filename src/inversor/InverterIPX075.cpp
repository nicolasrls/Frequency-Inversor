#include "InverterIPX075.h"

InverterIPX075* InverterIPX075::instance = nullptr;

/* ===== RS485 CALLBACKS ===== */
void InverterIPX075::preTransmission() {
    if (instance) {
        digitalWrite(instance->_pinDE_RE, HIGH);
    }
}

void InverterIPX075::postTransmission() {
    if (instance) {
        digitalWrite(instance->_pinDE_RE, LOW);
    }
}

/* ===== CONSTRUTOR ===== */
InverterIPX075::InverterIPX075(
    HardwareSerial& serial,
    uint8_t slaveId,
    int pinRX,
    int pinTX,
    int pinDE_RE
) : _serial(serial),
    _slaveId(slaveId),
    _pinRX(pinRX),
    _pinTX(pinTX),
    _pinDE_RE(pinDE_RE)
{
    instance = this;
}

/* ===== INIT ===== */
void InverterIPX075::begin(uint32_t baudrate) {
    pinMode(_pinDE_RE, OUTPUT);
    digitalWrite(_pinDE_RE, LOW);

    _serial.begin(baudrate, SERIAL_8N1, _pinRX, _pinTX);

    _node.begin(_slaveId, _serial);
    _node.preTransmission(preTransmission);
    _node.postTransmission(postTransmission);

    Serial.println("[INV] IPX075 iniciado");
}

/* ===== COMANDOS ===== */
void InverterIPX075::run() {
    uint8_t res = _node.writeSingleRegister(0x9CA7, 1);
    if (res == _node.ku8MBSuccess) {
        _running = true;
        Serial.println("[INV] RUN OK");
    } else {
        Serial.println("[INV] RUN ERRO");
    }
}

void InverterIPX075::stop() {
    uint8_t res = _node.writeSingleRegister(0x9CA7, 0);
    if (res == _node.ku8MBSuccess) {
        _running = false;
        Serial.println("[INV] STOP OK");
    } else {
        Serial.println("[INV] STOP ERRO");
    }
}

void InverterIPX075::setFrequencia(float hz) {
    uint16_t valor = hz * 100.0; // 10.0 Hz = 100

    uint8_t res = _node.writeSingleRegister(0x9CA6, valor);
    if (res == _node.ku8MBSuccess) {
        _frequencia = hz;
        Serial.print("[INV] Frequencia: ");
        Serial.println(hz);
    } else {
        Serial.println("[INV] ERRO FREQ");
    }
}

/* ===== GETTERS ===== */
bool InverterIPX075::isRunning() const {
    return _running;
}

float InverterIPX075::getFrequencia() const {
    return _frequencia;
}

void InverterIPX075::runReverse() {
    uint8_t res = _node.writeSingleRegister(0x9CA7, 3);
    if (res == _node.ku8MBSuccess) {
        _running = true;
    }
}

/*
 * Projeto desenvolvido por Nícolas Ribeiro L. S. Domiciano
 * © 2026 - Todos os direitos reservados.
 */