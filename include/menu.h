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

private:
  struct item_t {
    const std::string prompt;
    const std::string callback;

    item_t(const std::string &p, const std::string &c)
        : prompt(p), callback(c) {}
    item_t(const json &j) : item_t(j["prompt"], j["callback"]) {}
  };

  Menu(const json &json_data)
      : name(json_data["name"]), code(json_data["code"]),
        items({json_data["items"].begin(), json_data["items"].end()}) {}

  Menu(const std::string code, const callback_f callback)
      : name(code), code(code), items(), callback(callback) {}

public:
  static void read(const std::string &s) {
    for (const auto &json_data : json::parse(std::fstream(s))) {
      lookup.insert({json_data["code"], Menu(json_data)});
    }
  }

  static int start() { return getMenu("main")(); }
  static void insert(const std::string code, const callback_f callback) {
    lookup.insert({code, Menu(code, callback)});
  }

  static void print(const std::string code = "main", const int depth = 1);
  int operator()() const;

private:
  static std::unordered_map<std::string, Menu> lookup;

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
