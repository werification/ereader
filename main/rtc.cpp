#include "rtc.h"

#include <Wire.h>

#include "config.h"

namespace {
RTC_DS3231 rtc;
bool rtc_ready = false;

void scan_i2c_devices() {
  size_t device_count = 0U;
  DBG_PRINTLN("I2C: scanning addresses 0x01-0x7E");

  for (uint8_t address = 0x01U; address < 0x7FU; ++address) {
    Wire.beginTransmission(address);
    const uint8_t error = Wire.endTransmission();
    if (error != 0U) {
      continue;
    }

    ++device_count;
    DBG_PRINT("I2C: device found at 0x");
    DBG_PRINTLN2(address, HEX);
  }

  if (device_count == 0U) {
    DBG_PRINTLN("I2C: no devices found");
  } else {
    DBG_PRINT("I2C: scan complete, devices found: ");
    DBG_PRINTLN(static_cast<unsigned>(device_count));
  }
}
}

void rtc_init() {
  Wire.begin(PIN_RTC_SDA, PIN_RTC_SCL);
  rtc_ready = rtc.begin();
  DBG_PRINT("RTC: ");
  DBG_PRINTLN(rtc_ready ? "found at 0x68" : "not detected at 0x68");
  scan_i2c_devices();
}

bool rtc_read_now(DateTime &now) {
  if (!rtc_ready) {
    return false;
  }

  now = rtc.now();
  return true;
}

bool rtc_sync_from_build_time() {
  if (!rtc_ready) {
    return false;
  }

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  return true;
}
