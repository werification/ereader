#include "battery.h"

#include <driver/adc.h>

#include "config.h"

namespace {
constexpr uint32_t SAMPLE_COUNT = 16U;
constexpr float BATTERY_CALIBRATION_FACTOR = 1.5628f;

uint16_t read_adc_millivolts() {
  uint32_t total = 0;
  for (uint32_t i = 0; i < SAMPLE_COUNT; ++i) {
    total += analogReadMilliVolts(PIN_BATTERY_ADC);
  }
  return static_cast<uint16_t>(total / SAMPLE_COUNT);
}

uint8_t voltage_to_percentage(uint16_t millivolts) {
  const float voltage = static_cast<float>(millivolts) / 1000.0f;
  if (voltage <= 3.20f) {
    return 0;
  }
  if (voltage >= 4.20f) {
    return 100;
  }
  if (voltage <= 3.60f) {
    return static_cast<uint8_t>((voltage - 3.20f) / 0.40f * 20.0f);
  }
  if (voltage <= 3.80f) {
    return static_cast<uint8_t>(20.0f + (voltage - 3.60f) / 0.20f * 20.0f);
  }
  if (voltage <= 3.95f) {
    return static_cast<uint8_t>(40.0f + (voltage - 3.80f) / 0.15f * 20.0f);
  }
  if (voltage <= 4.10f) {
    return static_cast<uint8_t>(60.0f + (voltage - 3.95f) / 0.15f * 25.0f);
  }
  return static_cast<uint8_t>(85.0f + (voltage - 4.10f) / 0.10f * 15.0f);
}
}  // namespace

void battery_init() {
  analogReadResolution(12);
  analogSetPinAttenuation(PIN_BATTERY_ADC, ADC_11db);
}

uint16_t battery_pin_millivolts() {
  return read_adc_millivolts();
}

uint16_t battery_voltage_millivolts() {
  const uint16_t pin_millivolts = battery_pin_millivolts();
  return static_cast<uint16_t>(static_cast<float>(pin_millivolts) * BATTERY_CALIBRATION_FACTOR);
}

uint8_t battery_percentage() {
  return voltage_to_percentage(battery_voltage_millivolts());
}
