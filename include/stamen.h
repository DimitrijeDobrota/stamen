#ifndef STAMEN_H
#define STAMEN_H

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
public:
  friend class Generator;
  typedef int (*callback_f)(void);

  Menu(const Menu &) = delete;
  Menu &operator=(const Menu &) = delete;

  struct private_ctor_t {};

  // Tag type dispatch
  Menu(private_ctor_t, const std::string &code, const std::string &prompt)
      : Menu(code, prompt) {}

  Menu(private_ctor_t, const std::string &code, const callback_f callback)
      : Menu(code, callback) {}

  struct item_t {
    item_t(const callback_f func, const std::string &prompt)
        : callback(func), prompt(prompt) {}

    item_t(const std::string &code, const std::string &prompt)
        : code(code), prompt(prompt) {}

    const std::string getCode(void) const { return code; }
    const std::string getPrompt(void) const { return prompt; }
    const callback_f getCallback(void) const { return callback; }

    int operator()(void) const { return callback ? callback() : start(code); }

  private:
    const std::string prompt, code;
    const callback_f callback = nullptr;
  };

  typedef int (*display_f)(const std::string &, const item_t[], std::size_t);
  static const display_f display;

  static void read(const std::string &s);
  static void insert(const std::string &code, const callback_f callback);

  static void print(const std::string &entry) { print(entry, 1); }
  static int start(const std::string &entry) {
    const Menu *menu = getMenu(entry);
    if (!menu) return 1;
    return menu->operator()();
  }

  int operator()() const {
    return callback ? callback() : display(title, items.data(), items.size());
  }

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

  static void print(const std::string &entry, const int depth);

  static const Menu *getMenu(const std::string &code) {
    static lookup_t &lookup = getLookup();
    const auto it = lookup.find(code);
    if (it == lookup.end()) return nullptr;
    return &it->second;
  }

  const std::string code, title;
  const callback_f callback = nullptr;
  std::vector<item_t> items;
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

inline void Menu::print(const std::string &code, const int depth) {
  const Menu *menu = getMenu(code);
  if (!menu) return;

  if (depth == 1) std::cout << std::format("{}({})\n", menu->title, code);

  if (!menu->callback) {
    for (const auto &item : menu->items) {
      std::cout << std::format("{}{} ({})\n", std::string(depth << 1, ' '),
                               item.getPrompt(), item.getCode());
      menu->print(item.getCode(), depth + 1);
    }
  }
}

int builtinDisplay(const std::string &title, const Menu::item_t items[],
                   std::size_t size);

} // namespace stamen

#endif
