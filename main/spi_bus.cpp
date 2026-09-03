#include "spi_bus.h"

#include <SPI.h>

#include "config.h"

namespace {
SpiDevice owner = SpiDevice::None;
bool initialized = false;

void deassert_chip_selects() {
  pinMode(PIN_EPD_CS, OUTPUT);
  pinMode(PIN_SD_CS, OUTPUT);
  digitalWrite(PIN_EPD_CS, HIGH);
  digitalWrite(PIN_SD_CS, HIGH);
}

const char *device_name(SpiDevice device) {
  switch (device) {
    case SpiDevice::Sd: return "SD";
    case SpiDevice::Epaper: return "EPD";
    case SpiDevice::None: return "none";
  }
  return "?";
}
}

void spi_bus_init() {
  // Deselect both peripherals before starting the shared bus. This prevents
  // either device from sampling the initial SPI clocks during setup.
  deassert_chip_selects();
  if (!initialized) {
    SPI.begin(PIN_SPI_SCK, PIN_SPI_MISO, PIN_SPI_MOSI, PIN_SD_CS);
    initialized = true;
    DBG_PRINTLN("SPI initialized once");
  }
  owner = SpiDevice::None;
  deassert_chip_selects();
  DBG_PRINT("SPI pins SCK/MISO/MOSI=");
  DBG_PRINT(PIN_SPI_SCK);
  DBG_PRINT("/");
  DBG_PRINT(PIN_SPI_MISO);
  DBG_PRINT("/");
  DBG_PRINTLN(PIN_SPI_MOSI);
  DBG_PRINT("SPI CS EPD/SD=");
  DBG_PRINT(digitalRead(PIN_EPD_CS));
  DBG_PRINT("/");
  DBG_PRINTLN(digitalRead(PIN_SD_CS));
}

bool spi_bus_acquire(SpiDevice device) {
  if (device == SpiDevice::None) {
    return false;
  }
  if (owner != SpiDevice::None && owner != device) {
    DBG_PRINT("SPI owner violation current/requested=");
    DBG_PRINT(device_name(owner));
    DBG_PRINT("/");
    DBG_PRINTLN(device_name(device));
    return false;
  }
  deassert_chip_selects();
  owner = device;
  DBG_PRINT("SPI acquire ");
  DBG_PRINTLN(device_name(device));
  return true;
}

void spi_bus_release(SpiDevice device) {
  if (owner != device) {
    DBG_PRINT("SPI release violation owner/requested=");
    DBG_PRINT(device_name(owner));
    DBG_PRINT("/");
    DBG_PRINTLN(device_name(device));
    deassert_chip_selects();
    owner = SpiDevice::None;
    return;
  }
  deassert_chip_selects();
  owner = SpiDevice::None;
  DBG_PRINT("SPI release ");
  DBG_PRINTLN(device_name(device));
}

SpiDevice spi_bus_owner() {
  return owner;
}

void spi_bus_quiesce() {
  deassert_chip_selects();
  owner = SpiDevice::None;
}
