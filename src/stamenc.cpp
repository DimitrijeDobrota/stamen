#include "../include/stamen.h"

using namespace stamen;

const Menu::display_f Menu::display = nullptr;

void stamen_read(const char *file) { stamen::Menu::read(file); }
void stamen_print(const char *entry) { stamen::Menu::print(entry); }
int stamen_start(const char *entry) { return stamen::Menu::start(entry); }
void stamen_insert(const char *code, callback_f callback) {
  Menu::insert(code, callback);
}

int stamen_builtin_display(const char *title, const item_t items[], int size) {
  Menu::item_t *litems = new Menu::item_t[size];
  if (!litems) return -1;
  for (int i = 0; i < size; i++) {
    litems[i] = Menu::item_t(items[i].callback, items[i].prompt);
  }
  const int ret = builtinDisplay(title, litems, size);
  delete[] litems;
  return ret;
}
