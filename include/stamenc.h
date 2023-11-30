#ifndef CSTAMEN_H
#define CSTAMEN_H

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC extern
#endif

typedef int (*callback_f)(void);

typedef struct item_t item_t;
struct item_t {
  callback_f callback;
  const char *prompt;
};

typedef int (*display_f)(const char *, const item_t[], int);
extern const display_f display;

EXTERNC void stamen_read(const char *file);
EXTERNC void stamen_print(const char *entry);
EXTERNC int stamen_start(const char *entry);
EXTERNC void stamen_insert(const char *code, callback_f callback);
EXTERNC int stamen_builtin_display(const char *title, const item_t items[], int size);

#endif
