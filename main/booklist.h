#pragma once

#include <Arduino.h>

constexpr size_t BOOKLIST_MAX_BOOKS = 4;
constexpr size_t BOOKLIST_MAX_NAME_LEN = 64;

struct BookEntry {
  char name[BOOKLIST_MAX_NAME_LEN];
  bool has_saved_position;
  uint32_t saved_position;
};

void booklist_init();
void booklist_clear();
size_t booklist_count();
bool booklist_add(const char *name);
const BookEntry *booklist_get(size_t index);
