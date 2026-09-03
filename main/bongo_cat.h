#pragma once

#include "buttons.h"

// Initializes the optional 128x64 Bongo Cat OLED on the already-configured
// Wire bus. A missing OLED is nonfatal to the reader.
void bongo_cat_init();

// Shows one press frame for every debounced button event.
void bongo_cat_press(ButtonEvent event);
