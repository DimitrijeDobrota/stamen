#ifndef MENU_H
#define MENU_H

#include <exception>
#include <format>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

class Menu {
  Menu(const Menu &) = delete;
  Menu &operator=(const Menu &) = delete;

  struct private_ctor_t {};

public:
  typedef int (*callback_f)(void);

  Menu(private_ctor_t, const std::string &code, const std::string &prompt)
      : Menu(code, prompt) {}

  Menu(private_ctor_t, const std::string &code, const callback_f callback)
      : Menu(code, callback) {}

  class EMenu : std::exception {
    virtual const char *what() const noexcept override {
      return "Trying to access an unknown menu";
    }
  };

  struct item_t {
    friend class Menu;

    item_t(const callback_f func, const std::string &prompt)
        : callback(func), prompt(prompt) {}

    const std::string getPrompt(void) const { return prompt; }
    const callback_f getCallback(void) const { return callback; }

    int operator()(void) const {
      return callback ? callback() : getMenu(code)();
    }

  private:
    item_t(const std::string &code, const std::string &prompt)
        : code(code), prompt(prompt) {}

    const std::string prompt, code;
    const callback_f callback = nullptr;
  };

  static void read(const std::string &s) {
    std::string line, delim, code, prompt;
    std::fstream fs(s);
    char tmp;

    lookup_t::iterator last = lookup.end();
    while (std::getline(fs, line)) {
      if (line.empty()) continue;
      std::istringstream ss(line);
      ss >> delim >> code;
      ss.ignore(1, ' '), std::getline(ss, prompt);
      if (delim == "+") {
        const auto [iter, succ] = lookup.emplace(
            std::piecewise_construct, std::forward_as_tuple(code),
            std::forward_as_tuple(private_ctor_t{}, code, prompt));
        last = iter;
      } else {
        last->second.items.push_back({code, prompt});
      }
    }
  }

  static int start(const std::string &entry) { return getMenu(entry)(); }
  static void insert(const std::string &code, const callback_f callback) {
    lookup.emplace(std::piecewise_construct, std::forward_as_tuple(code),
                   std::forward_as_tuple(private_ctor_t{}, code, callback));
  }

  static void print(const std::string &entry) { print(entry, 1); }

  static void generateSource(std::ostream &os);
  static void generateInclude(std::ostream &os);

  typedef int (*display_f)(const std::string &, const item_t[], std::size_t);
  static const display_f display;
  static int builtinDisplay(const std::string &title, const item_t items[],
                            std::size_t size);

  int operator()() const {
    return callback ? callback() : display(title, items.data(), items.size());
  }

private:
  Menu(const std::string &code, const std::string &prompt)
      : code(code), title(prompt) {}

  Menu(const std::string &code, const callback_f callback)
      : code(code), title(code), callback(callback) {}

  typedef std::unordered_map<std::string, Menu> lookup_t;
  static lookup_t lookup;

  static void print(const std::string &entry, const int depth);

  static const Menu &getMenu(const std::string &code) {
    const auto it = lookup.find(code);
    if (it == lookup.end()) throw EMenu();
    return it->second;
  }

  const std::string code, title;
  const callback_f callback = nullptr;
  std::vector<item_t> items;
};

#endif
