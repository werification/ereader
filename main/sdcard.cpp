#include "sdcard.h"

#include <SD.h>
#include <SPI.h>
#include <cstring>
#include <limits>
#include <utility>

#include "booklist.h"
#include "config.h"
#include "utils.h"
#include "spi_bus.h"

SdFile::SdFile(fs::File file) : file_(file) {}

SdFile::~SdFile() {
  close();
}

SdFile::SdFile(SdFile &&other) noexcept : file_(other.file_) {
  // fs::File copies share the same FileImpl. Do not close the source after
  // copying: that would close the handle now owned by this wrapper as well.
  other.file_ = fs::File();
}

SdFile &SdFile::operator=(SdFile &&other) noexcept {
  if (this != &other) {
    close();
    file_ = other.file_;
    // Leave the source wrapper empty without closing the shared FileImpl.
    other.file_ = fs::File();
  }
  return *this;
}

SdFile::operator bool() const {
  return static_cast<bool>(file_);
}

bool SdFile::operator!() const {
  return !static_cast<bool>(file_);
}

void SdFile::close() {
  file_.close();
}

bool SdFile::isDirectory() {
  return static_cast<bool>(file_) && file_.isDirectory();
}

const char *SdFile::name() const {
  return file_.name();
}

const char *SdFile::path() const {
  return file_.path();
}

SdFile SdFile::openNextFile() {
  if (!sdcard_is_mounted() || !spi_bus_acquire(SpiDevice::Sd)) {
    return SdFile();
  }
  SdFile next(file_.openNextFile());
  spi_bus_release(SpiDevice::Sd);
  return next;
}

size_t SdFile::size() const {
  if (!sdcard_is_mounted() || !spi_bus_acquire(SpiDevice::Sd)) {
    return 0U;
  }
  const size_t result = file_.size();
  spi_bus_release(SpiDevice::Sd);
  return result;
}

size_t SdFile::position() const {
  if (!sdcard_is_mounted() || !spi_bus_acquire(SpiDevice::Sd)) {
    return static_cast<size_t>(-1);
  }
  const size_t result = file_.position();
  spi_bus_release(SpiDevice::Sd);
  return result;
}

bool SdFile::seek(uint32_t offset) {
  if (!sdcard_is_mounted() || !spi_bus_acquire(SpiDevice::Sd)) {
    return false;
  }
  const bool result = file_.seek(offset);
  spi_bus_release(SpiDevice::Sd);
  return result;
}

int SdFile::read() {
  if (!sdcard_is_mounted() || !spi_bus_acquire(SpiDevice::Sd)) {
    return -1;
  }
  const int result = file_.read();
  spi_bus_release(SpiDevice::Sd);
  return result;
}

int SdFile::peek() {
  if (!sdcard_is_mounted() || !spi_bus_acquire(SpiDevice::Sd)) {
    return -1;
  }
  const int result = file_.peek();
  spi_bus_release(SpiDevice::Sd);
  return result;
}

size_t SdFile::readBytes(char *buffer, size_t length) {
  if (!sdcard_is_mounted() || buffer == nullptr || !spi_bus_acquire(SpiDevice::Sd)) {
    return 0U;
  }
  const size_t result = file_.readBytes(buffer, length);
  spi_bus_release(SpiDevice::Sd);
  return result;
}

size_t SdFile::read(uint8_t *buffer, size_t length) {
  if (!sdcard_is_mounted() || buffer == nullptr || !spi_bus_acquire(SpiDevice::Sd)) {
    return 0U;
  }
  const size_t result = file_.read(buffer, length);
  spi_bus_release(SpiDevice::Sd);
  return result;
}

size_t SdFile::write(const uint8_t *buffer, size_t length) {
  if (!sdcard_is_mounted() || buffer == nullptr || !spi_bus_acquire(SpiDevice::Sd)) {
    return 0U;
  }
  const size_t result = file_.write(buffer, length);
  spi_bus_release(SpiDevice::Sd);
  return result;
}

