#pragma once

#include <Arduino.h>

enum class SpiDevice : uint8_t {
  None,
  Sd,
  Epaper,
};

void spi_bus_init();
bool spi_bus_acquire(SpiDevice device);
void spi_bus_release(SpiDevice device);
SpiDevice spi_bus_owner();
void spi_bus_quiesce();
