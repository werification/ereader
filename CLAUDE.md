ESP32 E-Reader Project

Project Overview

A Kindle-like e-reader device built on the ESP32-S3 Super Mini microcontroller. The device reads .txt files from a MicroSD card and displays them on a WeAct Studio e-paper display. It includes a DS3231 RTC for timekeeping, four navigation buttons, and battery charging support. The UI is designed using SquareLine Studio (LVGL-based).

Current Project Focus

- Active area: 2.9" RBW e-paper bring-up and SD-card / SPI bus stability.
- Update this section when the current work area changes, so future sessions know where the project is at a glance.
- If debugging, start with the active area above before revisiting older subsystems.

Project Completion Tasks

- Preserve and audit the current source state before making further changes.
- Keep serial diagnostics available without allowing USB CDC to block standalone boot.
- Stabilize shared SPI ownership between the SD card and RBW e-paper display:
  - Keep the inactive device's CS line deasserted.
  - Mount the SD card before communicating with the e-paper display.
  - Explicitly reconfigure SPI between devices when required.
- Make scanned book paths and the SD mount lifecycle reliable.
- Handle the RBW panel's long refresh time without watchdog resets, brownouts, or unsafe display power states.
- Verify button input and screen transitions independently of serial connection.
- Verify text pagination and implement reading-position bookmarks.
- Add safe hibernation and later power-management/deep-sleep behavior.
- Build and test the complete standalone workflow.
- Update this document with only verified hardware behavior and final architecture decisions.

Immediate debugging priority

- Diagnose SD/e-paper SPI stability and reset causes before adding more remount retries.
- Preserve serial output while distinguishing watchdog, panic, brownout, USB, and SPI-related resets.
- Fix page navigation after an e-paper refresh: the current failure occurs during SD remount (`SD_BEGIN_RESULT=0`) before page text or pagination is read.
- Do not treat a successful boot mount as proof that the SD session remains usable after the display changes the shared SPI peripheral state.

Current Verified Runtime State (2026-09-03)

- The authoritative SD chip-select wiring is GPIO21 (`PIN_SD_CS`); GPIO18 is historical documentation and must not be substituted.
- The authoritative shared SPI wiring is SCK GPIO4, MOSI GPIO6, SD MISO GPIO17, SD CS GPIO21, and e-paper CS GPIO5.
- I2C discovery consistently finds the Bongo Cat OLED at `0x3C`, the DS3231 at `0x68`, and another device at `0x57`. Bongo Cat initializes successfully at `0x3C`.
- SD initialization succeeds at boot. The `/books` directory opens, the three current text books are scanned, and the bookmark snapshot loads.
- The e-paper refresh completes and the diagnostic CS/BUSY state is `EPD_CS/SD_CS/EPD_BUSY=1/1/1` after refresh. The display is hibernated after each refresh.
- The current software releases the SD session before display refresh and remounts SD on demand before book scanning/opening or page navigation.
- Book selection/opening can succeed after one or more remount attempts. The log has shown an initial `SD_BEGIN_RESULT=0` followed by `SD_BEGIN_RESULT=1`, after which a book opens and the reading screen appears.
- Page navigation is not currently working reliably. After the reading-screen refresh, Blue/Yellow attempts call the remount path, but all three attempts can return `SD_BEGIN_RESULT=0`; page text is therefore never read and the page index does not change.
- The failure is also observed on the selection screen and after small-book use. It is not currently proven to be caused by the 750 KB book, UTF-8 parsing, page-table growth, battery voltage, GPIO21 selection, or the bookmark format.
- Red can return to the menu, but bookmark writes may fail after SD recovery is lost (`SD: cannot create bookmark directory`).
- Observed e-paper refresh time is approximately 1.7 seconds in the current logs, while the panel documentation says approximately 27 seconds for RBW. Verify the panel/driver configuration separately; do not assume this timing discrepancy is the SD root cause.

Known Unresolved Defect and Required Fix