void SdFile::flush() {
  if (!sdcard_is_mounted() || !spi_bus_acquire(SpiDevice::Sd)) {
    return;
  }
  file_.flush();
  spi_bus_release(SpiDevice::Sd);
}

namespace {
constexpr char BOOKS_PREFIX[] = "/books/";
constexpr size_t BOOKS_PREFIX_LEN = sizeof(BOOKS_PREFIX) - 1U;
constexpr char BOOKMARK_DIR[] = "/.bookmarks";
constexpr char BOOKMARK_SLOT_A[] = "/.bookmarks/positions.0";
constexpr char BOOKMARK_SLOT_B[] = "/.bookmarks/positions.1";
constexpr char BOOKMARK_TEMP[] = "/.bookmarks/positions.tmp";
constexpr uint32_t BOOKMARK_MAGIC = 0x52424B31UL;  // RBK1
constexpr uint16_t BOOKMARK_VERSION = 1U;
constexpr size_t BOOKMARK_HEADER_BYTES = 16U;
constexpr size_t BOOKMARK_RECORD_BYTES = BOOKLIST_MAX_NAME_LEN + 8U;
constexpr size_t BOOKMARK_MAX_RECORDS = BOOKLIST_MAX_BOOKS;
bool sd_session_active = false;
uint32_t sd_generation = 0U;

bool append_component(char *out, size_t out_size, size_t &out_length,
                      const char *component, size_t component_length) {
  if (component_length == 0U) {
    return true;
  }
  if ((component_length == 1U && component[0] == '.') ||
      (component_length == 2U && component[0] == '.' && component[1] == '.')) {
    return false;
  }

  const size_t separator_length = (out_length > BOOKS_PREFIX_LEN) ? 1U : 0U;
  if (out_length > out_size || separator_length > out_size - out_length) {
    return false;
  }

  const size_t remaining = out_size - out_length - separator_length;
  if (remaining == 0U || component_length > remaining - 1U) {
    return false;
  }

  if (separator_length != 0U) {
    out[out_length++] = '/';
  }
  std::memcpy(out + out_length, component, component_length);
  out_length += component_length;
  out[out_length] = '\0';
  return true;
}

void prepare_spi_bus() {
  spi_bus_init();
}

bool sd_mkdir(const char *path) {
  if (!sd_session_active || !spi_bus_acquire(SpiDevice::Sd)) {
    return false;
  }
  const bool result = SD.mkdir(path);
  spi_bus_release(SpiDevice::Sd);
  return result;
}

bool sd_remove(const char *path) {
  if (!sd_session_active || !spi_bus_acquire(SpiDevice::Sd)) {
    return false;
  }
  const bool result = SD.remove(path);
  spi_bus_release(SpiDevice::Sd);
  return result;
}

bool sd_rename(const char *from, const char *to) {
  if (!sd_session_active || !spi_bus_acquire(SpiDevice::Sd)) {
    return false;
  }
  const bool result = SD.rename(from, to);
  spi_bus_release(SpiDevice::Sd);
  return result;
}

uint32_t read_u32(const uint8_t *data) {
  return static_cast<uint32_t>(data[0]) |
         (static_cast<uint32_t>(data[1]) << 8U) |
         (static_cast<uint32_t>(data[2]) << 16U) |
         (static_cast<uint32_t>(data[3]) << 24U);
}

uint16_t read_u16(const uint8_t *data) {
  return static_cast<uint16_t>(data[0]) |
         static_cast<uint16_t>(static_cast<uint16_t>(data[1]) << 8U);
}

void write_u32(uint8_t *data, uint32_t value) {
  data[0] = static_cast<uint8_t>(value & 0xFFU);
  data[1] = static_cast<uint8_t>((value >> 8U) & 0xFFU);
  data[2] = static_cast<uint8_t>((value >> 16U) & 0xFFU);
  data[3] = static_cast<uint8_t>((value >> 24U) & 0xFFU);
}

void write_u16(uint8_t *data, uint16_t value) {
  data[0] = static_cast<uint8_t>(value & 0xFFU);
  data[1] = static_cast<uint8_t>((value >> 8U) & 0xFFU);
}

uint32_t crc32(const uint8_t *data, size_t length) {
  uint32_t crc = 0xFFFFFFFFUL;
  for (size_t i = 0U; i < length; ++i) {
    crc ^= data[i];
    for (uint8_t bit = 0U; bit < 8U; ++bit) {
      crc = (crc >> 1U) ^ (0xEDB88320UL & static_cast<uint32_t>(-(crc & 1U)));
    }
  }
  return ~crc;
}

bool read_exact(SdFile &file, uint8_t *buffer, size_t length) {
  size_t offset = 0U;
  while (offset < length) {
    const size_t read_count = file.read(buffer + offset, length - offset);
    if (read_count == 0U) {
      return false;
    }
    offset += read_count;
  }
  return true;
}

bool path_is_book(const char *path) {
  char canonical[BOOKLIST_MAX_NAME_LEN] = {};
  return sdcard_canonical_book_path(path, canonical, sizeof(canonical)) &&
         std::strcmp(path, canonical) == 0;
}

struct BookmarkRecord {
  char path[BOOKLIST_MAX_NAME_LEN] = {};
  uint32_t position = 0U;
  uint32_t file_size = 0U;
};

struct BookmarkSnapshot {
  bool valid = false;
  uint32_t generation = 0U;
  size_t count = 0U;
  BookmarkRecord records[BOOKMARK_MAX_RECORDS] = {};
};

bool generation_is_newer(uint32_t candidate, uint32_t reference) {
  // Serial-number arithmetic keeps generation rollover deterministic while
  // treating zero as reserved for an invalid/unwritten snapshot.
  return candidate != reference && static_cast<int32_t>(candidate - reference) > 0;
}

const BookmarkSnapshot *newest_snapshot(const BookmarkSnapshot &a,
                                         bool valid_a,
                                         const BookmarkSnapshot &b,
                                         bool valid_b) {
  if (!valid_a) {
    return valid_b ? &b : nullptr;
  }
  if (!valid_b || generation_is_newer(a.generation, b.generation)) {
    return &a;
  }
  return &b;
}

bool read_bookmark_slot(const char *path, BookmarkSnapshot &snapshot) {
  snapshot = BookmarkSnapshot{};
  SdFile file = sdcard_open(path);
  if (!file) {
    return false;
  }

  const size_t file_size = file.size();
  if (file_size < BOOKMARK_HEADER_BYTES ||
      file_size > BOOKMARK_HEADER_BYTES + BOOKMARK_MAX_RECORDS * BOOKMARK_RECORD_BYTES) {
    file.close();
    return false;
  }

  uint8_t header[BOOKMARK_HEADER_BYTES] = {};
  if (!read_exact(file, header, sizeof(header))) {
    file.close();
    return false;
  }

  const uint32_t magic = read_u32(header);
  const uint16_t version = read_u16(header + 4U);
  const uint16_t count = read_u16(header + 6U);
  snapshot.generation = read_u32(header + 8U);
  const uint32_t stored_crc = read_u32(header + 12U);
  const size_t expected_size = BOOKMARK_HEADER_BYTES + static_cast<size_t>(count) * BOOKMARK_RECORD_BYTES;
  if (magic != BOOKMARK_MAGIC || version != BOOKMARK_VERSION || count > BOOKMARK_MAX_RECORDS ||
      snapshot.generation == 0U || expected_size != file_size) {
    file.close();
    return false;
  }

  uint8_t payload[BOOKMARK_MAX_RECORDS * BOOKMARK_RECORD_BYTES] = {};
  if (!read_exact(file, payload, static_cast<size_t>(count) * BOOKMARK_RECORD_BYTES)) {
    file.close();
    return false;
  }
  file.close();

  uint8_t crc_input[BOOKMARK_MAX_RECORDS * BOOKMARK_RECORD_BYTES + 12U] = {};
  std::memcpy(crc_input, header, 12U);
  std::memcpy(crc_input + 12U, payload, static_cast<size_t>(count) * BOOKMARK_RECORD_BYTES);
  if (crc32(crc_input, 12U + static_cast<size_t>(count) * BOOKMARK_RECORD_BYTES) != stored_crc) {
    return false;
  }

  for (size_t record_index = 0U; record_index < count; ++record_index) {
    const uint8_t *record = payload + record_index * BOOKMARK_RECORD_BYTES;
    BookmarkRecord &decoded = snapshot.records[record_index];
    std::memcpy(decoded.path, record, BOOKLIST_MAX_NAME_LEN);
    if (decoded.path[BOOKLIST_MAX_NAME_LEN - 1U] != '\0' || !path_is_book(decoded.path)) {
      return false;
    }
    decoded.position = read_u32(record + BOOKLIST_MAX_NAME_LEN);
    decoded.file_size = read_u32(record + BOOKLIST_MAX_NAME_LEN + 4U);
    if (decoded.position > decoded.file_size) {
      return false;
    }
    for (size_t previous = 0U; previous < record_index; ++previous) {
      if (std::strcmp(snapshot.records[previous].path, decoded.path) == 0) {
        return false;
      }
    }
  }

  snapshot.count = count;
  snapshot.valid = true;
  return true;
}

void apply_bookmark_snapshot(const BookmarkSnapshot &snapshot) {
  if (!snapshot.valid) {
    return;
  }

  for (size_t record_index = 0U; record_index < snapshot.count; ++record_index) {
    const BookmarkRecord &record = snapshot.records[record_index];
    for (size_t i = 0U; i < booklist_count(); ++i) {
      const BookEntry *entry = booklist_get(i);
      if (entry == nullptr || std::strcmp(entry->name, record.path) != 0) {
        continue;
      }

      SdFile book = sdcard_open(record.path);
      if (book && static_cast<uint64_t>(book.size()) == record.file_size) {
        book.close();
        booklist_set_saved_position(i, record.position, record.file_size);
      } else if (book) {
        book.close();
      }
      break;
    }
  }
}

}  // namespace

