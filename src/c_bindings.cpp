#include "menu.hpp"
#include "stamen.hpp"

namespace stamen {

int stamen_builtin_display(const char *title, const stamen_item_t itemv[], int size) {
    return builtin_display(title, itemv, size);
}

namespace menu {

void stamen_menu_read(const char *filename) { return read(filename); }

void stamen_menu_insert(const char *code, stamen_callback_f callback) {
    return insert(code, callback);
}

int stamen_menu_dynamic(const char *code, stamen_display_f display) {
    return dynamic(code, display);
}

} // namespace menu
} // namespace stamen
