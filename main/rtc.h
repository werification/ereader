#pragma once

#include <Arduino.h>
#include <RTClib.h>

void rtc_init();
bool rtc_read_now(DateTime &now);
bool rtc_sync_from_build_time();
