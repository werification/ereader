#include "rtc.h"

#include <Wire.h>

#include "config.h"

namespace {
RTC_DS3231 rtc;
bool rtc_ready = false;
}

void rtc_init() {
  Wire.begin(PIN_RTC_SDA, PIN_RTC_SCL);
  rtc_ready = rtc.begin();
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
