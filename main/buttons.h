#pragma once

#include <Arduino.h>

enum class ButtonEvent {
  None,
  RedPressed,
  YellowPressed,
  BluePressed,
  GreenPressed,
};

void buttons_init();
ButtonEvent buttons_poll();
