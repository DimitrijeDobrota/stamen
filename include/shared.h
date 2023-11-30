#ifndef STAMEN_SHARED_H
#define STAMEN_SHARED_H

typedef int (*stamen_callback_f)(void);

typedef struct item_t item_t;
struct item_t {
  stamen_callback_f callback;
  const char *prompt;
};

typedef int (*stamen_display_f)(const char *, const item_t[], int);
extern const stamen_display_f stamen_display;

#endif
