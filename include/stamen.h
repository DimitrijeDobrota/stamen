#ifndef STAMEN_STAMEN_H
#define STAMEN_STAMEN_H

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC extern
#endif

typedef int (*stamen_callback_f)(int);

typedef struct stamen_item_t stamen_item_t;
struct stamen_item_t {
  stamen_callback_f callback;
  const char *prompt;
};

typedef int (*stamen_display_f)(const char *, const stamen_item_t[], int);
extern const stamen_display_f stamen_display;

EXTERNC int stamen_dynamic(const char *code);
EXTERNC void stamen_read(const char *filename);
EXTERNC void stamen_insert(const char *code, stamen_callback_f callback);

EXTERNC int stamen_builtin_display(const char *title,
                                   const stamen_item_t itemv[], int size);
#ifdef __cplusplus
namespace stamen {

using callback_f = stamen_callback_f;
using display_f = stamen_display_f;
using item_t = stamen_item_t;

const auto dynamic = stamen_dynamic;
const auto read = stamen_read;
const auto insert = stamen_insert;
const auto builtin_display = stamen_builtin_display;

} // namespace stamen
#endif

#endif
