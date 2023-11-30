#include "../include/stamen.h"
#include "../include/stamenc.h"

using namespace stamen;

const Menu::display_f Menu::display = nullptr;

void stamen_read(const char *file) { stamen::Menu::read(file); }
void stamen_print(const char *entry) { stamen::Menu::print(entry); }
void stamen_insert(const char *code, callback_f callback) {
  Menu::insert(code, callback);
}

int stamen_builtin_display(const char *title, const item_t items[], int size) {
  return builtinDisplay(title, items, size);
}
