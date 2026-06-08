#ifndef INVERTER_IPX075_H
#define INVERTER_IPX075_H

#include <Arduino.h>
#include <ModbusMaster.h>

class InverterIPX075
{
public:
    InverterIPX075(
        HardwareSerial &serial,
        uint8_t slaveId,
        int pinRX,
        int pinTX,
        int pinDE_RE);

    void begin(uint32_t baudrate = 9600);

    void run();
    void stop();
    void setFrequencia(float hz);
    void runReverse();

    bool isRunning() const;
    float getFrequencia() const;

private:
    HardwareSerial &_serial;
    ModbusMaster _node;

    uint8_t _slaveId;
    int _pinRX;
    int _pinTX;
    int _pinDE_RE;

    bool _running = false;
    float _frequencia = 0.0;

    static InverterIPX075 *instance;

    static void preTransmission();
    static void postTransmission();
};

#endif

/*
 * Projeto desenvolvido por Nícolas Ribeiro L. S. Domiciano
 * © 2026 - Todos os direitos reservados.
 */