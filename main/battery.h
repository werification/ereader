#pragma once

#include <Arduino.h>

void battery_init();
uint16_t battery_pin_millivolts();
uint16_t battery_voltage_millivolts();
uint8_t battery_percentage();
