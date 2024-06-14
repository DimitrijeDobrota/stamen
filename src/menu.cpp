#include "menu.hpp"

#include <deque>
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <tuple>
#include <utility>

namespace stamen {
namespace menu {

std::unordered_map<std::string, menu_t> menu_lookup;
std::unordered_map<std::string, callback_f> free_lookup;
std::string display_stub_default;
display_f display;

void read(const char *filename) {
    std::string line, delim, code, prompt;
    std::fstream fs(filename);

    auto last = menu_lookup.end();
    while (std::getline(fs, line)) {
        if (line.empty()) continue;

        std::istringstream ss(line);
        ss >> delim >> code >> std::ws;
        std::getline(ss, prompt);

        if (delim != "+") last->second.insert(code, prompt);
        else {
            const auto [iter, succ] = menu_lookup.emplace(
                std::piecewise_construct, std::forward_as_tuple(code),
                std::forward_as_tuple(menu_t::private_ctor_t{}, code, prompt));
            last = iter;
        }
    }
}

void insert(const char *code, callback_f callback) {
    free_lookup.emplace(code, callback);
}

int dynamic(const char *code, display_f display) {
    menu::display_stub_default = code;
    menu::display = display;
    return display_stub(-1);
}

int display_stub(int idx) {
    static std::deque<const menu_t *> st;

    const std::string &code =
        !st.empty() ? st.back()->getCode(idx) : display_stub_default;

    const auto ml_it = menu_lookup.find(code);
    if (ml_it != menu_lookup.end()) {
        const auto &m = ml_it->second;

        st.push_back(&m);
        int ret = display(m.getTitle().c_str(), m.getItemv(), m.getSize());
        st.pop_back();

        return ret;
    }

    const auto fl_it = free_lookup.find(code);
    if (fl_it != free_lookup.end()) return fl_it->second(0);

    std::cout << "Stamen: nothing to do..." << std::endl;
    return 1;
}

void menu_t::insert(const std::string &code, const std::string &prompt,
                    callback_f callback) {
    char *buffer = new char[prompt.size() + 1];
    strcpy(buffer, prompt.c_str());

    items.emplace_back(callback, buffer);
    codes.emplace_back(code, prompt);
}

} // namespace menu
} // namespace stamen