SdFile sdcard_open_write(const char *path) {
  if (path == nullptr || !sdcard_is_mounted() || !spi_bus_acquire(SpiDevice::Sd)) {
    return SdFile();
  }
  SdFile file(SD.open(path, FILE_WRITE));
  spi_bus_release(SpiDevice::Sd);
  return file;
}

bool sdcard_canonical_book_path(const char *entry_name, char *out, size_t out_size) {
  if (entry_name == nullptr || out == nullptr || out_size <= BOOKS_PREFIX_LEN) {
    return false;
  }

  out[0] = '\0';
  std::memcpy(out, BOOKS_PREFIX, BOOKS_PREFIX_LEN);
  out[BOOKS_PREFIX_LEN] = '\0';

  const size_t input_length = std::strlen(entry_name);
  size_t input_start = 0U;
  while (input_start < input_length && entry_name[input_start] == '/') {
    ++input_start;
  }

  if (input_length - input_start >= 6U &&
      std::strncmp(entry_name + input_start, "books/", 6U) == 0) {
    input_start += 6U;
  }

  size_t out_length = BOOKS_PREFIX_LEN;
  size_t component_start = input_start;
  for (size_t i = input_start; i <= input_length; ++i) {
    if (i != input_length && entry_name[i] != '/') {
      continue;
    }

    if (!append_component(out, out_size, out_length,
                          entry_name + component_start, i - component_start)) {
      out[0] = '\0';
      return false;
    }
    component_start = i + 1U;
  }

  if (out_length == BOOKS_PREFIX_LEN) {
    out[0] = '\0';
    return false;
  }
  return true;
}