- The SD card becomes unavailable after one or more e-paper refreshes. `SD.begin()` then fails repeatedly even though both chip-select lines are HIGH and SPI has been restarted.
- The immediate defect is the SD/SPI lifecycle or Arduino SD driver state across repeated `SD.end()`/`SPI.end()`/`SPI.begin()`/`SD.begin()` cycles. It happens before `SD.open()` and before reader pagination, so reader rendering changes alone cannot fix it.
- The current idle-clock retry sequence is insufficient: ten dummy bytes (80 clock cycles), 1 MHz initialization, CS deassertion, and up to three retries did not eliminate the failure.
- The next software investigation must change the lifecycle rather than merely increasing retry counts. Prefer a single stable SD-driver lifecycle with explicit SPI transaction settings around SD access, or replace the repeated Arduino `SD` begin/end recovery with a lower-level/alternate SD implementation if the library cannot safely resume after e-paper use.
- Instrument heap, SPI configuration, SD driver state, page-table count/capacity, file size, and active page around every display-to-SD handoff. Keep the existing `SD_BEGIN_RESULT`, `/books` probe, CS, and reset diagnostics.
- Preserve the tested lazy pagination behavior: calculate the first page before showing a book, extend one page on demand for Next, and retain cached byte offsets for Previous. Do not reintroduce whole-book pagination while fixing SD ownership.
- After the SD lifecycle is fixed, retest small and large books, repeated Blue/Yellow navigation, Red/Green transitions, bookmark save/reload, UTF-8 input, empty files, long lines, and end-of-file behavior.

Important Debugging Boundaries

- Do not change GPIO21 to GPIO18 based on old notes; the user confirmed the physical SD CS is GPIO21.
- Do not change battery calibration or attribute this SD failure to the battery: the failing test was powered from USB 5 V and the failure is at `SD_BEGIN_RESULT=0`.
- Do not claim page parsing or page-table logic is the primary cause when the log says `reader: failed to open ...`; that message means SD access failed before pagination began.
- Do not remove e-paper hibernation or leave either device selected during the handoff. Both CS lines must remain HIGH whenever their device is inactive.


Hardware

MCU: ESP32-S3 Super Mini

- Chip: ESP32-S3 dual-core Xtensa LX7, up to 240 MHz
- Flash: 4 MB (QIO mode)
- SRAM: 512 KB
- Board dimensions: 22.52 × 18 mm
- USB: USB-C (native USB, no external serial chip)
- Connectivity: Wi-Fi 802.11 b/g/n, Bluetooth 5.0 / BLE (not used in this project)
- Arduino board selection: "ESP32S3 Dev Module" from esp32 by Espressif package
- PlatformIO board: esp32-s3-devkitm-1
- Reference: https://www.espboards.dev/esp32/esp32-s3-super-mini/

Safe GPIOs (no boot/flash/USB conflicts)

IO1, IO2, IO4, IO5, IO6, IO7, IO8, IO15, IO16, IO17, IO18, IO21

JTAG-only GPIOs (safe as GPIO when JTAG debugging is not used)

IO39 (MTCK), IO40 (MTDO), IO41 (MTDI) — default JTAG pins. Safe to use as regular GPIO in production builds where JTAG is not needed. This project uses IO39, IO40, and IO41 for buttons.

GPIOs to Avoid or Use with Caution

- IO0: Boot button
- IO3: Strapping pin (JTAG interface select) — currently used for battery charge receiver; be aware of boot-time behavior
- IO9–IO14: Flash/PSRAM pins (FSPIHD, FSPICS0, FSPID, FSPICLK, FSPIQ, FSPIWP) — these are wired to external flash on the Super Mini. Do NOT use as GPIO.
- IO48: Onboard WS2812 RGB LED + Red LED (shared)
- IO45, IO46: Strapping pins (flash voltage, boot mode)

⚠️ IMPORTANT: GPIOs 9–14 are wired to the external flash chip on the ESP32-S3 Super Mini (4MB QIO). They cannot be used as GPIO. All project pin assignments have been moved off these pins.

E-Paper Display: WeAct Studio Epaper Module

