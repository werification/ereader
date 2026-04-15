#include "utils.h"

bool ends_with_ignore_case(const String &value, const char *suffix) {
  const size_t value_len = value.length();
  const size_t suffix_len = strlen(suffix);
  if (value_len < suffix_len) {
    return false;
  }

  for (size_t i = 0; i < suffix_len; ++i) {
    char a = value[value_len - suffix_len + i];
    char b = suffix[i];
    if (a >= 'A' && a <= 'Z') a = static_cast<char>(a - 'A' + 'a');
    if (b >= 'A' && b <= 'Z') b = static_cast<char>(b - 'A' + 'a');
    if (a != b) {
      return false;
    }
  }

  return true;
}
