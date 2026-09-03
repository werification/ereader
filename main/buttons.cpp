#include "buttons.h"

#include <driver/gpio.h>
#include "config.h"

namespace {
bool readPressed(uint8_t pin) {
  return digitalRead(pin) == LOW;
}

// Release JTAG pins from their default JTAG function so they work as GPIO.
// GPIO 39 (MTCK), 40 (MTDO), 41 (MTDI) are JTAG by default on ESP32-S3.
void release_jtag_pin(uint8_t pin) {
  gpio_reset_pin(static_cast<gpio_num_t>(pin));
}

struct ButtonState {
  bool stable = false;
  bool raw = false;
  uint32_t last_change = 0U;
};

ButtonState red_state;
ButtonState yellow_state;
ButtonState blue_state;
ButtonState green_state;
bool initialized = false;

void initialize_state(ButtonState &state, uint8_t pin, uint32_t now) {
  state.raw = readPressed(pin);
  state.stable = state.raw;
  state.last_change = now;
}

bool debounce(ButtonState &state, bool raw, uint32_t now) {
  constexpr uint32_t DEBOUNCE_MS = 50U;
  if (raw != state.raw) {
    state.raw = raw;
    state.last_change = now;
  }
  if (state.raw != state.stable && (now - state.last_change) >= DEBOUNCE_MS) {
    state.stable = state.raw;
    return true;
  }
  return false;
}
}  // namespace

void buttons_init() {
  // Must release JTAG pins before pinMode works on them.
  release_jtag_pin(PIN_BUTTON_YELLOW);  // GPIO 41
  release_jtag_pin(PIN_BUTTON_BLUE);    // GPIO 39

  pinMode(PIN_BUTTON_RED, INPUT_PULLUP);
  pinMode(PIN_BUTTON_YELLOW, INPUT_PULLUP);
  pinMode(PIN_BUTTON_BLUE, INPUT_PULLUP);
  pinMode(PIN_BUTTON_GREEN, INPUT_PULLUP);

  const uint32_t now = millis();
  initialize_state(red_state, PIN_BUTTON_RED, now);
  initialize_state(yellow_state, PIN_BUTTON_YELLOW, now);
  initialize_state(blue_state, PIN_BUTTON_BLUE, now);
  initialize_state(green_state, PIN_BUTTON_GREEN, now);
  initialized = true;
}

ButtonEvent buttons_poll() {
  if (!initialized) {
    buttons_init();
  }

  const uint32_t now = millis();
  const bool red_changed = debounce(red_state, readPressed(PIN_BUTTON_RED), now);
  const bool yellow_changed = debounce(yellow_state, readPressed(PIN_BUTTON_YELLOW), now);
  const bool blue_changed = debounce(blue_state, readPressed(PIN_BUTTON_BLUE), now);
  const bool green_changed = debounce(green_state, readPressed(PIN_BUTTON_GREEN), now);

  if (red_changed && red_state.stable) return ButtonEvent::RedPressed;
  if (yellow_changed && yellow_state.stable) return ButtonEvent::YellowPressed;
  if (blue_changed && blue_state.stable) return ButtonEvent::BluePressed;
  if (green_changed && green_state.stable) return ButtonEvent::GreenPressed;
  return ButtonEvent::None;
}
