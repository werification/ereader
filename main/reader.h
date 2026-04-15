#pragma once

#include <Arduino.h>

void reader_show_main_menu();
void reader_show_book_selection_screen();
void reader_show_reading_screen();
void reader_show_storage_error();
void reader_set_book_selection_empty_state();

bool reader_select_next_book();
bool reader_select_prev_book();
bool reader_open_selected_book();
bool reader_next_page();
bool reader_prev_page();
bool reader_has_active_book();
bool reader_has_selected_book();
bool reader_is_on_main_menu();
bool reader_is_on_book_selection();
bool reader_is_on_reading_screen();
size_t reader_selected_book_index();
bool reader_select_book(size_t index);
bool reader_open_book(size_t index);
void reader_refresh_book_selection();
void reader_update_clock_labels(const char *time_text, const char *date_text);
void reader_update_battery_label(const char *battery_text);
void reader_show_menu_on_boot();
























































