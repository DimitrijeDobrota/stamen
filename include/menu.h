#ifndef STAMEN_MENU_H
#define STAMEN_MENU_H

#include "stamen.h"

#include <string>
#include <unordered_map>
#include <vector>

class Menu {
  struct private_ctor_t {};

public:
  using lookup_t = std::unordered_map<std::string, Menu>;
  static lookup_t lookup;

  static void read(const std::string &s);
  static void print(const std::string &entry) { internal_print(entry, 1); }
  static lookup_t &getLookup() { return lookup; }

  Menu(const Menu &) = delete;
  Menu &operator=(const Menu &) = delete;

  // Tag type dispatch
  Menu(private_ctor_t, const std::string &code, const std::string &prompt)
      : Menu(code, prompt) {}

  struct lookup_item_t {
    const std::string code;
    const std::string prompt;
  };

  [[nodiscard]] const std::string &getCode() const { return code; }
  [[nodiscard]] const std::string &getTitle() const { return title; }
  [[nodiscard]] const std::vector<lookup_item_t> &getItems() const {
    return lookup_items;
  }

private:
  Menu(std::string code, std::string prompt)
      : code(std::move(code)), title(std::move(prompt)) {}

  static const Menu *getMenu(const std::string &code) {
    const auto it = lookup.find(code);
    if (it == lookup.end()) return nullptr;
    return &it->second;
  }

  static void internal_print(const std::string &entry, const int depth);

  std::vector<lookup_item_t> lookup_items;
  const std::string code, title;
};

#endif
