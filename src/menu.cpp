#include "menu.h"

#include <deque>
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <tuple>
#include <utility>

namespace stamen {

std::unordered_map<std::string, Menu> Menu::menu_lookup;
std::unordered_map<std::string, callback_f> Menu::free_lookup;
std::string Menu::display_stub_default;
display_f Menu::display;

void Menu::entries_t::insert(const std::string &code,
                             const std::string &prompt, callback_f callback) {
    char *buffer = new char[prompt.size() + 1];
    strcpy(buffer, prompt.c_str());

    items.emplace_back(callback, buffer);
    codes.emplace_back(code, prompt);
}

const std::string &Menu::getCode(std::size_t idx) const {
    return entries.codes[idx].code;
}

const std::string &Menu::getPrompt(std::size_t idx) const {
    return entries.codes[idx].prompt;
}

callback_f Menu::getCallback(std::size_t idx) const {
    return entries.items[idx].callback;
}

void Menu::read(const std::string &s) {
    std::string line, delim, code, prompt;
    std::fstream fs(s);

    auto last = menu_lookup.end();
    while (std::getline(fs, line)) {
        if (line.empty()) continue;

        std::istringstream ss(line);
        ss >> delim >> code >> std::ws;
        std::getline(ss, prompt);

        if (delim != "+") last->second.entries.insert(code, prompt);
        else {
            const auto [iter, succ] = menu_lookup.emplace(
                std::piecewise_construct, std::forward_as_tuple(code),
                std::forward_as_tuple(private_ctor_t{}, code, prompt));
            last = iter;
        }
    }
}

int Menu::display_stub(int idx) {
    static std::deque<const Menu *> st;

    const std::string &code =
        !st.empty() ? st.back()->getCode(idx) : display_stub_default;

    const auto ml_it = menu_lookup.find(code);
    if (ml_it != menu_lookup.end()) {
        const Menu &menu = ml_it->second;
        st.push_back(&menu);
        int ret = display(menu.title.c_str(), menu.getItemv(), menu.getSize());
        st.pop_back();
        return ret;
    }

    const auto fl_it = free_lookup.find(code);
    if (fl_it != free_lookup.end()) return fl_it->second(0);

    std::cout << "Stamen: nothing to do..." << std::endl;
    return 1;
}

} // namespace stamen
