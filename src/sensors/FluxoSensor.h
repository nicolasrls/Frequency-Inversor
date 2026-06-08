#pragma once
#include <Arduino.h>

class FluxoSensor {
public:
    void begin(int pin);
    void update();
    float getFluxo();

    void IRAM_ATTR pulseISR();

private:
    volatile uint32_t _pulsos = 0;
    unsigned long _lastRead = 0;
    float _fluxo = 0.0;
    int _pin;
};

/*
 * Projeto desenvolvido por Nícolas Ribeiro L. S. Domiciano
 * © 2026 - Todos os direitos reservados.
 */