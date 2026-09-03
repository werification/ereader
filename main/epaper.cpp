#include "epaper.h"

#include <cstring>

#include <GxEPD2_3C.h>
#include <SPI.h>
#include <epd3c/GxEPD2_290_C90c.h>
#include <lvgl.h>
#include <esp32-hal.h>

#include "config.h"
#include "spi_bus.h"

namespace {
// LVGL logical resolution: landscape 296x128
constexpr int32_t EPD_HOR_RES = 296;
constexpr int32_t EPD_VER_RES = 128;
// GxEPD2 native resolution: portrait 128x296 (WIDTH=128, HEIGHT=296)
constexpr int32_t EPD_PANEL_W = 128;  // physical columns
constexpr int32_t EPD_PANEL_H = 296;  // physical rows
constexpr size_t LANDSCAPE_BYTES = (static_cast<size_t>(EPD_HOR_RES) * static_cast<size_t>(EPD_VER_RES) + 7U) / 8U;
constexpr size_t PORTRAIT_BYTES  = (static_cast<size_t>(EPD_PANEL_W) * static_cast<size_t>(EPD_PANEL_H) + 7U) / 8U;

using DisplayType = GxEPD2_3C<GxEPD2_290_C90c, GxEPD2_290_C90c::HEIGHT>;
DisplayType display(GxEPD2_290_C90c(PIN_EPD_CS, PIN_EPD_DC, PIN_EPD_RES, PIN_EPD_BUSY));

lv_disp_draw_buf_t draw_buf;
lv_disp_drv_t disp_drv;
lv_color_t lvgl_buffer[static_cast<size_t>(EPD_HOR_RES) * static_cast<size_t>(EPD_VER_RES)];

// Landscape framebuffers written by flush_to_display (LVGL coordinate space)
uint8_t black_landscape[LANDSCAPE_BYTES];
uint8_t red_landscape[LANDSCAPE_BYTES];

// Portrait framebuffers sent to GxEPD2 (panel coordinate space)
uint8_t black_portrait[PORTRAIT_BYTES];
uint8_t red_portrait[PORTRAIT_BYTES];

bool initialized = false;

void release_spi_bus() {
  spi_bus_release(SpiDevice::Epaper);
}

// Set a pixel in a landscape-layout (EPD_HOR_RES wide) 1bpp buffer.
// Bit order: MSB of each byte is the leftmost pixel.
void set_landscape_pixel(uint8_t *buf, int32_t x, int32_t y, bool on) {
  if (x < 0 || x >= EPD_HOR_RES || y < 0 || y >= EPD_VER_RES) return;
  const size_t idx  = static_cast<size_t>(y) * static_cast<size_t>(EPD_HOR_RES) + static_cast<size_t>(x);
  const size_t byte = idx >> 3;
  const uint8_t bit = static_cast<uint8_t>(0x80U >> (idx & 7U));
  if (on) buf[byte] |= bit; else buf[byte] &= static_cast<uint8_t>(~bit);
}

// Set a pixel in a portrait-layout (EPD_PANEL_W wide) 1bpp buffer.
void set_portrait_pixel(uint8_t *buf, int32_t x, int32_t y, bool on) {
  if (x < 0 || x >= EPD_PANEL_W || y < 0 || y >= EPD_PANEL_H) return;
  const size_t idx  = static_cast<size_t>(y) * static_cast<size_t>(EPD_PANEL_W) + static_cast<size_t>(x);
  const size_t byte = idx >> 3;
  const uint8_t bit = static_cast<uint8_t>(0x80U >> (idx & 7U));
  if (on) buf[byte] |= bit; else buf[byte] &= static_cast<uint8_t>(~bit);
}

bool is_red_pixel(lv_color_t color) {
  const uint32_t rgb = lv_color_to32(color);
  const uint8_t r = static_cast<uint8_t>((rgb >> 16) & 0xFFU);
  const uint8_t g = static_cast<uint8_t>((rgb >>  8) & 0xFFU);
  const uint8_t b = static_cast<uint8_t>( rgb        & 0xFFU);
  return r > 160U && g < 110U && b < 110U;
}

// Transpose landscape (296x128) framebuffers into portrait (128x296) buffers.
// Rotation: 90° CCW — landscape pixel (lx, ly) maps to portrait pixel (ly, EPD_HOR_RES-1-lx).
void build_portrait_buffers() {
  std::memset(black_portrait, 0xFFU, sizeof(black_portrait));
  std::memset(red_portrait,   0xFFU, sizeof(red_portrait));

  for (int32_t ly = 0; ly < EPD_VER_RES; ++ly) {
    for (int32_t lx = 0; lx < EPD_HOR_RES; ++lx) {
      const size_t idx  = static_cast<size_t>(ly) * static_cast<size_t>(EPD_HOR_RES) + static_cast<size_t>(lx);
      const size_t byte = idx >> 3;
      const uint8_t bit = static_cast<uint8_t>(0x80U >> (idx & 7U));

      const bool blk = !(black_landscape[byte] & bit);  // 0-bit = black pixel
      const bool red = !(red_landscape[byte]   & bit);  // 0-bit = red pixel

      // 90° CCW: portrait x = ly, portrait y = (EPD_HOR_RES - 1 - lx)
      const int32_t px = ly;
      const int32_t py = EPD_HOR_RES - 1 - lx;

      set_portrait_pixel(black_portrait, px, py, blk);
      set_portrait_pixel(red_portrait,   px, py, red);
    }
  }
}

void flush_to_display(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_p) {
  const int32_t w = area->x2 - area->x1 + 1;
  const int32_t h = area->y2 - area->y1 + 1;

  for (int32_t y = 0; y < h; ++y) {
    for (int32_t x = 0; x < w; ++x) {
      const lv_color_t color = color_p[static_cast<size_t>(y) * static_cast<size_t>(w) + static_cast<size_t>(x)];
      const int32_t px = area->x1 + x;
      const int32_t py = area->y1 + y;
      const bool red   = is_red_pixel(color);
      const bool black = !red && lv_color_brightness(color) < 128U;
      // Store inverted (GxEPD2 convention: 0 = ink, 1 = white)
      set_landscape_pixel(black_landscape, px, py, !black);
      set_landscape_pixel(red_landscape,   px, py, !red);
    }
  }

  lv_disp_flush_ready(drv);
}
}  // namespace