SdFile sdcard_open(const char *path) {
  if (path == nullptr) {
    DBG_PRINTLN("SD_OPEN_SKIP null path");
    return SdFile();
  }
  if (!sd_session_active) {
    DBG_PRINT("SD_OPEN_SKIP unavailable path=");
    DBG_PRINTLN(path);
    return SdFile();
  }
  if (!spi_bus_acquire(SpiDevice::Sd)) {
    DBG_PRINT("SD_OPEN_SKIP owner path=");
    DBG_PRINTLN(path);
    return SdFile();
  }

  DBG_PRINT("SD_OPEN_BEGIN path=");
  DBG_PRINTLN(path);
  // Match the reference project's access pattern: open the logical Arduino
  // path directly and use the returned handle as the existence check. An
  // extra SD.exists() call is unnecessary and adds another filesystem
  // transaction immediately before the real open.
  SdFile file(SD.open(path, FILE_READ));
  bool valid = static_cast<bool>(file);

  // Older ESP32 FS releases accepted mount-relative paths while newer
  // releases require a leading slash. Keep a narrow compatibility fallback
  // for cards whose original firmware stored paths as "books/foo.txt"; the
  // canonical path remains /books/foo.txt everywhere outside this function.
  if (!valid && std::strncmp(path, "/books/", BOOKS_PREFIX_LEN) == 0) {
    const char *legacy_path = path + 1U;
    DBG_PRINT("SD_OPEN_FALLBACK path=");
    DBG_PRINTLN(legacy_path);
    file = SdFile(SD.open(legacy_path, FILE_READ));
    valid = static_cast<bool>(file);
  }

  DBG_PRINT("SD_OPEN_RESULT path=");
  DBG_PRINT(path);
  DBG_PRINT(" valid=");
  DBG_PRINT(valid ? 1 : 0);
  DBG_PRINT(" handle=");
  DBG_PRINTLN(valid && file.path() != nullptr ? file.path() : "(none)");
  spi_bus_release(SpiDevice::Sd);
  return file;
}

