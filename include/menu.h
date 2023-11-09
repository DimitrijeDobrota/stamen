#ifndef MENU_H
#define MENU_H

#include "json.hpp"
#include <exception>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

using json = nlohmann::json;

class Menu {
public:
  typedef int (*callback_f)(void);

  class EMenu : std::exception {
    virtual const char *what() const noexcept override {
      return "Trying to access an unknown menu";
    }
  };

  struct item_t {
    const std::string prompt, callback;
    const callback_f func;

    item_t(const std::string &p, const std::string &c,
           const callback_f func = nullptr)
        : prompt(p), callback(c), func(func) {}
    item_t(const json &j) : item_t(j["prompt"], j["callback"]) {}
  };

  static void read(const std::string &s) {
    for (const auto &json_data : json::parse(std::fstream(s))) {
      lookup.insert({json_data["code"], Menu(json_data)});
    }
  }

  static int start() { return getMenu(entry)(); }
  static void insert(const std::string code, const callback_f callback) {
    lookup.insert({code, Menu(code, callback)});
  }

  static void print(const std::string &code = entry, const int depth = 1);
  static void generate(const std::string &code = entry);
  static int display(const std::string &name, const item_t items[], int size);

  int operator()() const {
    return callback ? callback() : display(name, items.data(), items.size());
  }

private:
  Menu(const json &json_data)
      : name(json_data["name"]), code(json_data["code"]),
        items({json_data["items"].begin(), json_data["items"].end()}) {}

  Menu(const std::string code, const callback_f callback)
      : name(code), code(code), items(), callback(callback) {}

  static std::unordered_map<std::string, Menu> lookup;
  static const std::string entry;

  static const Menu &getMenu(const std::string &code) {
    const auto it = lookup.find(code);
    if (it == lookup.end()) throw EMenu();
    return it->second;
  }

  const std::string name, code;
  const std::vector<item_t> items;
  const callback_f callback = nullptr;
};

#endif
