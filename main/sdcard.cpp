#include "sdcard.h"

#include <SD.h>
#include <SPI.h>

#include "booklist.h"
#include "config.h"
#include "utils.h"

bool sdcard_init() {
  digitalWrite(PIN_SD_CS, HIGH);

  Serial.println("SD: trying 4 MHz");
  if (SD.begin(PIN_SD_CS, SPI, 4000000U)) {
    Serial.println("SD: mounted at 4 MHz");
    return true;
  }

  Serial.println("SD: 4 MHz failed, trying 1 MHz");
  if (SD.begin(PIN_SD_CS, SPI, 1000000U)) {
    Serial.println("SD: mounted at 1 MHz");
    return true;
  }

  Serial.println("SD: mount failed");
  return false;
}

void sdcard_scan_books() {
  booklist_clear();

  File root = SD.open("books", FILE_READ);
  if (!root) {
    root = SD.open("/books", FILE_READ);
  }
  if (!root) {
    Serial.println("books directory not found");
    return;
  }

  while (true) {
    File entry = root.openNextFile();
    if (!entry) {
      break;
    }

    if (!entry.isDirectory()) {
      Serial.print("SD entry: ");
      Serial.println(entry.name());

      String name = entry.name();
      if (ends_with_ignore_case(name, ".txt")) {
        String full_path = String("books/") + name;
        Serial.print("Book: ");
        Serial.println(full_path);
        booklist_add(full_path.c_str());
        Serial.print("Book raw entry: ");
        Serial.println(full_path);
        Serial.print("Book count now: ");
        Serial.println(static_cast<unsigned long>(booklist_count()));

        if (booklist_count() > 0U) {
          const BookEntry *entry_info = booklist_get(booklist_count() - 1U);
          if (entry_info != nullptr) {
            Serial.print("Book stored: ");
            Serial.println(entry_info->name);
          }
        }
      }
    }

    entry.close();
  }

  root.close();
}
