#pragma once

#include <Arduino.h>

constexpr uint8_t PIN_BUTTON_RED = 1;
constexpr uint8_t PIN_BUTTON_YELLOW = 41;
constexpr uint8_t PIN_BUTTON_BLUE = 39;
constexpr uint8_t PIN_BUTTON_GREEN = 42;

constexpr uint8_t PIN_EPD_BUSY = 16;
constexpr uint8_t PIN_EPD_RES = 2;
constexpr uint8_t PIN_EPD_DC = 7;
constexpr uint8_t PIN_EPD_CS = 5;
constexpr uint8_t PIN_SPI_MOSI = 6;
constexpr uint8_t PIN_SPI_SCK = 4;
constexpr uint8_t PIN_SPI_MISO = 17;
constexpr uint8_t PIN_SD_CS = 21;

constexpr uint8_t PIN_RTC_SDA = 8;
constexpr uint8_t PIN_RTC_SCL = 15;

constexpr uint8_t PIN_BATTERY_ADC = 3;

// Debug logging — only writes when a USB CDC host is actually connected.
// On ESP32-S3 with USB CDC On Boot enabled, Serial.print() blocks if the
// TX buffer fills with no host consuming it. Guard every log call with this.
#define DBG_PRINT(x)      do { if (Serial) { Serial.print(x);      } } while(0)
#define DBG_PRINTLN(x)    do { if (Serial) { Serial.println(x);    } } while(0)
#define DBG_PRINT2(x, f)  do { if (Serial) { Serial.print(x, f);   } } while(0)
#define DBG_PRINTLN2(x, f) do { if (Serial) { Serial.println(x, f); } } while(0)
