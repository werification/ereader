#include <Arduino.h>
#include <SPI.h>
#include <cstdio>
#include <lvgl.h>
#include <esp_system.h>

#include "battery.h"
#include "bongo_cat.h"
#include "buttons.h"
#include "config.h"
#include "epaper.h"
#include "rtc.h"
#include "sdcard.h"
#include "spi_bus.h"
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
bool storage_ready = false;

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

bool updateBatteryDisplay() {
  const int screen = currentScreenToken();
  const uint32_t now_ms = millis();
  const bool screen_changed = screen != last_battery_screen;
  const bool poll_due = screen_changed || (now_ms - last_battery_poll_ms) >= 30000U;
  const bool log_due = screen_changed || (now_ms - last_battery_log_ms) >= 30000U;

  if (!poll_due && last_battery_percent != 255) {
    return false;
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
    DBG_PRINTLN(log_line);
  }

  if (!screen_changed && percent == last_battery_percent) {
    return false;
  }

  char battery_text[8];
  snprintf(battery_text, sizeof(battery_text), "%u%%", static_cast<unsigned>(percent));
  reader_update_battery_label(battery_text);
  last_battery_percent = percent;
  return true;
}

bool updateClockDisplay() {
  const int screen = currentScreenToken();
  const uint32_t now_ms = millis();
  const bool screen_changed = screen != last_clock_screen;
  const bool enough_time_passed = (now_ms - last_clock_poll_ms) >= 300000U;

  if (!screen_changed && !enough_time_passed) {
    return false;
  }

  last_clock_screen = screen;
  last_clock_poll_ms = now_ms;

  DateTime now;
  if (!rtc_read_now(now)) {
    return false;
  }

  if (!screen_changed && now.minute() == last_clock_minute) {
    return false;
  }

  char time_text[16];
  char date_text[24];
  snprintf(time_text, sizeof(time_text), "%02u:%02u", static_cast<unsigned>(now.hour()), static_cast<unsigned>(now.minute()));
  snprintf(date_text, sizeof(date_text), "%02u/%02u/%04u", static_cast<unsigned>(now.day()), static_cast<unsigned>(now.month()), static_cast<unsigned>(now.year()));
  reader_update_clock_labels(time_text, date_text);
  last_clock_minute = now.minute();
  return true;
}

bool handleButtonEvent(ButtonEvent event) {
  bongo_cat_press(event);

  switch (event) {
    case ButtonEvent::RedPressed:
      DBG_PRINTLN("button: red");
      reader_show_main_menu();
      return true;

    case ButtonEvent::GreenPressed:
      DBG_PRINTLN("button: green");
      if (reader_is_on_main_menu()) {
        reader_show_book_selection_screen();
        return true;
      }
      if (reader_is_on_book_selection()) {
        if (!storage_ready) {
          storage_ready = sdcard_recover();
          if (!storage_ready) {
            reader_show_storage_error();
            return false;
          }
          sdcard_scan_books();
          reader_refresh_book_selection();
        }
        reader_retry_bookmark_save();
        const bool opened = reader_open_selected_book();
        DBG_PRINT("reader open selected result=");
        DBG_PRINTLN(opened ? 1 : 0);
        if (!sdcard_is_mounted()) {
          storage_ready = false;
          reader_show_storage_error();
          return true;
        }
        return opened;
      }
      if (reader_is_on_reading_screen()) {
        reader_show_book_selection_screen();
        return true;
      }
      return false;

    case ButtonEvent::YellowPressed:
      DBG_PRINTLN("button: yellow");
      if (reader_is_on_reading_screen()) {
        if (!storage_ready) {
          return false;
        }
        const bool moved = reader_prev_page();
        if (!sdcard_is_mounted()) {
          storage_ready = false;
          reader_show_storage_error();
          return true;
        }
        return moved;
      }
      if (reader_is_on_book_selection()) {
        return reader_select_prev_book();
      }
      return false;

    case ButtonEvent::BluePressed:
      DBG_PRINTLN("button: blue");
      if (reader_is_on_reading_screen()) {
        if (!storage_ready) {
          return false;
        }
        const bool moved = reader_next_page();
        if (!sdcard_is_mounted()) {
          storage_ready = false;
          reader_show_storage_error();
          return true;
        }
        return moved;
      }
      if (reader_is_on_book_selection()) {
        return reader_select_next_book();
      }
      return false;

    case ButtonEvent::None:
      return false;
  }
  return false;
}

