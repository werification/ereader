#include "bongo_cat.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#include "bongo_cat_frames.h"
#include "config.h"

#ifndef SSD1306_WHITE
#error "Adafruit SSD1306 library is required for Bongo Cat OLED"
#endif

namespace {
constexpr const char BONGO_CAT_UPSTREAM_COMMIT[] =
    "97ab931ec523e5d270445f355bd18c9a915d0856";
}

namespace {
constexpr int OLED_WIDTH = 128;
constexpr int OLED_HEIGHT = 64;
constexpr uint8_t OLED_ADDRESS = 0x3C;

Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);
bool display_ready = false;
bool showing_left = false;

bool is_button_press(ButtonEvent event) {
  return event == ButtonEvent::RedPressed ||
         event == ButtonEvent::YellowPressed ||
         event == ButtonEvent::BluePressed ||
         event == ButtonEvent::GreenPressed;
}

void draw_frame(const unsigned char *frame) {
  display.clearDisplay();
  display.drawBitmap(0, 0, frame, FRAME_WIDTH, FRAME_HEIGHT, SSD1306_WHITE);
  display.display();
}
}

void bongo_cat_init() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    DBG_PRINTLN("Bongo Cat OLED: initialization failed");
    return;
  }

  display_ready = true;
  draw_frame(default_frame);
  DBG_PRINTLN("Bongo Cat OLED: ready at 0x3C");
}

void bongo_cat_press(ButtonEvent event) {
  if (!display_ready || !is_button_press(event)) {
    return;
  }

  showing_left = !showing_left;
  draw_frame(showing_left ? left_frame : right_frame);
}
