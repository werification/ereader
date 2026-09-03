#pragma once

#include <Arduino.h>
#include <FS.h>

class SdFile {
 public:
  SdFile() = default;
  ~SdFile();

  SdFile(const SdFile &) = delete;
  SdFile &operator=(const SdFile &) = delete;
  SdFile(SdFile &&other) noexcept;
  SdFile &operator=(SdFile &&other) noexcept;

  explicit operator bool() const;
  bool operator!() const;
  void close();
  bool isDirectory();
  const char *name() const;
  const char *path() const;
  SdFile openNextFile();
  size_t size() const;
  size_t position() const;
  bool seek(uint32_t offset);
  int read();
  int peek();
  size_t readBytes(char *buffer, size_t length);
  size_t read(uint8_t *buffer, size_t length);
  size_t write(const uint8_t *buffer, size_t length);
  void flush();

 private:
  explicit SdFile(fs::File file);
  fs::File file_;
  friend SdFile sdcard_open(const char *path);
  friend SdFile sdcard_open_write(const char *path);
};

bool sdcard_init();
bool sdcard_recover();
bool sdcard_is_mounted();
void sdcard_mark_unavailable(const char *operation);
uint32_t sdcard_generation();
void sdcard_release_for_display();
bool sdcard_remount();
SdFile sdcard_open(const char *path);
SdFile sdcard_open_write(const char *path);
bool sdcard_exists(const char *path);
void sdcard_scan_books();
void sdcard_load_bookmarks();
bool sdcard_save_bookmarks();

// Convert a directory entry returned by the SD library to the absolute path
// used by every reader operation. Returns false for unsafe or overlong paths.
bool sdcard_canonical_book_path(const char *entry_name, char *out, size_t out_size);
