#pragma once

#include "stddef.h"  // NOLINT

#ifdef __cplusplus
extern "C"
{
namespace stamen {
#endif

typedef int (*stamen_callback_f)(size_t);  // NOLINT

typedef struct stamen_item_t stamen_item_t;  // NOLINT
struct stamen_item_t
{
  stamen_callback_f callback;
  const char* prompt;
};

typedef int (*stamen_display_f)(const char*,  // NOLINT
                                const stamen_item_t[],
                                size_t);

#if !defined __cplusplus || defined WITH_C_BINDINGS

int stamen_builtin_display(const char* title,
                           const stamen_item_t itemv[],
                           size_t size);
#else

int builtin_display(const char* title,
                    const stamen_item_t itemv[],
                    size_t size);

#endif

#ifdef __cplusplus
}  // namespace stamen
}  // extern "C"
#endif