bool sdcard_exists(const char *path) {
  if (path == nullptr || !sd_session_active || !spi_bus_acquire(SpiDevice::Sd)) {
    return false;
  }
  const bool exists = SD.exists(path);
  spi_bus_release(SpiDevice::Sd);
  return exists;
}

bool sdcard_is_mounted() {
  return sd_session_active;
}

void sdcard_mark_unavailable(const char *operation) {
  if (!sd_session_active) {
    return;
  }
  sd_session_active = false;
  ++sd_generation;
  DBG_PRINT("SD unavailable during ");
  DBG_PRINTLN(operation != nullptr ? operation : "unknown operation");
  DBG_PRINT("SD generation=");
  DBG_PRINTLN(static_cast<unsigned long>(sd_generation));
}

uint32_t sdcard_generation() {
  return sd_generation;
}

void sdcard_release_for_display() {
  // Kept as a compatibility no-op. Display refreshes must not tear down the
  // mounted SD session; they only quiesce the logical bus owner.
  spi_bus_quiesce();
}

bool sdcard_recover() {
  DBG_PRINTLN("SD_RECOVERY_BEGIN");
  spi_bus_quiesce();
  // Always clear the Arduino SD driver's cached mount state. A prior I/O
  // failure may have marked the session unavailable without calling SD.end().
  SD.end();
  sd_session_active = false;
  DBG_PRINTLN("SD_RECOVERY_UNMOUNTED");
  DBG_PRINTLN("SD_MOUNT_BEGIN recovery");
  if (!spi_bus_acquire(SpiDevice::Sd)) {
    DBG_PRINTLN("SD_MOUNT_RESULT=0 owner");
    return false;
  }
  DBG_PRINT("SD_BEGIN_CALL_BEGIN recovery ms=");
  DBG_PRINTLN(millis());
  const bool mounted = SD.begin(PIN_SD_CS, SPI, 4000000U);
  DBG_PRINT("SD_BEGIN_CALL_END recovery ms=");
  DBG_PRINTLN(millis());
  spi_bus_release(SpiDevice::Sd);
  DBG_PRINT("SD_MOUNT_RESULT=");
  DBG_PRINTLN(mounted ? 1 : 0);
  DBG_PRINT("SD_BEGIN_RESULT=");
  DBG_PRINTLN(mounted ? 1 : 0);
  if (!mounted) {
    SD.end();
    return false;
  }
  sd_session_active = true;
  DBG_PRINTLN("SD_PROBE_BEGIN recovery /books");
  SdFile books = sdcard_open("/books");
  const bool valid = static_cast<bool>(books);
  books.close();
  DBG_PRINT("SD_PROBE_RESULT recovery=");
  DBG_PRINTLN(valid ? 1 : 0);
  DBG_PRINT("SD_BOOKS_OPEN=");
  DBG_PRINTLN(valid ? 1 : 0);
  if (!valid) {
    SD.end();
    sd_session_active = false;
    return false;
  }
  ++sd_generation;
  DBG_PRINT("SD recovery succeeded generation=");
  DBG_PRINTLN(static_cast<unsigned long>(sd_generation));
  return true;
}

