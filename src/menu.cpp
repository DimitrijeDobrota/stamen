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

void Menu::read(const std::string &s) {
  std::string line, delim, code, prompt;
  std::fstream fs(s);
  char tmp = 0;

  auto last = menu_lookup.end();
  while (std::getline(fs, line)) {
    if (line.empty()) continue;
    std::istringstream ss(line);
    ss >> delim >> code;
    ss.ignore(1, ' '), std::getline(ss, prompt);
    if (delim == "+") {
      const auto [iter, succ] = menu_lookup.emplace(
          std::piecewise_construct, std::forward_as_tuple(code),
          std::forward_as_tuple(private_ctor_t{}, code, prompt));
      last = iter;
    } else {
      last->second.entries.insert(code, prompt);
    }
  }
}

void Menu::print(const std::string &code, const int depth) {
  const auto it = menu_lookup.find(code);
  if (it == menu_lookup.end()) return;
  const Menu *menu = &it->second;

  if (depth == 1) std::cout << std::format("{}({})\n", menu->title, code);

  for (int i = 0; i < menu->getSize(); i++) {
    std::cout << std::format("{}{} ({})\n", std::string(depth << 1, ' '),
                             menu->getPrompt(i), menu->getCode(i));
    menu->print(code, depth + 1);
  }
}

int Menu::display_stub(int idx) {
  static std::deque<const Menu *> st;

  const std::string &code =
      st.size() ? st.back()->getCode(idx) : display_stub_default;

  const auto ml_it = menu_lookup.find(code);
  if (ml_it != menu_lookup.end()) {
    const Menu &menu = ml_it->second;
    st.push_back(&menu);
    int ret = stamen_builtin_display(menu.title.c_str(), menu.getItemv(),
                                     menu.getSize());
    st.pop_back();
    return ret;
  }

  const auto fl_it = free_lookup.find(code);
  if (fl_it != free_lookup.end()) { return fl_it->second(0); }

  std::cout << "Stamen: nothing to do..." << std::endl;
  return 1;
}

} // namespace stamen
