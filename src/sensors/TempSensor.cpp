#include "TempSensor.h"

TempSensor::TempSensor() {}

void TempSensor::begin(int pin) {
  _pin = pin;

  _oneWire = new OneWire(_pin);
  _dallas = new DallasTemperature(_oneWire);

  _dallas->begin();
}

void TempSensor::update() {
  if (!_dallas) return;

  _dallas->requestTemperatures();
  _temperatura = _dallas->getTempCByIndex(0);

  // Proteção contra erro do sensor
  if (_temperatura == DEVICE_DISCONNECTED_C) {
    _temperatura = 0.0;
  }
}

float TempSensor::getTemperatura() const {
  return _temperatura;
}

/*
 * Projeto desenvolvido por Nícolas Ribeiro L. S. Domiciano
 * © 2026 - Todos os direitos reservados.
 */