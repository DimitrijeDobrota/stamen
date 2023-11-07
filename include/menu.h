#ifndef MENU_H
#define MENU_H

#include "json.hpp"
#include <exception>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

using json = nlohmann::json;

class EMenu_callback : std::exception {
  virtual const char *what() const noexcept override {
    return "Unknown callback function or menu method";
  }
};

class EMenu_call : std::exception {
  virtual const char *what() const noexcept override {
    return "Trying a callback that doesn't exist";
  }
};

class Menu {
public:
  typedef int (*callback_f)(void);

  Menu(const std::string &s) {
    for (const auto &menu : json::parse(std::fstream(s))) {
      const json &items = menu["items"];
      const function_t *mf =
          new function_t(*this, menu["name"], {items.begin(), items.end()});
      lookup.insert({menu["code"], callback_t(mf)});
    }
  }

  void operator()() const { get_callback("main")(); }

  struct record_t {
    const std::string code;
    const callback_f callback;
  };

  void insert(const record_t &record) {
    lookup.insert({record.code, record.callback});
  }
  void insert(const std::vector<record_t> &records) {
    for (const auto &record : records) { insert(record); }
  }

private:
  struct item_t {
    const std::string prompt;
    const std::string callback;

    item_t(const std::string &p, const std::string &c)
        : prompt(p), callback(c) {}
    item_t(const json &j) : item_t(j["prompt"], j["callback"]) {}
  };

  struct function_t {
    function_t(const Menu &m, const std::string &n,
               const std::vector<item_t> &i)
        : menu(m), name(n), items(i) {}

    int display() const;

  private:
    const Menu &menu;
    const std::string name;
    const std::vector<item_t> items;
  };

  struct callback_t {
    const function_t *menu_func = nullptr;
    const callback_f func = nullptr;

    callback_t(const callback_f f) : func(f) {}
    callback_t(const function_t *f) : menu_func(f) {}

    int operator()() const {
      if (!func && !menu_func) throw EMenu_callback();
      return func ? func() : menu_func->display();
    }
  };

  const callback_t &get_callback(const std::string &s) const {
    const auto it = lookup.find(s);
    if (it == lookup.end()) throw EMenu_call();
    return it->second;
  }

  std::unordered_map<std::string, callback_t> lookup;
};

#endif
