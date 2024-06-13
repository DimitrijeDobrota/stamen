#ifndef STAMEN_STAMEN_H
#define STAMEN_STAMEN_H

#ifdef __cplusplus
extern "C" {
namespace stamen {
#endif

typedef int (*stamen_callback_f)(int);

typedef struct stamen_item_t stamen_item_t;
struct stamen_item_t {
    stamen_callback_f callback;
    const char *prompt;
};

typedef int (*stamen_display_f)(const char *, const stamen_item_t[], int);
extern const stamen_display_f stamen_display;

#if !defined __cplusplus || defined WITH_C_BINDINGS

void stamen_read(const char *filename);
void stamen_insert(const char *code, stamen_callback_f callback);

int stamen_dynamic(const char *code);
int stamen_builtin_display(const char *title, const stamen_item_t itemv[],
                           int size);

#endif

#ifdef __cplusplus
} // namespace stamen
} // extern "C"
#endif

#endif // STAMEN_STAMEN_H
