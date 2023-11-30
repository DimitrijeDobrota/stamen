#include "../include/stamenc.h"
#include "../include/stamen.h"

using namespace stamen;

void stamen_read(const char *file) { stamen::Menu::read(file); }
void stamen_print(const char *entry) { stamen::Menu::print(entry); }
void stamen_insert(const char *code, stamen_callback_f callback) {
  Menu::insert(code, callback);
}

int stamen_builtin_display(const char *title, const item_t items[], int size) {
  return builtinDisplay(title, items, size);
}
