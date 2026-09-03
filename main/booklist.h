#pragma once

#include <Arduino.h>

constexpr size_t BOOKLIST_MAX_BOOKS = 4;
constexpr size_t BOOKLIST_MAX_NAME_LEN = 64;

struct BookEntry {
  char name[BOOKLIST_MAX_NAME_LEN];
  bool has_saved_position;
  uint32_t saved_position;
  uint32_t saved_file_size;
};

void booklist_init();
void booklist_clear();
size_t booklist_count();
bool booklist_add(const char *name);
const BookEntry *booklist_get(size_t index);
bool booklist_set_saved_position(size_t index, uint32_t position, uint32_t file_size);
bool booklist_clear_saved_position(size_t index);
