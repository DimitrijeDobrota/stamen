#ifndef MENU_H
#define MENU_H

#include "json.hpp"
#include <exception>
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
  typedef int (*Menu_f)(void);
  static void Register(const std::string &s, Menu_f f) {
    function_lookup.insert({s, f});
  }
  static void Start() { get_callback("main")(); }
  static void Read(std::istream &is);

private:
  Menu() = delete;

  struct Menu_item {
    const std::string prompt;
    const std::string callback;

    Menu_item(const std::string &p, const std::string &c)
        : prompt(p), callback(c) {}
    Menu_item(const json &j) : Menu_item(j["prompt"], j["callback"]) {}
  };

  struct Menu_function {
    Menu_function(const std::string &n, const std::vector<Menu_item> &i)
        : name(n), items(i) {}

    int call() const;

  private:
    const std::string name;
    const std::vector<Menu_item> items;
  };

  struct Menu_callback {
    const Menu_f func = nullptr;
    const Menu_function *menu_func = nullptr;

    Menu_callback(const Menu_f f) : func(f) {}
    Menu_callback(const Menu_function *f) : menu_func(f) {}

    int operator()() const {
      if (!func && !menu_func) throw EMenu_callback();
      return func ? func() : menu_func->call();
    }
  };

  static const Menu_callback &get_callback(const std::string &s) {
    auto it = function_lookup.find(s);
    if (it == function_lookup.end()) throw EMenu_call();
    return (*it).second;
  }

  static std::unordered_map<std::string, Menu_callback> function_lookup;
};

#endif