bool sdcard_remount() {
  // Legacy entry point: explicit recovery is now the only remount path.
  return sdcard_recover();
}

bool sdcard_init() {
  prepare_spi_bus();
  DBG_PRINTLN("SD_MOUNT_BEGIN boot");
  if (!spi_bus_acquire(SpiDevice::Sd)) {
    DBG_PRINTLN("SD_MOUNT_RESULT=0 owner");
    return false;
  }

  DBG_PRINTLN("SD: trying 4 MHz");
  DBG_PRINT("SD_BEGIN_CALL_BEGIN boot ms=");
  DBG_PRINTLN(millis());
  bool mounted = SD.begin(PIN_SD_CS, SPI, 4000000U);
  DBG_PRINT("SD_BEGIN_CALL_END boot ms=");
  DBG_PRINTLN(millis());
  spi_bus_release(SpiDevice::Sd);
  DBG_PRINT("SD_MOUNT_RESULT=");
  DBG_PRINTLN(mounted ? 1 : 0);
  DBG_PRINT("SD_BEGIN_RESULT=");
  DBG_PRINTLN(mounted ? 1 : 0);

  if (!mounted) {
    DBG_PRINTLN("SD: 4 MHz failed, trying 1 MHz");
    SD.end();
    if (!spi_bus_acquire(SpiDevice::Sd)) {
      return false;
    }
    DBG_PRINT("SD_BEGIN_CALL_BEGIN fallback ms=");
    DBG_PRINTLN(millis());
    mounted = SD.begin(PIN_SD_CS, SPI, 1000000U);
    DBG_PRINT("SD_BEGIN_CALL_END fallback ms=");
    DBG_PRINTLN(millis());
    spi_bus_release(SpiDevice::Sd);
    DBG_PRINT("SD_MOUNT_RESULT fallback=");
    DBG_PRINTLN(mounted ? 1 : 0);
    DBG_PRINT("SD_BEGIN_RESULT=");
    DBG_PRINTLN(mounted ? 1 : 0);
  }

  if (!mounted) {
    SD.end();
    DBG_PRINTLN("SD: mount failed");
    return false;
  }

  sd_session_active = true;
  ++sd_generation;
  DBG_PRINTLN("SD_PROBE_BEGIN boot /books");
  SdFile books = sdcard_open("/books");
  const bool valid = static_cast<bool>(books);
  books.close();
  DBG_PRINT("SD_PROBE_RESULT boot=");
  DBG_PRINTLN(valid ? 1 : 0);
  DBG_PRINT("SD_BOOKS_OPEN=");
  DBG_PRINTLN(valid ? 1 : 0);
  if (!valid) {
    SD.end();
    sd_session_active = false;
    return false;
  }
  DBG_PRINT("SD generation=");
  DBG_PRINTLN(static_cast<unsigned long>(sd_generation));
  return true;
}

