#include "menu.h"

#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <tuple>
#include <utility>

Menu::lookup_t Menu::lookup;

void Menu::read(const std::string &s) {
  std::string line, delim, code, prompt;
  std::fstream fs(s);
  char tmp = 0;

  lookup_t &lookup = getLookup();
  auto last = lookup.end();
  while (std::getline(fs, line)) {
    if (line.empty()) continue;
    std::istringstream ss(line);
    ss >> delim >> code;
    ss.ignore(1, ' '), std::getline(ss, prompt);
    if (delim == "+") {
      const auto [iter, succ] =
          lookup.emplace(std::piecewise_construct, std::forward_as_tuple(code),
                         std::forward_as_tuple(private_ctor_t{}, code, prompt));
      last = iter;
    } else {
      last->second.lookup_items.emplace_back(code, prompt);
    }
  }
}

void Menu::internal_print(const std::string &code, const int depth) {
  const Menu *menu = getMenu(code);
  if (!menu) return;

  if (depth == 1) std::cout << std::format("{}({})\n", menu->title, code);

  for (const auto &[code, prompt] : menu->lookup_items) {
    std::cout << std::format("{}{} ({})\n", std::string(depth << 1, ' '),
                             prompt, code);
    menu->internal_print(code, depth + 1);
  }
}
