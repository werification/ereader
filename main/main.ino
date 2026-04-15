#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <cstdio>
#include <lvgl.h>

#include "battery.h"
#include "buttons.h"
#include "config.h"
#include "epaper.h"
#include "rtc.h"
#include "sdcard.h"
#include "reader.h"
#include "../UI_Export/ui.h"

namespace {
uint32_t last_lv_tick = 0;
uint32_t last_clock_poll_ms = 0;
uint32_t last_battery_poll_ms = 0;
uint32_t last_battery_log_ms = 0;
int last_clock_minute = -1;
int last_clock_screen = -1;
int last_battery_screen = -1;
uint8_t last_battery_percent = 255;

int currentScreenToken() {
  if (reader_is_on_main_menu()) {
    return 0;
  }
  if (reader_is_on_book_selection()) {
    return 1;
  }
  if (reader_is_on_reading_screen()) {
    return 2;
  }
  return 3;
}

void updateBatteryDisplay() {
  const int screen = currentScreenToken();
  const uint32_t now_ms = millis();
  const bool screen_changed = screen != last_battery_screen;
  const bool poll_due = screen_changed || (now_ms - last_battery_poll_ms) >= 30000U;
  const bool log_due = screen_changed || (now_ms - last_battery_log_ms) >= 30000U;

  if (!poll_due && last_battery_percent != 255) {
    return;
  }

  last_battery_screen = screen;
  last_battery_poll_ms = now_ms;

  const uint16_t battery_mv = battery_voltage_millivolts();
  const uint8_t percent = battery_percentage();

  if (log_due) {
    last_battery_log_ms = now_ms;
    const uint16_t pin_mv = battery_pin_millivolts();
    char log_line[64];
    snprintf(log_line, sizeof(log_line), "Battery: %u%% (%u mV pack, %u mV pin)", static_cast<unsigned>(percent), static_cast<unsigned>(battery_mv), static_cast<unsigned>(pin_mv));
    Serial.println(log_line);
  }

  if (!screen_changed && percent == last_battery_percent) {
    return;
  }

  char battery_text[8];
  snprintf(battery_text, sizeof(battery_text), "%u%%", static_cast<unsigned>(percent));
  reader_update_battery_label(battery_text);
  last_battery_percent = percent;
}

void updateClockDisplay() {
  const int screen = currentScreenToken();
  const uint32_t now_ms = millis();
  const bool screen_changed = screen != last_clock_screen;
  const bool enough_time_passed = (now_ms - last_clock_poll_ms) >= 300000U;

  if (!screen_changed && !enough_time_passed) {
    return;
  }

  last_clock_screen = screen;
  last_clock_poll_ms = now_ms;

  DateTime now;
  if (!rtc_read_now(now)) {
    return;
  }

  if (!screen_changed && now.minute() == last_clock_minute) {
    return;
  }

  char time_text[16];
  char date_text[24];
  snprintf(time_text, sizeof(time_text), "%02u:%02u", static_cast<unsigned>(now.hour()), static_cast<unsigned>(now.minute()));
  snprintf(date_text, sizeof(date_text), "%02u/%02u/%04u", static_cast<unsigned>(now.day()), static_cast<unsigned>(now.month()), static_cast<unsigned>(now.year()));
  reader_update_clock_labels(time_text, date_text);
  last_clock_minute = now.minute();
}

void initSpiBus() {
  SPI.end();
  SPI.begin(PIN_SPI_SCK, PIN_SPI_MISO, PIN_SPI_MOSI, PIN_SD_CS);
  pinMode(PIN_EPD_CS, OUTPUT);
  pinMode(PIN_SD_CS, OUTPUT);
  digitalWrite(PIN_EPD_CS, HIGH);
  digitalWrite(PIN_SD_CS, HIGH);
}

void handleButtonEvent(ButtonEvent event) {
  switch (event) {
    case ButtonEvent::RedPressed:
      reader_show_main_menu();
      break;

    case ButtonEvent::GreenPressed:
      if (reader_is_on_main_menu()) {
        reader_show_book_selection_screen();
      } else if (reader_is_on_book_selection()) {
        reader_open_selected_book();
      } else if (reader_is_on_reading_screen()) {
        reader_show_book_selection_screen();
      }
      break;

    case ButtonEvent::YellowPressed:
      if (reader_is_on_reading_screen()) {
        reader_prev_page();
      } else if (reader_is_on_book_selection()) {
        reader_select_prev_book();
      }
      break;

    case ButtonEvent::BluePressed:
      if (reader_is_on_reading_screen()) {
        reader_next_page();
      } else if (reader_is_on_book_selection()) {
        reader_select_next_book();
      }
      break;

    case ButtonEvent::None:
      break;
  }
}

void serviceLvgl() {
  const uint32_t now = millis();
  if (last_lv_tick == 0) {
    last_lv_tick = now;
  }
  lv_tick_inc(now - last_lv_tick);
  last_lv_tick = now;
  lv_timer_handler();
}
}  // namespace

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("myereader boot");

  initSpiBus();
  rtc_init();
  battery_init();
  buttons_init();

  const bool sd_ready = sdcard_init();
  Serial.print("SD ready: ");
  Serial.println(sd_ready ? "yes" : "no");

  if (sd_ready) {
    sdcard_scan_books();
  } else {
    Serial.println("SD init failed");
  }

  epaper_init();
  ui_init();
  if (sd_ready) {
    reader_show_main_menu();
  } else {
    reader_show_storage_error();
  }

  if (reader_has_selected_book()) {
    reader_select_book(0);
  }

  updateClockDisplay();
  updateBatteryDisplay();
  epaper_hibernate();
}

void loop() {
  handleButtonEvent(buttons_poll());
  updateClockDisplay();
  updateBatteryDisplay();
  serviceLvgl();
  delay(5);
}

// Boot flow:
// - Initialize shared SPI bus
// - Mount SD card and scan books
// - Initialize e-paper and LVGL UI
// - Show the main menu or book-selection screen
// - Hibernate the panel after the initial render