void refreshEpaperWithDiagnostics() {
  const uint32_t refresh_start = millis();
  DBG_PRINT("EPD_REFRESH_BEGIN ms=");
  DBG_PRINTLN(refresh_start);
  epaper_refresh();
  DBG_PRINT("EPD_REFRESH_END ms=");
  DBG_PRINTLN(millis());
  DBG_PRINT("EPD_CS/SD_CS/EPD_BUSY=");
  DBG_PRINT(digitalRead(PIN_EPD_CS));
  DBG_PRINT("/");
  DBG_PRINT(digitalRead(PIN_SD_CS));
  DBG_PRINT("/");
  DBG_PRINTLN(digitalRead(PIN_EPD_BUSY));
}

void serviceLvgl() {
  const uint32_t now = millis();
  if (last_lv_tick == 0) {
    last_lv_tick = now;
  }
  lv_tick_inc(now - last_lv_tick);
  last_lv_tick = now;

  // Run lv_timer_handler() twice: the first call processes pending screen
  // transitions (lv_disp_load_scr schedules async), the second flushes the
  // newly active screen into the framebuffer.
  lv_timer_handler();
  lv_timer_handler();
}
}  // namespace

void setup() {
  Serial.begin(115200);
  DBG_PRINT("RESET_REASON=");
  DBG_PRINTLN(static_cast<int>(esp_reset_reason()));
  DBG_PRINT("BOOT_MILLIS=");
  DBG_PRINTLN(millis());
  // USB CDC on ESP32-S3: do NOT wait for Serial (no while(!Serial)).
  // The DBG macros check if(Serial) before writing, so they're safe
  // whether or not a host is connected.

  spi_bus_init();
  rtc_init();
  bongo_cat_init();
  battery_init();
  buttons_init();

  storage_ready = sdcard_init();
  DBG_PRINT("SD_INIT_RESULT=");
  DBG_PRINTLN(storage_ready ? 1 : 0);
  DBG_PRINT("SD ready: ");
  DBG_PRINTLN(storage_ready ? "yes" : "no");

  if (storage_ready) {
    sdcard_scan_books();
  } else {
    DBG_PRINTLN("SD init failed");
  }

  DBG_PRINTLN("EPD_INIT_BEGIN");
  epaper_init();
  DBG_PRINTLN("EPD_INIT_END");

  ui_init();
  if (storage_ready) {
    reader_show_main_menu();
  } else {
    reader_show_storage_error();
  }

  if (reader_has_selected_book()) {
    reader_select_book(0);
  }

  updateClockDisplay();
  updateBatteryDisplay();
  serviceLvgl();
  refreshEpaperWithDiagnostics();

  // The SD session remains mounted while the display is hibernated. Each
  // display operation claims the shared bus and leaves both CS lines HIGH.
}

void loop() {
  const ButtonEvent event = buttons_poll();
  const bool button_pressed = handleButtonEvent(event);

  const bool clock_changed = updateClockDisplay();

  const bool battery_changed = updateBatteryDisplay();

  if (button_pressed || clock_changed || battery_changed) {
    DBG_PRINT("loop: refresh triggered button=");
    DBG_PRINT(button_pressed ? "1" : "0");
    DBG_PRINT(" clock=");
    DBG_PRINT(clock_changed ? "1" : "0");
    DBG_PRINT(" battery=");
    DBG_PRINTLN(battery_changed ? "1" : "0");
    DBG_PRINT("loop: screen token before serviceLvgl=");
    DBG_PRINTLN(currentScreenToken());
    serviceLvgl();
    DBG_PRINTLN("loop: serviceLvgl done, calling epaper_refresh");
    refreshEpaperWithDiagnostics();
    DBG_PRINTLN("loop: refresh complete");
  }

  delay(5);
}

// Boot flow:
// - Initialize shared SPI bus
// - Mount SD card and scan books
// - Initialize e-paper and LVGL UI
// - Show the main menu or book-selection screen
// - Hibernate the panel after the initial render
