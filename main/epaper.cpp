#include "epaper.h"

#include <cstring>

#include <GxEPD2_3C.h>
#include <SPI.h>
#include <epd3c/GxEPD2_290_C90c.h>
#include <lvgl.h>

#include "config.h"

namespace {
constexpr int32_t EPD_HOR_RES = 296;
constexpr int32_t EPD_VER_RES = 128;
// The panel is mounted in landscape; LVGL should match the physical 296x128 layout.
constexpr size_t FRAMEBUFFER_BYTES = (static_cast<size_t>(EPD_HOR_RES) * static_cast<size_t>(EPD_VER_RES) + 7U) / 8U;

using DisplayType = GxEPD2_3C<GxEPD2_290_C90c, GxEPD2_290_C90c::HEIGHT>;
DisplayType display(GxEPD2_290_C90c(PIN_EPD_CS, PIN_EPD_DC, PIN_EPD_RES, PIN_EPD_BUSY));

lv_disp_draw_buf_t draw_buf;
lv_disp_drv_t disp_drv;
lv_color_t lvgl_buffer[static_cast<size_t>(EPD_HOR_RES) * static_cast<size_t>(EPD_VER_RES)];
uint8_t black_framebuffer[FRAMEBUFFER_BYTES];
uint8_t red_framebuffer[FRAMEBUFFER_BYTES];
bool initialized = false;

void set_frame_pixel(uint8_t *buffer, int32_t x, int32_t y, bool on) {
  if (x < 0 || x >= EPD_HOR_RES || y < 0 || y >= EPD_VER_RES) {
    return;
  }

  const size_t bit_index = static_cast<size_t>(y) * static_cast<size_t>(EPD_HOR_RES) + static_cast<size_t>(x);
  const size_t byte_index = bit_index >> 3;
  const uint8_t mask = static_cast<uint8_t>(0x80U >> (bit_index & 0x7U));

  if (on) {
    buffer[byte_index] |= mask;
  } else {
    buffer[byte_index] &= static_cast<uint8_t>(~mask);
  }
}

bool is_red_pixel(lv_color_t color) {
  const uint32_t rgb = lv_color_to32(color);
  const uint8_t red = static_cast<uint8_t>((rgb >> 16) & 0xFFU);
  const uint8_t green = static_cast<uint8_t>((rgb >> 8) & 0xFFU);
  const uint8_t blue = static_cast<uint8_t>(rgb & 0xFFU);
  return red > 160U && green < 110U && blue < 110U;
}

void flush_to_display(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_p) {
  const int32_t w = area->x2 - area->x1 + 1;
  const int32_t h = area->y2 - area->y1 + 1;

  for (int32_t y = 0; y < h; ++y) {
    for (int32_t x = 0; x < w; ++x) {
      const size_t src_index = static_cast<size_t>(y) * static_cast<size_t>(w) + static_cast<size_t>(x);
      const lv_color_t color = color_p[src_index];
      const int32_t px = area->x1 + x;
      const int32_t py = area->y1 + y;

      const bool red = is_red_pixel(color);
      const bool black = !red && lv_color_brightness(color) < 128;

      set_frame_pixel(red_framebuffer, px, py, red);
      set_frame_pixel(black_framebuffer, px, py, black);
    }
  }

  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.drawBitmap(0, 0, black_framebuffer, EPD_HOR_RES, EPD_VER_RES, GxEPD_BLACK);
    display.drawBitmap(0, 0, red_framebuffer, EPD_HOR_RES, EPD_VER_RES, GxEPD_RED);
  } while (display.nextPage());

  lv_disp_flush_ready(drv);
}
}  // namespace

void epaper_init() {
  if (initialized) {
    return;
  }

  std::memset(black_framebuffer, 0xFF, sizeof(black_framebuffer));
  std::memset(red_framebuffer, 0xFF, sizeof(red_framebuffer));
  display.init(115200, true, 2, false);
  display.setFullWindow();
  display.clearScreen();
  display.refresh();
  display.hibernate();
  delay(100);
  display.setRotation(1);
  display.setFullWindow();

  lv_init();
  lv_disp_draw_buf_init(&draw_buf, lvgl_buffer, nullptr, static_cast<uint32_t>(sizeof(lvgl_buffer) / sizeof(lvgl_buffer[0])));

  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = EPD_HOR_RES;
  disp_drv.ver_res = EPD_VER_RES;
  disp_drv.flush_cb = flush_to_display;
  disp_drv.draw_buf = &draw_buf;
  disp_drv.full_refresh = 1;

  lv_disp_t *disp = lv_disp_drv_register(&disp_drv);
  lv_disp_set_default(disp);

  initialized = true;
}

void epaper_hibernate() {
  display.hibernate();
}

void epaper_flush_example() {
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
  } while (display.nextPage());
  display.hibernate();
}

void epaper_show_test_pattern() {
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.fillRect(0, 0, EPD_VER_RES, 32, GxEPD_BLACK);
    display.fillRect(0, 40, EPD_VER_RES, 32, GxEPD_RED);

    display.setTextColor(GxEPD_WHITE);
    display.setCursor(8, 22);
    display.setTextSize(2);
    display.print("BLACK");

    display.setTextColor(GxEPD_BLACK);
    display.setCursor(8, 62);
    display.print("RED");
  } while (display.nextPage());
}
