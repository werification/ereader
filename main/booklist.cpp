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

  std::strncpy(books[book_count].name, name, BOOKLIST_MAX_NAME_LEN - 1);
  books[book_count].name[BOOKLIST_MAX_NAME_LEN - 1] = '\0';
  books[book_count].has_saved_position = false;
  books[book_count].saved_position = 0;
  ++book_count;
  return true;
}

const BookEntry *booklist_get(size_t index) {
  if (index >= book_count) {
    return nullptr;
  }
  return &books[index];
}
