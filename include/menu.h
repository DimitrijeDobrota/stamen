#ifndef STAMEN_MENU_H
#define STAMEN_MENU_H

#include "stamen.h"

#ifdef __cplusplus
extern "C" {
namespace stamen {
#endif

void stamen_menu_read(const char *filename);
void stamen_menu_insert(const char *code, stamen_callback_f callback);
int stamen_menu_dynamic(const char *code, stamen_display_f display);

#ifdef __cplusplus
} // namespace stamen
} // extern "C"
#endif

#endif
