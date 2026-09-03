#include "booklist.h"

#include <cstring>

namespace {
BookEntry books[BOOKLIST_MAX_BOOKS];
size_t book_count = 0;
}

void booklist_init() {
  booklist_clear();
}

void booklist_clear() {
  book_count = 0;
  std::memset(books, 0, sizeof(books));
}

size_t booklist_count() {
  return book_count;
}

bool booklist_add(const char *name) {
  if (book_count >= BOOKLIST_MAX_BOOKS || name == nullptr) {
    return false;
  }

  const size_t name_length = std::strlen(name);
  if (name_length >= BOOKLIST_MAX_NAME_LEN) {
    return false;
  }

  std::memcpy(books[book_count].name, name, name_length + 1U);
  books[book_count].has_saved_position = false;
  books[book_count].saved_position = 0;
  books[book_count].saved_file_size = 0;
  ++book_count;
  return true;
}

const BookEntry *booklist_get(size_t index) {
  if (index >= book_count) {
    return nullptr;
  }
  return &books[index];
}

bool booklist_set_saved_position(size_t index, uint32_t position, uint32_t file_size) {
  if (index >= book_count || position > file_size) {
    return false;
  }
  books[index].has_saved_position = true;
  books[index].saved_position = position;
  books[index].saved_file_size = file_size;
  return true;
}

bool booklist_clear_saved_position(size_t index) {
  if (index >= book_count) {
    return false;
  }
  books[index].has_saved_position = false;
  books[index].saved_position = 0;
  books[index].saved_file_size = 0;
  return true;
}
