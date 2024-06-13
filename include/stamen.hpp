#ifndef STAMEN_STAMEN_HPP
#define STAMEN_STAMEN_HPP

#include "stamen.h"

namespace stamen {

using callback_f = stamen_callback_f;
using display_f = stamen_display_f;
using item_t = stamen_item_t;

extern const display_f &display;

int dynamic(const char *code);
void read(const char *filename);
void insert(const char *code, callback_f callback);
int builtin_display(const char *title, const item_t itemv[], int size);

} // namespace stamen

#endif
