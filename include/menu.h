#ifndef MENU_H
#define MENU_H

#include "json.hpp"
#include <exception>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

using json = nlohmann::json;
using std::string, std::vector;

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

  Menu() : function_lookup() {}
  Menu(const string &s) : Menu() {
    std::fstream f(s);
    Read(f);
  }

  void Start() const { get_callback("main")(); }

  void Read(std::istream &is);
  Menu &Register(const string &s, Menu_f f) {
    function_lookup.insert({s, f});
    return *this;
  }

private:
  struct Menu_item {
    const string prompt;
    const string callback;

    Menu_item(const string &p, const string &c) : prompt(p), callback(c) {}
    Menu_item(const json &j) : Menu_item(j["prompt"], j["callback"]) {}
  };

  struct Menu_function {
    Menu_function(const Menu &m, const string &n, const vector<Menu_item> &i)
        : menu(m), name(n), items(i) {}

    int display() const;

  private:
    const Menu &menu;
    const string name;
    const vector<Menu_item> items;
  };

  struct Menu_callback {
    const Menu_f func = nullptr;
    const Menu_function *menu_func = nullptr;

    Menu_callback(const Menu_f f) : func(f) {}
    Menu_callback(const Menu_function *f) : menu_func(f) {}

    int operator()() const {
      if (!func && !menu_func) throw EMenu_callback();
      return func ? func() : menu_func->display();
    }
  };

  const Menu_callback &get_callback(const string &s) const {
    auto it = function_lookup.find(s);
    if (it == function_lookup.end()) throw EMenu_call();
    return (*it).second;
  }

  std::unordered_map<string, Menu_callback> function_lookup;
};

#endif