void sdcard_scan_books() {
  booklist_clear();

  SdFile root = sdcard_open("/books");
  if (!root) {
    DBG_PRINTLN("books directory not found");
    return;
  }

  while (true) {
    SdFile entry = root.openNextFile();
    if (!entry) {
      break;
    }

    if (!entry.isDirectory()) {
      const char *raw_name = entry.name();
      // File::name() is the basename on the ESP32 VFS implementation. Build
      // the logical path from the directory we opened, matching the Arduino
      // SD usage that this project previously relied on. Do not use
      // File::path() as a source of truth: it is an implementation detail and
      // can be null or vary across FS backends.
      DBG_PRINT("SD entry name: ");
      DBG_PRINTLN(raw_name != nullptr ? raw_name : "(null)");
      DBG_PRINT("SD entry path: /books/");
      DBG_PRINTLN(raw_name != nullptr ? raw_name : "(null)");

      String name = raw_name != nullptr ? raw_name : "";
      if (ends_with_ignore_case(name, ".txt")) {
        char full_path[BOOKLIST_MAX_NAME_LEN] = {};
        String scanned_path = String("/books/") + name;
        if (!sdcard_canonical_book_path(scanned_path.c_str(), full_path, sizeof(full_path))) {
          DBG_PRINTLN("SD: rejected unsafe or overlong book path");
        } else {
          DBG_PRINT("Book canonical path: ");
          DBG_PRINTLN(full_path);
          SdFile probe = sdcard_open(full_path);
          const bool probe_valid = static_cast<bool>(probe);
          probe.close();
          DBG_PRINT("SD_SCAN_FILE_PROBE path=");
          DBG_PRINT(full_path);
          DBG_PRINT(" valid=");
          DBG_PRINTLN(probe_valid ? 1 : 0);
          if (!probe_valid) {
            DBG_PRINTLN("SD: scanned entry is not reopenable by canonical path; skipping");
          } else if (!booklist_add(full_path)) {
            DBG_PRINTLN("SD: book list full or path rejected");
          } else {
            DBG_PRINT("Book count now: ");
            DBG_PRINTLN(static_cast<unsigned long>(booklist_count()));
          }
        }
      }
    }

    entry.close();
  }

  root.close();
  sdcard_load_bookmarks();
}

void sdcard_load_bookmarks() {
  if (!sdcard_exists(BOOKMARK_DIR) && !sd_mkdir(BOOKMARK_DIR)) {
    DBG_PRINTLN("SD: bookmark directory unavailable");
    return;
  }

  BookmarkSnapshot snapshot_a;
  BookmarkSnapshot snapshot_b;
  const bool valid_a = read_bookmark_slot(BOOKMARK_SLOT_A, snapshot_a);
  const bool valid_b = read_bookmark_slot(BOOKMARK_SLOT_B, snapshot_b);
  if (!valid_a && !valid_b) {
    DBG_PRINTLN("SD: no valid bookmark snapshot");
    return;
  }

  const BookmarkSnapshot *newest =
      newest_snapshot(snapshot_a, valid_a, snapshot_b, valid_b);
  if (newest == nullptr) {
    DBG_PRINTLN("SD: no valid bookmark snapshot");
    return;
  }
  apply_bookmark_snapshot(*newest);
  DBG_PRINT("SD: loaded bookmark snapshot generation ");
  DBG_PRINTLN(static_cast<unsigned long>(newest->generation));
}

