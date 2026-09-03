#include "reader.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>

#include <lvgl.h>

extern "C" const lv_font_t ui_font_reader;

extern "C" {
#include "../UI_Export/ui.h"
}

#include "booklist.h"
#include "config.h"
#include "sdcard.h"

namespace {
constexpr const lv_font_t *READING_FONT = &ui_font_reader;
constexpr int32_t READING_LETTER_SPACE = 0;
constexpr int32_t READING_LINE_SPACE = 0;
constexpr size_t PAGE_BUFFER_BYTES = 4096U;
constexpr size_t BOOK_PATH_BYTES = 96U;
constexpr int32_t TEXT_ZONE_WIDTH = 287;
constexpr int32_t TEXT_ZONE_HEIGHT = 103;
constexpr size_t INITIAL_PAGE_OFFSET_CAPACITY = 16U;

using FileOffset = uint32_t;
constexpr FileOffset MAX_FILE_OFFSET = std::numeric_limits<FileOffset>::max();

enum class ScreenMode {
  MainMenu,
  BookSelection,
  Reading,
};

struct PageTable {
  FileOffset *offsets = nullptr;
  size_t count = 0U;
  size_t capacity = 0U;
  FileOffset file_size = 0U;
  bool complete = false;
};

struct DecodedUnit {
  FileOffset source_start = 0U;
  FileOffset source_end = 0U;
  char rendered[4] = {};
  size_t rendered_len = 0U;
};

ScreenMode screen_mode = ScreenMode::MainMenu;
bool active_book = false;
bool storage_error_active = false;
size_t selected_book_index = 0U;
size_t active_book_index = 0U;
size_t active_page = 0U;
uint32_t active_sd_generation = 0U;

char active_book_path[BOOK_PATH_BYTES] = {};
char page_buffer[PAGE_BUFFER_BYTES] = {};
char pagination_buffer[PAGE_BUFFER_BYTES] = {};
char candidate_page_buffer[PAGE_BUFFER_BYTES] = {};

FileOffset *page_offsets = nullptr;
size_t page_offsets_count = 0U;
size_t page_offsets_capacity = 0U;
FileOffset active_book_file_size = 0U;
bool pending_bookmark_save = false;
bool page_offsets_complete = false;

lv_obj_t *reading_text_label = nullptr;
lv_obj_t *error_text_label = nullptr;

void ensure_book_selection_screen();

void mark_storage_failure(const char *operation) {
  sdcard_mark_unavailable(operation);
}

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

void ensure_error_label() {
  ensure_book_selection_screen();
  if (error_text_label == nullptr) {
    error_text_label = lv_label_create(ui_BookSelection);
    lv_obj_set_width(error_text_label, 260);
    lv_obj_set_style_text_color(error_text_label, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(error_text_label, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_long_mode(error_text_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(error_text_label, LV_ALIGN_CENTER, 0, 0);
  }
  lv_obj_move_foreground(error_text_label);
}

void clear_book_selection_error() {
  if (error_text_label != nullptr) {
    lv_obj_add_flag(error_text_label, LV_OBJ_FLAG_HIDDEN);
  }
}

void refresh_book_selection_ui() {
  ensure_book_selection_screen();

  const size_t count = booklist_count();
  if (count == 0U) {
    selected_book_index = 0U;
  } else if (selected_book_index >= count) {
    selected_book_index = count - 1U;
  }

  lv_obj_t *name_labels[] = {ui_BookName, ui_BookName1, ui_BookName2, ui_BookName3};
  lv_obj_t *page_labels[] = {ui_PageNum, ui_PageNum1, ui_PageNum2, ui_PageNum3};
  lv_obj_t *status_labels[] = {ui_LastOpen, ui_LastOpen1, ui_LastOpen2, ui_LastOpen3};

  for (size_t i = 0U; i < 4U; ++i) {
    lv_obj_set_width(name_labels[i], 150);
    lv_label_set_long_mode(name_labels[i], LV_LABEL_LONG_CLIP);
    lv_obj_set_style_text_align(name_labels[i], LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);

    const BookEntry *entry = booklist_get(i);
    if (entry != nullptr) {
      lv_label_set_text(name_labels[i], entry->name);
      char row_text[16];
      std::snprintf(row_text, sizeof(row_text), "%u", static_cast<unsigned>(i + 1U));
      lv_label_set_text(page_labels[i], row_text);
      // Always prioritize the current selection marker so it remains clear
      // which row Green will open. Resume is shown for saved books that are
      // not currently selected.
      const char *status = (i == selected_book_index)
                               ? "Selected"
                               : (entry->has_saved_position ? "Resume" : "");
      lv_label_set_text(status_labels[i], status);
    } else {
      lv_label_set_text(name_labels[i], "");
      lv_label_set_text(page_labels[i], "");
      lv_label_set_text(status_labels[i], "");
    }
  }

  lv_label_set_text(ui_BookSelect, (booklist_count() == 0U) ? "No books found" : "Select your book");
}

void clear_active_book_content() {
  active_sd_generation = 0U;
  std::free(page_offsets);
  page_offsets = nullptr;
  page_offsets_count = 0U;
  page_offsets_capacity = 0U;
  active_book_file_size = 0U;
  page_offsets_complete = false;
  active_page = 0U;
  active_book_path[0] = '\0';
  page_buffer[0] = '\0';
}

bool checked_file_size(SdFile &file, FileOffset &file_size) {
  const uint64_t size = static_cast<uint64_t>(file.size());
  if (size > static_cast<uint64_t>(MAX_FILE_OFFSET)) {
    DBG_PRINTLN("reader: file exceeds 32-bit seek range");
    return false;
  }
  file_size = static_cast<FileOffset>(size);
  return true;
}

void invalidate_reading_label_if_needed() {
  if (ui_ReadingScreen == nullptr || reading_text_label == nullptr) {
    return;
  }
  // The generated screen is normally retained. If it was destroyed by a
  // caller, the next ensure_reading_screen() creates a new parent and the
  // child pointer must not be reused.
  if (lv_obj_get_parent(reading_text_label) != ui_TextZone) {
    reading_text_label = nullptr;
  }
}

bool is_ascii_whitespace(unsigned char byte) {
  return byte == ' ' || byte == '\n' || byte == '\r' || byte == '\t';
}

bool is_breakable_unit(const DecodedUnit &unit) {
  if (unit.rendered_len != 1U) {
    return false;
  }

  const unsigned char byte = static_cast<unsigned char>(unit.rendered[0]);
  return is_ascii_whitespace(byte) || byte == '-' || byte == ',' || byte == '.' || byte == ';' || byte == ':';
}

bool is_utf8_continuation_byte(unsigned char byte) {
  return (byte & 0xC0U) == 0x80U;
}

uint32_t windows1252_code_point(unsigned char byte) {
  if (byte >= 0xA0U) {
    return static_cast<uint32_t>(byte);
  }

  // Windows-1252 replaces the C1 control range with punctuation and symbols.
  // Undefined entries remain replacement characters rather than controls.
  constexpr uint32_t c1_map[32] = {
      0x20ACU, 0U,      0x201AU, 0x192U, 0x201EU, 0x2026U, 0x2020U, 0x2021U,
      0x2C6U,  0x2030U, 0x160U,  0x2039U, 0x152U, 0U,      0x17DU,  0U,
      0U,      0x2018U, 0x2019U, 0x201CU, 0x201DU, 0x2022U, 0x2013U, 0x2014U,
      0x2DCU,  0x2122U, 0x161U,  0x203AU, 0x153U, 0U,      0x17EU, 0x178U,
  };
  return c1_map[byte - 0x80U];
}

bool set_code_point_unit(DecodedUnit &unit, FileOffset start, uint32_t code_point) {
  unit.source_start = start;
  unit.source_end = start + 1U;

  if (code_point <= 0x7FU) {
    unit.rendered[0] = static_cast<char>(code_point);
    unit.rendered_len = 1U;
  } else if (code_point <= 0x7FFU) {
    unit.rendered[0] = static_cast<char>(0xC0U | (code_point >> 6U));
    unit.rendered[1] = static_cast<char>(0x80U | (code_point & 0x3FU));
    unit.rendered_len = 2U;
  } else if (code_point <= 0xFFFFU && !(code_point >= 0xD800U && code_point <= 0xDFFFU)) {
    unit.rendered[0] = static_cast<char>(0xE0U | (code_point >> 12U));
    unit.rendered[1] = static_cast<char>(0x80U | ((code_point >> 6U) & 0x3FU));
    unit.rendered[2] = static_cast<char>(0x80U | (code_point & 0x3FU));
    unit.rendered_len = 3U;
  } else if (code_point <= 0x10FFFFU) {
    unit.rendered[0] = static_cast<char>(0xF0U | (code_point >> 18U));
    unit.rendered[1] = static_cast<char>(0x80U | ((code_point >> 12U) & 0x3FU));
    unit.rendered[2] = static_cast<char>(0x80U | ((code_point >> 6U) & 0x3FU));
    unit.rendered[3] = static_cast<char>(0x80U | (code_point & 0x3FU));
    unit.rendered_len = 4U;
  } else {
    return false;
  }
  return true;
}

constexpr size_t MAX_UTF8_REPLACEMENT_LOGS = 32U;
size_t utf8_replacement_log_count = 0U;

void log_utf8_replacement(FileOffset start,
                          const unsigned char *bytes,
                          size_t byte_count,
                          const char *reason) {
  ++utf8_replacement_log_count;
  if (utf8_replacement_log_count > MAX_UTF8_REPLACEMENT_LOGS) {
    return;
  }

  char line[128] = {};
  size_t used = static_cast<size_t>(std::snprintf(
      line, sizeof(line), "reader: UTF-8 replacement at byte %lu (%s):",
      static_cast<unsigned long>(start), (reason != nullptr) ? reason : "invalid"));
  if (used >= sizeof(line)) {
    used = sizeof(line) - 1U;
  }
  for (size_t i = 0U; i < byte_count && used + 4U < sizeof(line); ++i) {
    const int written = std::snprintf(line + used, sizeof(line) - used,
                                      " %02X", static_cast<unsigned>(bytes[i]));
    if (written <= 0) {
      break;
    }
    used += static_cast<size_t>(written);
  }
  DBG_PRINTLN(line);
}

void set_replacement_unit(DecodedUnit &unit, FileOffset start) {
  unit.source_start = start;
  unit.source_end = start + 1U;
  unit.rendered[0] = '?';
  unit.rendered_len = 1U;
}

bool set_windows1252_fallback_unit(DecodedUnit &unit, FileOffset start, unsigned char byte) {
  const uint32_t code_point = windows1252_code_point(byte);
  return code_point != 0U && set_code_point_unit(unit, start, code_point);
}

bool rewind_after_malformed_sequence(SdFile &file, FileOffset start) {
  // The malformed sequence must consume exactly its lead byte. Rewinding any
  // bytes read speculatively leaves each following byte available for the next
  // deterministic replacement unit.
  return file.seek(start + 1U);
}

bool read_decoded_unit(SdFile &file, FileOffset file_size, DecodedUnit &unit) {
  const FileOffset start = static_cast<FileOffset>(file.position());
  if (start >= file_size) {
    return false;
  }

  const int first = file.read();
  if (first < 0) {
    return false;
  }

  const unsigned char lead = static_cast<unsigned char>(first);
  if (lead == '\r') {
    // CRLF occupies two source bytes but is one rendered newline. This keeps
    // page offsets byte-accurate while matching the visual text layout.
    if (file.peek() == '\n') {
      file.read();
    }
    unit.source_start = start;
    unit.source_end = static_cast<FileOffset>(file.position());
    unit.rendered[0] = '\n';
    unit.rendered_len = 1U;
    return true;
  }

  size_t expected_len = 0U;
  if (lead <= 0x7FU) {
    expected_len = 1U;
  } else if (lead >= 0xC2U && lead <= 0xDFU) {
    expected_len = 2U;
  } else if (lead >= 0xE0U && lead <= 0xEFU) {
    expected_len = 3U;
  } else if (lead >= 0xF0U && lead <= 0xF4U) {
    expected_len = 4U;
  }

  // NUL cannot be represented in an LVGL C string; replace it just like other
  // unsupported input instead of accidentally terminating the page.
  if (lead == 0U || expected_len == 0U) {
    log_utf8_replacement(start, &lead, 1U, (lead == 0U) ? "NUL" : "invalid lead");
    set_replacement_unit(unit, start);
    return true;
  }

  unsigned char bytes[4] = {lead, 0U, 0U, 0U};
  bool complete = true;
  for (size_t i = 1U; i < expected_len; ++i) {
    const int next = file.read();
    if (next < 0) {
      complete = false;
      break;
    }
    bytes[i] = static_cast<unsigned char>(next);
  }

  if (!complete) {
    size_t bytes_read = static_cast<size_t>(file.position()) - static_cast<size_t>(start);
    if (bytes_read > expected_len) {
      bytes_read = expected_len;
    }
    log_utf8_replacement(start, bytes, bytes_read, "truncated");
    if (!rewind_after_malformed_sequence(file, start)) {
      return false;
    }
    set_replacement_unit(unit, start);
    return true;
  }

  bool valid = true;
  for (size_t i = 1U; i < expected_len; ++i) {
    if (!is_utf8_continuation_byte(bytes[i])) {
      valid = false;
      break;
    }
  }

  // Enforce the UTF-8 shortest-form and Unicode scalar-value ranges. The
  // second-byte constraints reject overlong values, surrogate halves, and
  // code points above U+10FFFF before the code point is accepted.
  if (valid && expected_len == 3U) {
    if ((lead == 0xE0U && bytes[1] < 0xA0U) ||
        (lead == 0xEDU && bytes[1] >= 0xA0U)) {
      valid = false;
    }
  } else if (valid && expected_len == 4U) {
    if ((lead == 0xF0U && bytes[1] < 0x90U) ||
        (lead == 0xF4U && bytes[1] > 0x8FU)) {
      valid = false;
    }
  }

  if (valid) {
    uint32_t code_point = static_cast<uint32_t>(lead);
    if (expected_len == 2U) {
      code_point = (static_cast<uint32_t>(lead & 0x1FU) << 6U) |
                   static_cast<uint32_t>(bytes[1] & 0x3FU);
    } else if (expected_len == 3U) {
      code_point = (static_cast<uint32_t>(lead & 0x0FU) << 12U) |
                   (static_cast<uint32_t>(bytes[1] & 0x3FU) << 6U) |
                   static_cast<uint32_t>(bytes[2] & 0x3FU);
    } else if (expected_len == 4U) {
      code_point = (static_cast<uint32_t>(lead & 0x07U) << 18U) |
                   (static_cast<uint32_t>(bytes[1] & 0x3FU) << 12U) |
                   (static_cast<uint32_t>(bytes[2] & 0x3FU) << 6U) |
                   static_cast<uint32_t>(bytes[3] & 0x3FU);
    }

    if (code_point > 0x10FFFFU || (code_point >= 0xD800U && code_point <= 0xDFFFU)) {
      valid = false;
    }
  }

  if (!valid) {
    const char *reason = "invalid continuation";
    if (expected_len == 3U &&
        ((lead == 0xE0U && bytes[1] < 0xA0U) || (lead == 0xEDU && bytes[1] >= 0xA0U))) {
      reason = "invalid scalar range";
    } else if (expected_len == 4U &&
               ((lead == 0xF0U && bytes[1] < 0x90U) || (lead == 0xF4U && bytes[1] > 0x8FU))) {
      reason = "invalid scalar range";
    }
    log_utf8_replacement(start, bytes, expected_len, reason);
    if (!rewind_after_malformed_sequence(file, start)) {
      return false;
    }
    set_replacement_unit(unit, start);
    return true;
  }

  unit.source_start = start;
  unit.source_end = static_cast<FileOffset>(file.position());
  unit.rendered_len = expected_len;
  std::memcpy(unit.rendered, bytes, expected_len);
  return true;
}

bool reserve_page_offset_capacity(PageTable &table, size_t needed) {
  if (needed <= table.capacity) {
    return true;
  }

  const size_t max_size = std::numeric_limits<size_t>::max();
  if (needed > max_size / sizeof(FileOffset)) {
    return false;
  }

  size_t new_capacity = (table.capacity == 0U) ? INITIAL_PAGE_OFFSET_CAPACITY : table.capacity;
  while (new_capacity < needed) {
    if (new_capacity > max_size / 2U) {
      new_capacity = needed;
      break;
    }
    new_capacity *= 2U;
  }

  if (new_capacity < needed || new_capacity > max_size / sizeof(FileOffset)) {
    return false;
  }

  void *raw = std::realloc(table.offsets, new_capacity * sizeof(FileOffset));
  if (raw == nullptr) {
    return false;
  }

  table.offsets = static_cast<FileOffset *>(raw);
  table.capacity = new_capacity;
  return true;
}

bool push_page_offset(PageTable &table, FileOffset offset) {
  if (table.count > 0U && offset < table.offsets[table.count - 1U]) {
    return false;
  }

  if (table.count == std::numeric_limits<size_t>::max()) {
    return false;
  }
  if (!reserve_page_offset_capacity(table, table.count + 1U)) {
    return false;
  }

  table.offsets[table.count++] = offset;
  return true;
}

void destroy_page_table(PageTable &table) {
  std::free(table.offsets);
  table.offsets = nullptr;
  table.count = 0U;
  table.capacity = 0U;
  table.file_size = 0U;
  table.complete = false;
}

bool validate_page_table(const PageTable &table) {
  if (table.offsets == nullptr || table.count < 2U || table.offsets[0] > table.file_size) {
    return false;
  }
  if (table.complete && table.offsets[table.count - 1U] != table.file_size) {
    return false;
  }

  for (size_t i = 1U; i < table.count; ++i) {
    if (table.offsets[i] < table.offsets[i - 1U] || table.offsets[i] > table.file_size) {
      return false;
    }
  }
  return true;
}


bool skip_source_whitespace(SdFile &file, FileOffset offset, FileOffset file_size, FileOffset &result) {
  result = offset;
  while (result < file_size) {
    if (!file.seek(result)) {
      return false;
    }

    const int byte = file.read();
    if (byte < 0) {
      return false;
    }
    if (!is_ascii_whitespace(static_cast<unsigned char>(byte))) {
      break;
    }
    ++result;
  }
  return true;
}

int32_t text_zone_width() {
  if (ui_TextZone != nullptr) {
    const int32_t width = static_cast<int32_t>(lv_obj_get_width(ui_TextZone));
    if (width > 0) {
      return width;
    }
  }
  return TEXT_ZONE_WIDTH;
}

int32_t text_zone_height() {
  if (ui_TextZone != nullptr) {
    const int32_t height = static_cast<int32_t>(lv_obj_get_height(ui_TextZone));
    if (height > 0) {
      return height;
    }
  }
  return TEXT_ZONE_HEIGHT;
}

bool page_fits_text(const char *text) {
  if (text == nullptr) {
    return false;
  }

  lv_point_t size{};
  lv_txt_get_size(&size, text, READING_FONT, READING_LETTER_SPACE, READING_LINE_SPACE,
                  text_zone_width(), LV_TEXT_FLAG_NONE);
  return size.y <= text_zone_height();
}

bool build_one_page(SdFile &file, FileOffset page_start, FileOffset file_size, FileOffset &page_end) {
  if (!file.seek(page_start)) {
    return false;
  }

  std::memset(pagination_buffer, 0, sizeof(pagination_buffer));
  size_t rendered_len = 0U;
  FileOffset best_end = page_start;
  FileOffset last_break_end = page_start;
  FileOffset first_unit_end = page_start;
  bool saw_unit = false;
  bool stopped_before_eof = false;

  while (static_cast<FileOffset>(file.position()) < file_size) {
    DecodedUnit unit;
    if (!read_decoded_unit(file, file_size, unit)) {
      return false;
    }
    if (unit.source_start < page_start || unit.source_end <= unit.source_start || unit.source_end > file_size) {
      return false;
    }

    const size_t available = (PAGE_BUFFER_BYTES - 1U) - rendered_len;
    if (unit.rendered_len > available) {
      stopped_before_eof = true;
      if (!saw_unit) {
        // Every decoded unit is at most four bytes, so this branch is only a
        // defensive guard for a future decoder change.
        page_end = unit.source_end;
        return page_end > page_start;
      }
      break;
    }

    if (!saw_unit) {
      first_unit_end = unit.source_end;
    }
    saw_unit = true;
    std::memcpy(pagination_buffer + rendered_len, unit.rendered, unit.rendered_len);
    rendered_len += unit.rendered_len;
    pagination_buffer[rendered_len] = '\0';

    if (page_fits_text(pagination_buffer)) {
      best_end = unit.source_end;
      if (is_breakable_unit(unit)) {
        last_break_end = unit.source_end;
      }
    } else if (best_end == page_start) {
      // A single glyph or long unbroken token can be wider than the screen;
      // force progress rather than looping forever at the same byte offset.
      page_end = first_unit_end;
      return page_end > page_start;
    } else {
      stopped_before_eof = true;
      break;
    }
  }

  if (!saw_unit) {
    return false;
  }

  page_end = (stopped_before_eof && last_break_end > page_start) ? last_break_end : best_end;
  if (page_end <= page_start) {
    page_end = first_unit_end;
  }
  return page_end > page_start && page_end <= file_size;
}

bool extend_page_table_from_file(SdFile &file, PageTable &table) {
  if (table.complete || !file || table.offsets == nullptr || table.count == 0U) {
    return table.complete;
  }

  const FileOffset page_start = table.offsets[table.count - 1U];
  // Keep pagination on one open handle. A deep bookmark can extend hundreds
  // of pages; reopening the same large file for every boundary can overload
  // the SD transaction path and leaves many more opportunities for a stale
  // card state than a single seek/read stream.
  FileOffset page_end = page_start;
  if (!build_one_page(file, page_start, table.file_size, page_end)) {
    return false;
  }

  FileOffset next_start = page_end;
  if (!skip_source_whitespace(file, page_end, table.file_size, next_start)) {
    return false;
  }
  if (next_start <= page_start) {
    next_start = page_end;
  }
  if (next_start <= page_start || next_start > table.file_size ||
      !push_page_offset(table, next_start)) {
    return false;
  }
  table.complete = next_start >= table.file_size;
  return validate_page_table(table);
}

bool extend_page_table(const char *path, PageTable &table) {
  if (table.complete || path == nullptr || table.offsets == nullptr || table.count == 0U) {
    return table.complete;
  }

  SdFile file = sdcard_open(path);
  if (!file) {
    mark_storage_failure("page-table open");
    return false;
  }
  const bool extended = extend_page_table_from_file(file, table);
  file.close();
  return extended;
}

bool ensure_page_table_page(const char *path, PageTable &table, size_t page_index) {
  if (page_index == std::numeric_limits<size_t>::max()) {
    return false;
  }
  while (table.count <= page_index + 1U) {
    if (table.complete || !extend_page_table(path, table)) {
      return false;
    }
  }
  return true;
}

bool initialize_page_table_for_path(const char *path, PageTable &table) {
  utf8_replacement_log_count = 0U;
  if (path == nullptr || path[0] == '\0') {
    return false;
  }

  SdFile file = sdcard_open(path);
  if (!file) {
    DBG_PRINT("reader: failed to open ");
    DBG_PRINTLN(path);
    mark_storage_failure("reader open");
    return false;
  }

  FileOffset file_size = 0U;
  if (!checked_file_size(file, file_size)) {
    file.close();
    return false;
  }
  table.file_size = file_size;

  FileOffset page_start = 0U;
  if (!skip_source_whitespace(file, 0U, file_size, page_start) ||
      !push_page_offset(table, page_start)) {
    file.close();
    return false;
  }

  // Empty and all-whitespace files intentionally become one blank page.
  if (page_start == file_size) {
    table.offsets[0] = 0U;
    table.complete = true;
    const bool pushed = push_page_offset(table, file_size);
    file.close();
    return pushed && validate_page_table(table);
  }

  FileOffset page_end = page_start;
  if (!build_one_page(file, page_start, file_size, page_end)) {
    file.close();
    return false;
  }

  FileOffset next_start = page_end;
  if (!skip_source_whitespace(file, page_end, file_size, next_start)) {
    file.close();
    return false;
  }
  file.close();
  if (next_start <= page_start || next_start > file_size ||
      !push_page_offset(table, next_start)) {
    return false;
  }
  table.complete = next_start >= file_size;
  return validate_page_table(table);
}

size_t trim_trailing_whitespace(char *text, size_t len) {
  if (text == nullptr) {
    return 0U;
  }

  while (len > 0U && is_ascii_whitespace(static_cast<unsigned char>(text[len - 1U]))) {
    --len;
  }
  text[len] = '\0';
  return len;
}

bool read_page_text(const char *path, const PageTable &table, size_t page_index, char *out, size_t out_size) {
  if (path == nullptr || out == nullptr || out_size == 0U || page_index + 1U >= table.count) {
    return false;
  }

  out[0] = '\0';
  const FileOffset start = table.offsets[page_index];
  const FileOffset end = table.offsets[page_index + 1U];
  if (start > end || end > table.file_size) {
    return false;
  }

  SdFile file = sdcard_open(path);
  if (!file) {
    DBG_PRINTLN("reader: failed to reopen book for page draw");
    mark_storage_failure("page draw open");
    return false;
  }
  if (static_cast<FileOffset>(file.size()) != table.file_size || !file.seek(start)) {
    DBG_PRINTLN("reader: page source changed or seek failed");
    file.close();
    mark_storage_failure("page draw seek");
    return false;
  }

  size_t rendered_len = 0U;
  while (static_cast<FileOffset>(file.position()) < end) {
    const FileOffset before = static_cast<FileOffset>(file.position());
    DecodedUnit unit;
    if (!read_decoded_unit(file, end, unit) || unit.source_start != before || unit.source_end <= before || unit.source_end > end) {
      DBG_PRINTLN("reader: failed to decode page");
      file.close();
      return false;
    }

    if (unit.rendered_len > (out_size - 1U) - rendered_len) {
      DBG_PRINTLN("reader: rendered page exceeds buffer");
      file.close();
      return false;
    }
    std::memcpy(out + rendered_len, unit.rendered, unit.rendered_len);
    rendered_len += unit.rendered_len;
  }

  const bool reached_end = static_cast<FileOffset>(file.position()) == end;
  file.close();
  if (!reached_end) {
    return false;
  }

  out[rendered_len] = '\0';
  trim_trailing_whitespace(out, rendered_len);
  return true;
}

size_t active_page_count() {
  return (page_offsets_count >= 2U) ? page_offsets_count - 1U : 0U;
}

bool ensure_active_page(size_t page_index) {
  PageTable table;
  table.offsets = page_offsets;
  table.count = page_offsets_count;
  table.capacity = page_offsets_capacity;
  table.file_size = active_book_file_size;
  table.complete = page_offsets_complete;

  const bool ensured = ensure_page_table_page(active_book_path, table, page_index);
  // extend_page_table() may realloc the offset array before a later SD or
  // validation failure. Synchronize even on failure so globals never retain a
  // pointer to memory that realloc has already released.
  page_offsets = table.offsets;
  page_offsets_count = table.count;
  page_offsets_capacity = table.capacity;
  page_offsets_complete = table.complete;
  return ensured;
}

size_t page_for_saved_offset(const PageTable &table, FileOffset saved_offset) {
  if (!validate_page_table(table) || saved_offset > table.file_size) {
    return std::numeric_limits<size_t>::max();
  }

  // A partial table cannot resolve an offset at or beyond its last known
  // boundary. The caller must extend it before asking again.
  if (!table.complete && saved_offset >= table.offsets[table.count - 1U]) {
    return std::numeric_limits<size_t>::max();
  }

  // Page endpoints are sorted. An exact endpoint resumes at that page;
  // interior offsets resume at the page containing the offset.
  size_t page = 0U;
  for (size_t i = 1U; i < table.count; ++i) {
    if (saved_offset < table.offsets[i]) {
      break;
    }
    page = i;
  }
  const size_t page_count = table.count - 1U;
  return (page < page_count) ? page : (page_count - 1U);
}

void remember_active_position() {
  if (!active_book || active_book_index >= booklist_count() || active_page >= active_page_count()) {
    return;
  }

  if (!booklist_set_saved_position(active_book_index, page_offsets[active_page], active_book_file_size)) {
    pending_bookmark_save = true;
    return;
  }
  pending_bookmark_save = !sdcard_save_bookmarks();
  if (pending_bookmark_save) {
    DBG_PRINTLN("reader: bookmark save deferred");
  }
}

void retry_pending_bookmark_save() {
  if (!pending_bookmark_save) {
    return;
  }
  if (active_book) {
    remember_active_position();
    return;
  }
  pending_bookmark_save = !sdcard_save_bookmarks();
}

bool read_active_page(size_t page_index) {
  if (!active_book || !sdcard_is_mounted() ||
      sdcard_generation() != active_sd_generation ||
      !ensure_active_page(page_index)) {
    return false;
  }

  PageTable table;
  table.offsets = page_offsets;
  table.count = page_offsets_count;
  table.capacity = page_offsets_capacity;
  table.file_size = active_book_file_size;
  table.complete = page_offsets_complete;
  return read_page_text(active_book_path, table, page_index, page_buffer, sizeof(page_buffer));
}

void update_reading_ui(const char *text, size_t page_index) {
  ensure_reading_screen();
  if (reading_text_label == nullptr) {
    reading_text_label = lv_label_create(ui_TextZone);
    lv_obj_set_width(reading_text_label, text_zone_width());
    lv_obj_set_style_text_color(reading_text_label, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(reading_text_label, READING_FONT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_long_mode(reading_text_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(reading_text_label, LV_ALIGN_TOP_LEFT, 0, 0);
  }

  lv_label_set_text(reading_text_label, (text != nullptr) ? text : "");
  lv_obj_invalidate(ui_TextZone);
  lv_obj_invalidate(ui_ReadingScreen);

  char page_text[32];
  std::snprintf(page_text, sizeof(page_text), "Page %lu", static_cast<unsigned long>(page_index + 1U));
  lv_label_set_text(ui_PageNumRead, page_text);
}

void show_book_selection_error(const char *message) {
  reader_show_book_selection_screen();
  ensure_error_label();
  lv_obj_clear_flag(error_text_label, LV_OBJ_FLAG_HIDDEN);
  lv_label_set_text(error_text_label, (message != nullptr) ? message : "Book open failed");
}

}  // namespace

void reader_show_book_selection_error(const char *message) {
  show_book_selection_error(message);
}

void reader_clear_book_selection_error() {
  clear_book_selection_error();
}

void reader_show_main_menu() {
  ensure_main_menu_screen();
  if (active_book) {
    remember_active_position();
  } else {
    retry_pending_bookmark_save();
  }
  storage_error_active = false;
  active_book = false;
  clear_active_book_content();
  screen_mode = ScreenMode::MainMenu;
  lv_disp_load_scr(ui_MainMenu);
}

void reader_show_book_selection_screen() {
  ensure_book_selection_screen();
  if (active_book) {
    remember_active_position();
    active_book = false;
    clear_active_book_content();
  } else {
    retry_pending_bookmark_save();
  }
  storage_error_active = false;
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
  if (active_book) {
    remember_active_position();
  } else {
    retry_pending_bookmark_save();
  }
  storage_error_active = true;
  active_book = false;
  clear_active_book_content();
  screen_mode = ScreenMode::MainMenu;
  lv_label_set_text(ui_time, "Insert SD card");
  lv_label_set_text(ui_date, "");
  lv_disp_load_scr(ui_MainMenu);
}

void reader_update_clock_labels(const char *time_text, const char *date_text) {
  if (time_text == nullptr || date_text == nullptr || storage_error_active) {
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
  clear_book_selection_error();
  refresh_book_selection_ui();
  return true;
}

void reader_refresh_book_selection() {
  refresh_book_selection_ui();
}

void reader_retry_bookmark_save() {
  retry_pending_bookmark_save();
}

bool reader_select_next_book() {
  const size_t count = booklist_count();
  if (count == 0U) {
    return false;
  }

  selected_book_index = (selected_book_index + 1U) % count;
  clear_book_selection_error();
  refresh_book_selection_ui();
  return true;
}

bool reader_select_prev_book() {
  const size_t count = booklist_count();
  if (count == 0U) {
    return false;
  }

  selected_book_index = (selected_book_index == 0U) ? (count - 1U) : (selected_book_index - 1U);
  clear_book_selection_error();
  refresh_book_selection_ui();
  return true;
}

bool reader_open_selected_book() {
  return reader_open_book(selected_book_index);
}

bool reader_open_book(size_t index) {
  const BookEntry *entry = booklist_get(index);
  if (entry == nullptr) {
    DBG_PRINTLN("reader: open_book invalid index");
    show_book_selection_error("Invalid book selection");
    return false;
  }

  char candidate_path[BOOK_PATH_BYTES] = {};
  if (!sdcard_canonical_book_path(entry->name, candidate_path, sizeof(candidate_path))) {
    DBG_PRINTLN("reader: invalid book path");
    show_book_selection_error("Invalid book path");
    return false;
  }

  PageTable candidate;
  if (!sdcard_is_mounted() || !initialize_page_table_for_path(candidate_path, candidate)) {
    DBG_PRINTLN("reader: failed to initialize page table");
    destroy_page_table(candidate);
    show_book_selection_error("Failed to load first page");
    return false;
  }

  const BookEntry *current_entry = booklist_get(index);
  size_t candidate_page = 0U;
  if (current_entry != nullptr && current_entry->has_saved_position) {
    if (current_entry->saved_file_size == candidate.file_size &&
        current_entry->saved_position <= candidate.file_size) {
      SdFile resume_file = sdcard_open(candidate_path);
      if (resume_file) {
        while (!candidate.complete &&
               current_entry->saved_position >= candidate.offsets[candidate.count - 1U]) {
          if (!extend_page_table_from_file(resume_file, candidate)) {
            break;
          }
        }
        resume_file.close();
      } else {
        DBG_PRINTLN("reader: failed to open book for bookmark resume");
      }
      candidate_page = page_for_saved_offset(candidate, current_entry->saved_position);
      if (candidate_page == std::numeric_limits<size_t>::max()) {
        DBG_PRINTLN("reader: saved position invalid, using first page");
        candidate_page = 0U;
      }
    } else {
      DBG_PRINTLN("reader: saved file size changed or position invalid, using first page");
    }
  }

  if (!ensure_page_table_page(candidate_path, candidate, candidate_page)) {
    DBG_PRINTLN("reader: failed to paginate candidate page");
    destroy_page_table(candidate);
    show_book_selection_error("Failed to load book page");
    return false;
  }

  // Decode the candidate page before changing any active-reader state. A
  // failed resume read therefore leaves the previous book and visible page
  // untouched until the selection error is shown.
  std::memset(candidate_page_buffer, 0, sizeof(candidate_page_buffer));
  if (!read_page_text(candidate_path, candidate, candidate_page,
                      candidate_page_buffer, sizeof(candidate_page_buffer))) {
    DBG_PRINTLN("reader: failed to read candidate page");
    destroy_page_table(candidate);
    show_book_selection_error("Failed to read book page");
    return false;
  }

  std::free(page_offsets);
  page_offsets = candidate.offsets;
  page_offsets_count = candidate.count;
  page_offsets_capacity = candidate.capacity;
  page_offsets_complete = candidate.complete;
  active_book_file_size = candidate.file_size;
  active_sd_generation = sdcard_generation();
  candidate.offsets = nullptr;
  candidate.count = 0U;
  candidate.capacity = 0U;
  candidate.file_size = 0U;

  std::snprintf(active_book_path, sizeof(active_book_path), "%s", candidate_path);
  std::memcpy(page_buffer, candidate_page_buffer, sizeof(page_buffer));
  selected_book_index = index;
  active_book_index = index;
  active_page = candidate_page;
  active_book = true;
  pending_bookmark_save = false;
  clear_book_selection_error();

  reader_show_reading_screen();
  update_reading_ui(page_buffer, active_page);
  // Persist the displayed page only after the candidate read and state swap
  // succeed; this also replaces any stale saved record after a fallback open.
  remember_active_position();
  return true;
}

bool reader_next_page() {
  if (!active_book || !sdcard_is_mounted() || active_page == std::numeric_limits<size_t>::max()) {
    return false;
  }

  const size_t target_page = active_page + 1U;
  if (!ensure_active_page(target_page)) {
    return false;
  }

  DBG_PRINT("reader: next page from ");
  DBG_PRINTLN(static_cast<unsigned long>(active_page));
  if (!read_active_page(target_page)) {
    DBG_PRINTLN("reader: next page failed");
    return false;
  }

  active_page = target_page;
  update_reading_ui(page_buffer, active_page);
  remember_active_position();
  DBG_PRINT("reader: now on page ");
  DBG_PRINTLN(static_cast<unsigned long>(active_page));
  return true;
}

bool reader_prev_page() {
  if (!active_book || !sdcard_is_mounted() || active_page == 0U || active_page >= active_page_count()) {
    return false;
  }

  const size_t target_page = active_page - 1U;
  DBG_PRINT("reader: prev page from ");
  DBG_PRINTLN(static_cast<unsigned long>(active_page));
  if (!read_active_page(target_page)) {
    DBG_PRINTLN("reader: prev page failed");
    return false;
  }

  active_page = target_page;
  update_reading_ui(page_buffer, active_page);
  remember_active_position();
  DBG_PRINT("reader: now on page ");
  DBG_PRINTLN(static_cast<unsigned long>(active_page));
  return true;
}

void reader_show_menu_on_boot() {
  reader_show_book_selection_screen();
}
