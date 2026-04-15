#include "reader.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <SD.h>

extern "C" {
#include "../UI_Export/ui.h"
}

#include "booklist.h"

namespace {
constexpr const lv_font_t *READING_FONT = &lv_font_montserrat_12;
constexpr int32_t READING_LETTER_SPACE = 0;
constexpr int32_t READING_LINE_SPACE = 0;
constexpr size_t PAGE_BUFFER_BYTES = 4096U;
constexpr size_t BOOK_PATH_BYTES = 96U;

enum class ScreenMode {
  MainMenu,
  BookSelection,
  Reading,
};

ScreenMode screen_mode = ScreenMode::MainMenu;
bool active_book = false;
size_t selected_book_index = 0;
size_t active_book_index = 0;
size_t active_page = 0;

char active_book_path[BOOK_PATH_BYTES];
char page_buffer[PAGE_BUFFER_BYTES];

uint32_t *page_offsets = nullptr;
size_t page_offsets_count = 0;
size_t page_offsets_capacity = 0;

lv_obj_t *reading_text_label = nullptr;

void ensure_main_menu_screen() {
  if (ui_MainMenu == nullptr) {
    ui_MainMenu_screen_init();
  }
}

void ensure_reading_screen() {
  if (ui_ReadingScreen == nullptr) {
    ui_ReadingScreen_screen_init();
  }
}

void ensure_book_selection_screen() {
  if (ui_BookSelection == nullptr) {
    ui_BookSelection_screen_init();
  }
}

bool is_utf8_continuation_byte(unsigned char byte) {
  return (byte & 0xC0U) == 0x80U;
}

size_t utf8_char_length(unsigned char lead) {
  if ((lead & 0x80U) == 0U) {
    return 1U;
  }
  if ((lead & 0xE0U) == 0xC0U) {
    return 2U;
  }
  if ((lead & 0xF0U) == 0xE0U) {
    return 3U;
  }
  if ((lead & 0xF8U) == 0xF0U) {
    return 4U;
  }
  return 1U;
}

bool is_breakable_char(unsigned char c) {
  return c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == '-' || c == ',' || c == '.';
}

size_t trim_trailing_whitespace(size_t len) {
  while (len > 0U) {
    const unsigned char c = static_cast<unsigned char>(page_buffer[len - 1U]);
    if (c == ' ' || c == '\n' || c == '\r' || c == '\t') {
      --len;
      continue;
    }
    break;
  }
  return len;
}

size_t skip_leading_whitespace_in_file(File &file, size_t offset, size_t file_size) {
  while (offset < file_size) {
    if (!file.seek(offset)) {
      return offset;
    }

    const int byte = file.read();
    if (byte < 0) {
      return offset;
    }

    if (byte == ' ' || byte == '\n' || byte == '\r' || byte == '\t') {
      ++offset;
      continue;
    }

    file.seek(offset);
    break;
  }

  return offset;
}

bool read_utf8_char(File &file, char *out, size_t &out_len) {
  const int first = file.read();
  if (first < 0) {
    return false;
  }

  out[0] = static_cast<char>(first);
  out_len = utf8_char_length(static_cast<unsigned char>(first));
  for (size_t i = 1; i < out_len; ++i) {
    const int next = file.read();
    if (next < 0) {
      out_len = i;
      return false;
    }
    out[i] = static_cast<char>(next);
  }

  return true;
}

void clear_active_book_content() {
  std::memset(active_book_path, 0, sizeof(active_book_path));
  std::memset(page_buffer, 0, sizeof(page_buffer));

  std::free(page_offsets);
  page_offsets = nullptr;
  page_offsets_count = 0;
  page_offsets_capacity = 0;
  active_page = 0;
}

bool reserve_page_offset_capacity(size_t needed) {
  if (needed <= page_offsets_capacity) {
    return true;
  }

  size_t new_capacity = (page_offsets_capacity == 0U) ? 16U : page_offsets_capacity * 2U;
  while (new_capacity < needed) {
    new_capacity *= 2U;
  }

  void *raw = std::realloc(page_offsets, new_capacity * sizeof(uint32_t));
  if (raw == nullptr) {
    return false;
  }

  page_offsets = static_cast<uint32_t *>(raw);
  page_offsets_capacity = new_capacity;
  return true;
}

bool push_page_offset(uint32_t offset) {
  if (!reserve_page_offset_capacity(page_offsets_count + 1U)) {
    return false;
  }

  page_offsets[page_offsets_count++] = offset;
  return true;
}

size_t page_count() {
  if (page_offsets_count == 0U) {
    return 0U;
  }
  return page_offsets_count - 1U;
}

bool page_fits_text(const char *text) {
  lv_point_t size{};
  const int32_t max_width = static_cast<int32_t>(lv_obj_get_width(ui_TextZone));
  const int32_t max_height = static_cast<int32_t>(lv_obj_get_height(ui_TextZone));
  lv_txt_get_size(&size, text, READING_FONT, READING_LETTER_SPACE, READING_LINE_SPACE, max_width, LV_TEXT_FLAG_NONE);
  return size.y <= max_height;
}

bool load_active_book_info(size_t index) {
  const BookEntry *entry = booklist_get(index);
  if (entry == nullptr) {
    Serial.println("reader: no book entry");
    return false;
  }

  std::snprintf(active_book_path, sizeof(active_book_path), "%s", entry->name);
  Serial.print("reader: selected index ");
  Serial.println(static_cast<unsigned long>(index));
  Serial.print("reader: opening ");
  Serial.println(active_book_path);
  Serial.print("reader: booklist name ");
  Serial.println(entry->name);
  Serial.print("reader: exists active path ");
  Serial.println(SD.exists(active_book_path) ? "yes" : "no");

  File file = SD.open(active_book_path, FILE_READ);
  if (!file) {
    Serial.println("reader: failed to open file");
    return false;
  }
  file.close();
  Serial.println("reader: file open ok");
  return true;
}

bool build_page_table() {
  page_offsets_count = 0;

  if (!push_page_offset(0U)) {
    Serial.println("reader: page offset alloc failed");
    return false;
  }

  File file = SD.open(active_book_path, FILE_READ);
  if (!file) {
    Serial.println("reader: reopen failed for pagination");
    return false;
  }

  const size_t file_size = static_cast<size_t>(file.size());
  Serial.print("reader: file size ");
  Serial.println(static_cast<unsigned long>(file_size));

  size_t page_start = skip_leading_whitespace_in_file(file, 0U, file_size);
  page_offsets[0] = static_cast<uint32_t>(page_start);

  while (page_start < file_size) {
    if (!file.seek(page_start)) {
      Serial.println("reader: seek failed during pagination");
      file.close();
      return false;
    }

    size_t candidate_len = 0U;
    size_t best_end = page_start;
    size_t last_break_end = page_start;
    char char_bytes[4];

    while (candidate_len < PAGE_BUFFER_BYTES - 1U && file.position() < file_size) {
      size_t char_len = 0U;
      if (!read_utf8_char(file, char_bytes, char_len)) {
        break;
      }

      if (candidate_len + char_len >= PAGE_BUFFER_BYTES) {
        break;
      }

      std::memcpy(page_buffer + candidate_len, char_bytes, char_len);
      candidate_len += char_len;
      page_buffer[candidate_len] = '\0';

      const size_t candidate_end = page_start + candidate_len;
      if (page_fits_text(page_buffer)) {
        best_end = candidate_end;
        if (is_breakable_char(static_cast<unsigned char>(char_bytes[0]))) {
          last_break_end = candidate_end;
        }
      } else {
        break;
      }
    }

    size_t page_end = (last_break_end > page_start) ? last_break_end : best_end;
    if (page_end <= page_start) {
      page_end = page_start + utf8_char_length(static_cast<unsigned char>(page_buffer[0]));
    }

    if (page_end > file_size) {
      page_end = file_size;
    }

    size_t next_start = skip_leading_whitespace_in_file(file, page_end, file_size);
    if (next_start <= page_start) {
      next_start = page_end;
    }

    if (!push_page_offset(static_cast<uint32_t>(next_start))) {
      Serial.println("reader: page offset append failed");
      file.close();
      return false;
    }

    page_start = next_start;
  }

  if (page_offsets[page_offsets_count - 1U] != file_size) {
    if (!push_page_offset(static_cast<uint32_t>(file_size))) {
      Serial.println("reader: final offset append failed");
      file.close();
      return false;
    }
  }

  file.close();
  Serial.print("reader: page count ");
  Serial.println(static_cast<unsigned long>(page_count()));
  return true;
}

void refresh_book_selection_ui() {
  ensure_book_selection_screen();

  const lv_obj_t *name_labels[] = {ui_BookName, ui_BookName1, ui_BookName2, ui_BookName3};
  const lv_obj_t *page_labels[] = {ui_PageNum, ui_PageNum1, ui_PageNum2, ui_PageNum3};
  const lv_obj_t *status_labels[] = {ui_LastOpen, ui_LastOpen1, ui_LastOpen2, ui_LastOpen3};

  for (size_t i = 0; i < 4; ++i) {
    lv_obj_t *name_label = const_cast<lv_obj_t *>(name_labels[i]);
    lv_obj_set_width(name_label, 150);
    lv_label_set_long_mode(name_label, LV_LABEL_LONG_CLIP);
    lv_obj_set_style_text_align(name_label, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);

    const BookEntry *entry = booklist_get(i);
    if (entry != nullptr) {
      lv_label_set_text(name_label, entry->name);
      char page_text[16];
      std::snprintf(page_text, sizeof(page_text), "%u", static_cast<unsigned>(i + 1U));
      lv_label_set_text(const_cast<lv_obj_t *>(page_labels[i]), page_text);
      lv_label_set_text(const_cast<lv_obj_t *>(status_labels[i]), (i == selected_book_index) ? "Selected" : "");
    } else {
      lv_label_set_text(name_label, "");
      lv_label_set_text(const_cast<lv_obj_t *>(page_labels[i]), "");
      lv_label_set_text(const_cast<lv_obj_t *>(status_labels[i]), "");
    }
  }

  if (booklist_count() == 0U) {
    lv_label_set_text(ui_BookSelect, "No books found");
  } else {
    lv_label_set_text(ui_BookSelect, "Select your book");
  }
}

void update_reading_ui(const char *text) {
  ensure_reading_screen();
  if (reading_text_label == nullptr) {
    reading_text_label = lv_label_create(ui_TextZone);
    lv_obj_set_width(reading_text_label, lv_obj_get_width(ui_TextZone));
    lv_obj_set_style_text_color(reading_text_label, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(reading_text_label, READING_FONT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_long_mode(reading_text_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(reading_text_label, LV_ALIGN_TOP_LEFT, 0, 0);
  }

  lv_label_set_text(reading_text_label, text);

  char page_text[32];
  std::snprintf(page_text, sizeof(page_text), "Page %u", static_cast<unsigned>(active_page + 1U));
  lv_label_set_text(ui_PageNumRead, page_text);
}

bool show_page(size_t page_index) {
  if (page_index >= page_count()) {
    return false;
  }

  const size_t start = page_offsets[page_index];
  const size_t end = page_offsets[page_index + 1U];
  const size_t read_len = (end > start) ? (end - start) : 0U;
  if (read_len == 0U) {
    update_reading_ui("");
    return true;
  }

  File file = SD.open(active_book_path, FILE_READ);
  if (!file) {
    Serial.println("reader: failed to reopen book for page draw");
    return false;
  }
  if (!file.seek(start)) {
    Serial.println("reader: failed to seek to page start");
    file.close();
    return false;
  }

  const size_t capped_read_len = (read_len < PAGE_BUFFER_BYTES - 1U) ? read_len : (PAGE_BUFFER_BYTES - 1U);
  const size_t got = file.readBytes(page_buffer, capped_read_len);
  file.close();

  page_buffer[got] = '\0';
  const size_t trimmed_end = trim_trailing_whitespace(got);
  page_buffer[trimmed_end] = '\0';
  update_reading_ui(page_buffer);
  return true;
}
}  // namespace

void reader_show_main_menu() {
  ensure_main_menu_screen();
  active_book = false;
  clear_active_book_content();
  screen_mode = ScreenMode::MainMenu;
  lv_disp_load_scr(ui_MainMenu);
}

void reader_show_book_selection_screen() {
  ensure_book_selection_screen();
  screen_mode = ScreenMode::BookSelection;
  refresh_book_selection_ui();
  lv_disp_load_scr(ui_BookSelection);
}

void reader_show_reading_screen() {
  ensure_reading_screen();
  screen_mode = ScreenMode::Reading;
  lv_disp_load_scr(ui_ReadingScreen);
}

void reader_show_storage_error() {
  ensure_main_menu_screen();
  active_book = false;
  clear_active_book_content();
  screen_mode = ScreenMode::MainMenu;
  lv_label_set_text(ui_time, "Insert SD card");
  lv_label_set_text(ui_date, "");
  lv_disp_load_scr(ui_MainMenu);
}

void reader_update_clock_labels(const char *time_text, const char *date_text) {
  if (time_text == nullptr || date_text == nullptr) {
    return;
  }

  switch (screen_mode) {
    case ScreenMode::MainMenu:
      ensure_main_menu_screen();
      lv_label_set_text(ui_time, time_text);
      lv_label_set_text(ui_date, date_text);
      break;
    case ScreenMode::BookSelection:
      ensure_book_selection_screen();
      lv_label_set_text(ui_BookSelTime, time_text);
      break;
    case ScreenMode::Reading:
      ensure_reading_screen();
      lv_label_set_text(ui_TimeReading, time_text);
      break;
  }
}

void reader_update_battery_label(const char *battery_text) {
  if (battery_text == nullptr) {
    return;
  }

  switch (screen_mode) {
    case ScreenMode::MainMenu:
      ensure_main_menu_screen();
      lv_label_set_text(ui_batterypercentage, battery_text);
      break;
    case ScreenMode::BookSelection:
      ensure_book_selection_screen();
      lv_label_set_text(ui_BookSelPercentage, battery_text);
      break;
    case ScreenMode::Reading:
      break;
  }
}

void reader_set_book_selection_empty_state() {
  ensure_book_selection_screen();
  lv_label_set_text(ui_BookSelect, "No books found");
}

bool reader_has_active_book() {
  return active_book;
}

bool reader_has_selected_book() {
  return booklist_count() > 0U;
}

bool reader_is_on_main_menu() {
  return screen_mode == ScreenMode::MainMenu;
}

bool reader_is_on_book_selection() {
  return screen_mode == ScreenMode::BookSelection;
}

bool reader_is_on_reading_screen() {
  return screen_mode == ScreenMode::Reading;
}

size_t reader_selected_book_index() {
  return selected_book_index;
}

bool reader_select_book(size_t index) {
  if (booklist_get(index) == nullptr) {
    return false;
  }

  selected_book_index = index;
  refresh_book_selection_ui();
  return true;
}

void reader_refresh_book_selection() {
  refresh_book_selection_ui();
}

bool reader_select_next_book() {
  const size_t count = booklist_count();
  if (count == 0U) {
    return false;
  }

  selected_book_index = (selected_book_index + 1U) % count;
  refresh_book_selection_ui();
  return true;
}

bool reader_select_prev_book() {
  const size_t count = booklist_count();
  if (count == 0U) {
    return false;
  }

  selected_book_index = (selected_book_index == 0U) ? (count - 1U) : (selected_book_index - 1U);
  refresh_book_selection_ui();
  return true;
}

bool reader_open_selected_book() {
  return reader_open_book(selected_book_index);
}

bool reader_open_book(size_t index) {
  if (booklist_get(index) == nullptr) {
    Serial.println("reader: open_book invalid index");
    return false;
  }

  if (!load_active_book_info(index)) {
    active_book = false;
    return false;
  }

  if (!build_page_table()) {
    clear_active_book_content();
    active_book = false;
    return false;
  }

  active_book = true;
  active_book_index = index;
  active_page = 0;
  reader_show_reading_screen();
  return show_page(active_page);
}

bool reader_next_page() {
  if (!active_book) {
    return false;
  }

  if (active_page + 1U >= page_count()) {
    return false;
  }

  ++active_page;
  return show_page(active_page);
}

bool reader_prev_page() {
  if (!active_book || active_page == 0U) {
    return false;
  }

  --active_page;
  return show_page(active_page);
}

void reader_show_menu_on_boot() {
  reader_show_book_selection_screen();
}