bool sdcard_save_bookmarks() {
  if (!sdcard_exists(BOOKMARK_DIR) && !sd_mkdir(BOOKMARK_DIR)) {
    DBG_PRINTLN("SD: cannot create bookmark directory");
    return false;
  }

  BookmarkSnapshot snapshot_a;
  BookmarkSnapshot snapshot_b;
  const bool valid_a = read_bookmark_slot(BOOKMARK_SLOT_A, snapshot_a);
  const bool valid_b = read_bookmark_slot(BOOKMARK_SLOT_B, snapshot_b);
  const BookmarkSnapshot *newest_snapshot_ptr =
      newest_snapshot(snapshot_a, valid_a, snapshot_b, valid_b);
  const uint32_t newest = newest_snapshot_ptr == nullptr ? 0U : newest_snapshot_ptr->generation;
  const uint32_t generation = (newest == std::numeric_limits<uint32_t>::max()) ? 1U : newest + 1U;

  // A slot is selected only after the complete temporary snapshot exists. The
  // older valid slot is retained until rename succeeds, so a failed write
  // cannot erase the last known-good snapshot.
  uint8_t header[BOOKMARK_HEADER_BYTES] = {};
  uint8_t payload[BOOKMARK_MAX_RECORDS * BOOKMARK_RECORD_BYTES] = {};
  write_u32(header, BOOKMARK_MAGIC);
  write_u16(header + 4U, BOOKMARK_VERSION);
  write_u32(header + 8U, generation);

  size_t saved_count = 0U;
  for (size_t i = 0U; i < booklist_count(); ++i) {
    const BookEntry *entry = booklist_get(i);
    if (entry == nullptr) {
      return false;
    }
    if (!entry->has_saved_position) {
      continue;
    }
    if (saved_count >= BOOKMARK_MAX_RECORDS) {
      return false;
    }
    uint8_t *record = payload + saved_count * BOOKMARK_RECORD_BYTES;
    std::memcpy(record, entry->name, BOOKLIST_MAX_NAME_LEN);
    write_u32(record + BOOKLIST_MAX_NAME_LEN, entry->saved_position);
    write_u32(record + BOOKLIST_MAX_NAME_LEN + 4U, entry->saved_file_size);
    ++saved_count;
  }
  write_u16(header + 6U, static_cast<uint16_t>(saved_count));
  const size_t saved_payload_bytes = saved_count * BOOKMARK_RECORD_BYTES;

  // CRC covers the version, count, generation, and payload, not the CRC field.

  uint8_t crc_input[BOOKMARK_MAX_RECORDS * BOOKMARK_RECORD_BYTES + 12U] = {};
  std::memcpy(crc_input, header, 12U);
  std::memcpy(crc_input + 12U, payload, saved_payload_bytes);
  write_u32(header + 12U, crc32(crc_input, 12U + saved_payload_bytes));

  sd_remove(BOOKMARK_TEMP);
  SdFile temp = sdcard_open_write(BOOKMARK_TEMP);
  if (!temp) {
    DBG_PRINTLN("SD: cannot open bookmark temporary file");
    return false;
  }
  const bool wrote = temp.write(header, sizeof(header)) == sizeof(header) &&
                     temp.write(payload, saved_payload_bytes) == saved_payload_bytes;
  temp.flush();
  temp.close();
  if (!wrote) {
    DBG_PRINTLN("SD: bookmark temporary write failed");
    sd_remove(BOOKMARK_TEMP);
    return false;
  }

  const char *target = !valid_a ? BOOKMARK_SLOT_A
                               : (!valid_b ? BOOKMARK_SLOT_B
                                            : (generation_is_newer(snapshot_a.generation, snapshot_b.generation)
                                                   ? BOOKMARK_SLOT_B
                                                   : BOOKMARK_SLOT_A));
  // Remove only the older slot after the temporary file has been closed. The
  // newest valid snapshot is never touched by a failed replacement.

  sd_remove(target);
  if (!sd_rename(BOOKMARK_TEMP, target)) {
    DBG_PRINTLN("SD: bookmark rename failed");
    sd_remove(BOOKMARK_TEMP);
    return false;
  }
  DBG_PRINT("SD: saved bookmark generation ");
  DBG_PRINTLN(static_cast<unsigned long>(generation));
  return true;
}
