#ifndef STAMEN_H
#define STAMEN_H

#include "shared.h"

#include <exception>
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace stamen {

class Menu {
  struct private_ctor_t {};

public:
  friend class Generator;

  typedef stamen_callback_f callback_f;
  typedef stamen_display_f display_f;

  Menu(const Menu &) = delete;
  Menu &operator=(const Menu &) = delete;

  // Tag type dispatch
  Menu(private_ctor_t, const std::string &code, const std::string &prompt)
      : Menu(code, prompt) {}

  Menu(private_ctor_t, const std::string &code, const callback_f callback)
      : Menu(code, callback) {}

  static void read(const std::string &s);
  static void print(const std::string &entry) { internal_print(entry, 1); }
  static void insert(const std::string &code, const callback_f callback);

private:
  Menu(const std::string &code, const std::string &prompt)
      : code(code), title(prompt) {}

  Menu(const std::string &code, const callback_f callback)
      : code(code), title(code), callback(callback) {}

  typedef std::unordered_map<std::string, Menu> lookup_t;
  static lookup_t &getLookup(void) {
    static lookup_t lookup;
    return lookup;
  }

  static void internal_print(const std::string &entry, const int depth);

  static const Menu *getMenu(const std::string &code) {
    static lookup_t &lookup = getLookup();
    const auto it = lookup.find(code);
    if (it == lookup.end()) return nullptr;
    return &it->second;
  }

  const std::string code, title;
  const callback_f callback = nullptr;

  typedef std::pair<std::string, std::string> lookup_item_t;
  std::vector<lookup_item_t> items;
};

inline void Menu::read(const std::string &s) {
  std::string line, delim, code, prompt;
  std::fstream fs(s);
  char tmp;

  lookup_t &lookup = getLookup();
  lookup_t::iterator last = lookup.end();
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
      last->second.items.push_back({code, prompt});
    }
  }
}

inline void Menu::insert(const std::string &code, const callback_f callback) {
  getLookup().emplace(std::piecewise_construct, std::forward_as_tuple(code),
                      std::forward_as_tuple(private_ctor_t{}, code, callback));
}

inline void Menu::internal_print(const std::string &code, const int depth) {
  const Menu *menu = getMenu(code);
  if (!menu) return;

  if (depth == 1) std::cout << std::format("{}({})\n", menu->title, code);

  if (!menu->callback) {
    for (const auto &[code, prompt] : menu->items) {
      std::cout << std::format("{}{} ({})\n", std::string(depth << 1, ' '),
                               prompt, code);
      menu->internal_print(code, depth + 1);
    }
  }
}

int builtinDisplay(const char *title, const ::item_t items[], int size);

} // namespace stamen

#endif
