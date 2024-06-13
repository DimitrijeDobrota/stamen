#include "stamen.h"
#include "stamen.hpp"

namespace stamen {

int stamen_dynamic(const char *code) { return dynamic(code); }

void stamen_read(const char *filename) { return read(filename); }

void stamen_insert(const char *code, stamen_callback_f callback) {
    return insert(code, callback);
}

int stamen_builtin_display(const char *title, const stamen_item_t itemv[],
                           int size) {
    return builtin_display(title, itemv, size);
}

} // namespace stamen