- Supported sizes: 1.54", 2.13", 2.9", 3.7", 4.2"
- Interface: SPI
- Driver IC: SSD1680 (for 2.13" and 2.9" variants)
- Variants: Black & White (BW), or Red/Black/White (RBW)
- BW full refresh time: ~3 seconds
- RBW full refresh time: ~27 seconds
- BW partial refresh: Supported (must still do full refresh periodically)
- RBW partial refresh: NOT supported
- Resolution (2.9" panel): native 128 × 296 pixels; this build presents a 296 × 128 landscape LVGL canvas and rotates/transposes it for the panel.
- Reference: https://github.com/WeActStudio/WeActStudio.EpaperModule

E-Paper Wiring

| E-Paper Pin | Function         | ESP32 GPIO |
|-------------|------------------|------------|
| BUSY        | Busy signal      | 16     |
| RES         | Reset            | 2      |
| DC          | Data/Command     | 7      |
| CS          | Chip Select      | 5      |
| SDA (MOSI)  | SPI Data In      | 6      |
| SCL (SCK)   | SPI Clock        | 4      |
| VCC         | Power            | 3.3V       |
| GND         | Ground           | GND        |

E-Paper Critical Notes

1. The display MUST be put into powerOff() or hibernate() mode after every refresh. Leaving it powered on will permanently damage the display — this is not recoverable.
2. Minimum refresh interval: 180 seconds recommended. Refresh at least once every 24 hours.
3. Partial refresh caveat: After several partial refreshes, a full refresh is mandatory. Failure to do so causes irreversible display artifacts.
4. Before long-term storage: Run a full clear/refresh cycle.

DS3231 RTC Module

- Interface: I2C
- Library: RTClib by Adafruit (recommended) or ErriezDS3231
- Features: High-precision real-time clock, battery-backed, temperature-compensated crystal oscillator

DS3231 Wiring

| DS3231 Pin | Function | ESP32 GPIO |
|------------|----------|------------|
| SDA        | I2C Data | 8      |
| SCL        | I2C Clock| 15     |
| VCC        | Power    | 3.3V       |
| GND        | Ground   | GND        |

The DS3231 uses a dedicated I2C bus (GPIO 8/15), separate from the SPI bus used by the e-paper and SD card. Initialize with: Wire.begin(8, 15);
MicroSD Card Module

- Interface: SPI
- Filesystem: FAT32 expected

MicroSD Wiring

| SD Pin | Function    | ESP32 GPIO |
|--------|-------------|------------|
| CS     | Chip Select | 21     |
| MOSI   | Data In     | 6      |
| CLK    | Clock       | 4      |
| MISO   | Data Out    | 17     |
| VCC    | Power       | 3.3V       |
| GND    | Ground      | GND        |

SPI Bus Sharing: E-Paper + MicroSD

The e-paper display and MicroSD card share the same SPI bus (MOSI=GPIO6, SCK=GPIO4) with separate CS lines (e-paper CS=GPIO5, SD CS=GPIO21). The e-paper does not use MISO (it's a write-only device); the SD card uses MISO=GPIO17.

SPI Bus Initialization Order (Critical)

Per Espressif documentation, when sharing an SPI bus with an SD card:

1. Initialize the SPI bus.
2. Pull all non-SD CS lines HIGH (set GPIO5 HIGH to deselect e-paper).
3. Mount the SD card FIRST — this puts the SD card into SPI mode. If you communicate with other SPI devices before the SD card is initialized, the SD card may interpret those signals and enter an undefined state.
4. After SD card is mounted, you can freely communicate with the e-paper display.

This build passes the shared SPI object and settings explicitly to GxEPD2, while SD uses the same bus with its own CS. Keep both CS lines HIGH whenever their device is inactive. If the bus must be reconfigured, use the actual wiring:
```cpp
SPI.end();
SPI.begin(4, 17, 6, 21);  // SCK, MISO, MOSI, SD CS
```
⚠️ Always deassert (pull HIGH) the CS of the device you're NOT talking to. SPI bus contention between the SD card and e-paper will cause data corruption and potentially hang the bus.
```

Buttons

| Button | Color  | ESP32 GPIO | Notes                    |
|--------|--------|------------|--------------------------|
| RED    | Red    | 1      | Safe GPIO                |
| YELLOW | Yellow | 41     | JTAG MTDI — safe, JTAG not used |
| BLUE   | Blue   | 39     | JTAG TCK — safe, JTAG not used |
| GREEN  | Green  | 42     | Internal pull-up; active LOW |

Buttons connect GPIO to GND when pressed (active LOW). All four buttons use internal pull-ups:
```cpp
pinMode(1, INPUT_PULLUP);   // RED
pinMode(41, INPUT_PULLUP);  // YELLOW
pinMode(39, INPUT_PULLUP);  // BLUE
pinMode(42, INPUT_PULLUP);  // GREEN
```
Debounce all button inputs (software debounce ~50ms or use a library).

Battery Monitoring and Charging

- The firmware currently measures the battery-divider ADC on GPIO3 (`PIN_BATTERY_ADC`), although older documentation describes GPIO3 as a charge-detection input. Confirm the physical wiring before changing the calibration.
- Battery measurement uses 12-bit ADC resolution, 11 dB attenuation, and an average of 16 `analogReadMilliVolts()` samples.
- The current firmware calibration is `BATTERY_CALIBRATION_FACTOR = 1.5628`, so it assumes: battery voltage = ADC-pin voltage × 1.5628.
- Verified multimeter reading: approximately 0.46 V at the ADC/GPIO3 point and 3.84 V directly across the battery. These readings imply a factor of approximately 8.35 (`3.84 / 0.46`) and do not match the current firmware calibration.
- Do not update the calibration factor based on this single reading yet. First confirm that the measured point is the actual ESP32 GPIO3 ADC input, that battery voltage was measured across battery positive and negative, and that the divider wiring/ground are correct. Take a second pair of readings at a different battery voltage to confirm the ratio.
- GPIO3 is a strapping pin sampled at reset. Ensure the battery-divider/charge circuit does not force it to an unsafe level during boot or alter the JTAG/debug interface selection.

Pin Assignment Summary

| GPIO | Assignment              | Bus/Protocol | Notes                          |
|------|-------------------------|--------------|--------------------------------|
| 1    | RED button              | Digital In   | Safe GPIO, use INPUT_PULLUP    |
| 2    | E-paper RES (Reset)     | Digital Out  | Safe GPIO                      |
| 3    | Battery charge receiver | Analog/Digital| ⚠️ Strapping pin              |
| 4    | E-paper SCL + SD CLK    | SPI SCK      | Shared SPI clock               |
| 5    | E-paper CS              | SPI CS       | Safe GPIO                      |
| 6    | E-paper SDA + SD MOSI   | SPI MOSI     | Shared SPI data out            |
| 7    | E-paper DC              | Digital Out  | Safe GPIO                      |
| 8    | DS3231 SDA              | I2C Data     | Safe GPIO                      |
| 15   | DS3231 SCL              | I2C Clock    | Safe GPIO (moved from IO9)     |
| 16   | E-paper BUSY            | Digital In   | Safe GPIO                      |
| 17   | SD MISO                 | SPI MISO     | Safe GPIO                      |
| 41   | YELLOW button           | Digital In   | JTAG MTDI — safe, JTAG not used |
| 21   | SD CS                   | SPI CS       | Safe GPIO                      |
| 39   | BLUE button             | Digital In   | JTAG TCK — safe, JTAG not used |
| 42   | GREEN button            | Digital In   | Safe GPIO; use INPUT_PULLUP       |

Software Stack

Framework

- Arduino framework on ESP32-S3 (esp32 board package by Espressif, v3.x+)
- Board selection: ESP32S3 Dev Module
- Or PlatformIO with board = esp32-s3-devkitm-1
UI: SquareLine Studio + LVGL

- UI layouts and screens are designed in SquareLine Studio and exported as LVGL C code.
- LVGL drives the e-paper display through a custom flush callback that bridges LVGL's framebuffer to the e-paper driver (GxEPD2 or direct SSD1680 commands).
- Color depth: This generated UI is configured for 8-bit LVGL colors; the display bridge converts it into black/red e-paper planes.
- Display resolution: The panel is natively 128×296; this build presents a 296×128 landscape LVGL canvas and rotates/transposes it for the panel.
- LVGL's lv_disp_drv_t flush callback must convert the LVGL buffer to the e-paper's expected format and trigger a display update.

Key Libraries

| Library                  | Purpose                        | Notes                                      |
|--------------------------|--------------------------------|--------------------------------------------|
| GxEPD2               | E-paper driver                 | Use GxEPD2_BW for BW, GxEPD2_3C for RBW. Driver class depends on panel size (e.g., GxEPD2_290_BS for 2.9" BW SSD1680). |
| Adafruit GFX         | Graphics primitives            | Dependency of GxEPD2                       |
| LVGL                 | UI framework                   | SquareLine Studio export target            |
| RTClib (Adafruit)    | DS3231 RTC interface           | I2C on GPIO 8/15                           |
| SD or SdFat      | MicroSD card file access       | FAT32, SPI on shared bus                   |
| Wire                 | I2C for DS3231 and OLED       | Wire.begin(8, 15); OLED at 0x3C        |
| Adafruit SSD1306     | 128x64 Bongo Cat OLED         | Uses the existing Wire bus              |

The optional Bongo Cat OLED reuses the DS3231 I2C bus (SDA GPIO8, SCL GPIO15)
and responds at 0x3C. Its three 128x64 bitmap frames are adapted from
https://github.com/younes-makhchan/esp32-bongo-cat-oled at commit
97ab931ec523e5d270445f355bd18c9a915d0856. The OLED is initialized only after
rtc_init() configures Wire and never starts a second I2C bus. It is nonfatal if
missing; each debounced Red, Yellow, Blue, or Green button press alternates the
cat's left/right press frame without affecting reader actions.

The upstream repository does not include an explicit license file. Confirm
permission/licensing before redistributing the adapted frame artwork.

## GxEPD2 Driver Selection Guide

For WeAct Studio modules using SSD1680:

| Panel Size | BW Driver Class       | 3-Color Driver Class     |
|------------|-----------------------|--------------------------|
| 2.13"      | GxEPD2_213_BN      | GxEPD2_213_Z98c       |
| 2.9"       | GxEPD2_290_BS      | GxEPD2_290_C90c       |

If unsure, consult GxEPD2_display_selection_new_style.h in the GxEPD2 examples for all supported drivers.

Architecture & Key Design Decisions

Reading Flow

1. On boot, initialize I2C (RTC) and SPI (SD card first, then e-paper).
2. Scan the MicroSD card root (or a /books/ directory) for .txt files.
3. Present a book selection menu on the e-paper via LVGL UI.
4. User navigates with buttons (RED/YELLOW/BLUE/GREEN) to select a book.
5. Open the selected .txt file, paginate the text to fit the e-paper resolution.
6. Display one page at a time; buttons control next/previous page, back to menu, etc.
7. Save reading position (current file + byte offset) to SD card or RTC NVRAM for resume functionality.

Text Pagination

- Read a chunk of text from the SD card into a RAM buffer.
- Use LVGL's text layout engine (or manual calculation with font metrics) to determine how many characters/lines fit on one e-paper screen.
- Cache page boundary offsets (byte positions in the file) so backward navigation doesn't require re-reading from the start.
- Be mindful of UTF-8 multi-byte characters — do not split mid-character.

Power Management

- E-paper hibernate after every screen update to protect the display.
- ESP32-S3 deep sleep between interactions to maximize battery life. Wake on button press (configure GPIOs as ext0/ext1 wakeup sources).
- Consider reducing CPU frequency (e.g., 80 MHz or even 10 MHz) during active reading — e-paper refresh still works at lower clock speeds.
- The e-paper retains its image with zero power — perfect for a battery-powered reader.

Memory Constraints

- ESP32-S3 Super Mini has 4 MB flash with a max sketch size of 1280 KB and max SPIFFS/data partition of 320 KB (default partition scheme).
- 512 KB SRAM — LVGL needs a display buffer. For a 128×296 monochrome display, a full framebuffer is only ~4.7 KB (128×296/8), which is very manageable.
- Load text from SD in chunks (e.g., 4 KB at a time) rather than loading entire books into RAM.
- Consider a custom partition table if you need more program space for fonts.

Common Pitfalls

1. SPI bus contention: Always manage CS lines explicitly. Never leave both e-paper CS and SD CS asserted simultaneously. Initialize SD card before e-paper on shared bus.

2. E-paper damage: Forgetting to call hibernate() or powerOff() after every refresh can permanently damage the display.

3. GxEPD2 SPI pins: This build passes the shared SPI object and explicit settings to display.init(); do not add a second remap unless the hardware configuration changes.

4. Boot failures from flash GPIOs: GPIOs 9–14 are wired to the external flash chip on the Super Mini and must not be used as GPIO.

5. GPIO3 at boot: The battery charging circuit on GPIO3 must not pull this pin to an unintended level during power-on reset.

6. SD card not entering SPI mode: If the SD card is not the first device communicated with on the shared SPI bus after initialization, it may stay in SD mode and interfere with all traffic.

7. 3.3V only for e-paper: The e-paper VCC MUST be 3.3V, NOT 5V.

Button Mapping (Suggested)

| Button | Primary Action        | Secondary/Long Press     |
|--------|-----------------------|--------------------------|
| RED    | Back / Exit           | Power off / Deep sleep   |
| YELLOW | Previous page         | Jump back 10 pages       |
| BLUE   | Next page             | Jump forward 10 pages    |
| GREEN  | Select / Confirm      | Toggle info overlay (time, battery, progress) |

This mapping is a suggestion — adapt to your SquareLine UI design.

## Bongo Cat OLED

- Display: 128x64 monochrome SSD1306-compatible I2C OLED at address 0x3C.
- Wiring: same I2C bus as the DS3231, SDA GPIO8 and SCL GPIO15.
- Frames: `main/bongo_cat_frames.h`, adapted from the upstream Bongo Cat project.
- Input: existing debounced `ButtonEvent` path; all four buttons trigger feedback.
- The OLED is optional and does not block startup if unavailable.

Do not copy the upstream standalone `src/main.cpp` or its GPIO16/GPIO17 assumptions into this project. Keep the e-reader's authoritative button and shared-SPI lifecycles intact.

## Build & Upload

Arduino IDE

1. Install esp32 board package by Espressif (v3.x+).
2. Select board: ESP32S3 Dev Module.
3. Set Flash Size: 4MB, Flash Mode: QIO, USB CDC On Boot: Enabled.
4. Install libraries: GxEPD2, Adafruit GFX, LVGL, RTClib, SD (or SdFat), Adafruit SSD1306.
5. Copy SquareLine Studio exported UI files into the sketch's ui/ directory.
6. Configure lv_conf.h for monochrome, correct resolution, and tick source.

PlatformIOini
[env:esp32s3-ereader]
platform = espressif32
board = esp32-s3-devkitm-1
framework = arduino
monitor_speed = 115200
board_build.flash_mode = qio
lib_deps =
    zinggjm/GxEPD2
    adafruit/Adafruit GFX Library
    adafruit/Adafruit SSD1306
    adafruit/RTClib
    lvgl/lvgl
build_flags =
    -D LV_CONF_INCLUDE_SIMPLE
    -D LV_COLOR_DEPTH=1
File Structure (Suggested)
project-root/
├── claude.md                  # This file
├── src/
│   ├── main.cpp               # Entry point, setup/loop
│   ├── display.h/.cpp         # E-paper init, flush, hibernate wrappers
│   ├── sdcard.h/.cpp          # SD card mount, file listing, text reading
│   ├── rtc.h/.cpp              # RTC date/time helpers
│   ├── buttons.h/.cpp          # Button reading, debounce, event dispatch
│   ├── reader.h/.cpp           # Text pagination, bookmarking, page cache
│   ├── bongo_cat.h/.cpp        # Optional 128x64 OLED feedback
│   └── ui/                     # SquareLine Studio generated LVGL UI code
│       ├── ui.h
│       ├── ui.c
│       ├── ui_helpers.h/.c
│       ├── screens/
│       └── fonts/
├── lib/                        # PlatformIO library overrides if needed
└── data/                       # SPIFFS/data files if used

MicroSD Card Structure (Expected)
microsd-root/
├── books/
│   ├── book1.txt
│   ├── book2.txt
│   └── ...
└── .bookmarks/                # Auto-generated rotating bookmark snapshots
    ├── positions.0            # Binary snapshot slot
    ├── positions.1            # Binary snapshot slot
    └── positions.tmp           # Temporary write file, ignored on boot

Bookmark positions are keyed by canonical /books/... paths and store a byte offset together with the file size observed when it was saved. Missing, corrupt, incompatible, stale, or out-of-range records are ignored; they never prevent books from opening. The two slots rotate so a failed write leaves the previous valid snapshot available. Generated SquareLine UI files remain unchanged.

References

- ESP32-S3 Super Mini: https://www.espboards.dev/esp32/esp32-s3-super-mini/
- WeAct E-Paper Module: https://github.com/WeActStudio/WeActStudio.EpaperModule
- GxEPD2 Library: https://github.com/ZinggJM/GxEPD2
- LVGL Documentation: https://docs.lvgl.io/
- SquareLine Studio: https://squareline.io/
- Espressif SPI Bus Sharing with SD Card: https://docs.espressif.com/projects/esp-idf/en/stable/esp32s3/api-reference/peripherals/sdspi_share.html
- RTClib (Adafruit): https://github.com/adafruit/RTClib
- Adafruit SSD1306: https://github.com/adafruit/Adafruit_SSD1306
- Bongo Cat source frames: https://github.com/younes-makhchan/esp32-bongo-cat-oled
