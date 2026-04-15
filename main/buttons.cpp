#include "buttons.h"

#include "config.h"

namespace {
bool readPressed(uint8_t pin) {
  return digitalRead(pin) == LOW;
}
}  // namespace

void buttons_init() {
  pinMode(PIN_BUTTON_RED, INPUT_PULLUP);
  pinMode(PIN_BUTTON_YELLOW, INPUT_PULLUP);
  pinMode(PIN_BUTTON_BLUE, INPUT_PULLUP);
  pinMode(PIN_BUTTON_GREEN, INPUT_PULLUP);
}

ButtonEvent buttons_poll() {
  static bool prevRed = false;
  static bool prevYellow = false;
  static bool prevBlue = false;
  static bool prevGreen = false;

  const bool red = readPressed(PIN_BUTTON_RED);
  const bool yellow = readPressed(PIN_BUTTON_YELLOW);
  const bool blue = readPressed(PIN_BUTTON_BLUE);
  const bool green = readPressed(PIN_BUTTON_GREEN);

  ButtonEvent event = ButtonEvent::None;
  if (red && !prevRed) event = ButtonEvent::RedPressed;
  else if (yellow && !prevYellow) event = ButtonEvent::YellowPressed;
  else if (blue && !prevBlue) event = ButtonEvent::BluePressed;
  else if (green && !prevGreen) event = ButtonEvent::GreenPressed;

  prevRed = red;
  prevYellow = yellow;
  prevBlue = blue;
  prevGreen = green;

  return event;
}
