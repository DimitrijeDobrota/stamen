#ifndef STAMEN_STAMEN_H
#define STAMEN_STAMEN_H

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC extern
#endif

typedef int (*stamen_callback_f)(void);

typedef struct item_t item_t;
struct item_t {
  stamen_callback_f callback;
  const char *prompt;
};

typedef int (*stamen_display_f)(const char *, const item_t[], int);
extern const stamen_display_f stamen_display;

EXTERNC int stamen_builtin_display(const char *title, const item_t itemv[], int size);

#endif
