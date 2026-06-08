#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

#include <OneWire.h>
#include <DallasTemperature.h>

class TempSensor {
  public:
    TempSensor();

    void begin(int pin);
    void update();

    float getTemperatura() const;

  private:
    int _pin;
    float _temperatura = 0.0;

    OneWire* _oneWire = nullptr;
    DallasTemperature* _dallas = nullptr;
};

#endif

/*
 * Projeto desenvolvido por Nícolas Ribeiro L. S. Domiciano
 * © 2026 - Todos os direitos reservados.
 */
