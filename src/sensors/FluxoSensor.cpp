#include "FluxoSensor.h"

static FluxoSensor* instance = nullptr;

void IRAM_ATTR fluxoISR() {
    if (instance) instance->pulseISR();
}

void FluxoSensor::begin(int pin) {
    _pin = pin;
    pinMode(_pin, INPUT_PULLUP);
    _pulsos = 0;
    _lastRead = millis();

    instance = this;
    attachInterrupt(digitalPinToInterrupt(_pin), fluxoISR, RISING);
}

void FluxoSensor::pulseISR() {
    _pulsos++;
}

void FluxoSensor::update() {
    unsigned long now = millis();

    if (now - _lastRead >= 1000) {
        // YF-S201 → 450 pulsos por litro
        _fluxo = (_pulsos / 450.0) * 60.0;
        _pulsos = 0;
        _lastRead = now;
    }
}

float FluxoSensor::getFluxo() {
    return _fluxo;
}

/*
 * Projeto desenvolvido por Nícolas Ribeiro L. S. Domiciano
 * © 2026 - Todos os direitos reservados.
 */