void epaper_init() {
  if (initialized) {
    return;
  }

  std::memset(black_landscape, 0xFFU, sizeof(black_landscape));
  std::memset(red_landscape,   0xFFU, sizeof(red_landscape));
  if (!spi_bus_acquire(SpiDevice::Epaper)) {
    DBG_PRINTLN("E-paper SPI acquire failed");
    return;
  }
  display.init(0, false, 2, false, SPI, SPISettings(4000000U, MSBFIRST, SPI_MODE0));
  DBG_PRINTLN("E-paper initialized on shared SPI bus");
  DBG_PRINTLN("Both SPI chip selects deasserted after e-paper init");
  display.setFullWindow();
  // clearScreen() writes and refreshes the panel once; do not issue a second
  // identical refresh during boot.
  display.clearScreen();
  display.hibernate();
  release_spi_bus();
  delay(100);

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
  if (!spi_bus_acquire(SpiDevice::Epaper)) {
    return;
  }
  display.hibernate();
  release_spi_bus();
}

void epaper_refresh() {
  if (!spi_bus_acquire(SpiDevice::Epaper)) {
    DBG_PRINTLN("E-paper refresh SPI acquire failed");
    return;
  }
  build_portrait_buffers();
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.drawBitmap(0, 0, black_portrait, EPD_PANEL_W, EPD_PANEL_H, GxEPD_BLACK);
    display.drawBitmap(0, 0, red_portrait,   EPD_PANEL_W, EPD_PANEL_H, GxEPD_RED);
  } while (display.nextPage());
  display.hibernate();
  release_spi_bus();
}

void epaper_flush_example() {
  if (!spi_bus_acquire(SpiDevice::Epaper)) {
    return;
  }
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
  } while (display.nextPage());
  display.hibernate();
  release_spi_bus();
}

void epaper_show_test_pattern() {
  if (!spi_bus_acquire(SpiDevice::Epaper)) {
    return;
  }
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
  display.hibernate();
  release_spi_bus();
}
