#ifndef CSTAMEN_H
#define CSTAMEN_H

#include "shared.h"

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC extern
#endif

EXTERNC void stamen_read(const char *file);
EXTERNC void stamen_print(const char *entry);
EXTERNC void stamen_insert(const char *code, stamen_callback_f callback);
EXTERNC int stamen_builtin_display(const char *title, const item_t itemv[], int size);

#endif
