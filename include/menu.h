#ifndef STAMEN_MENU_H
#define STAMEN_MENU_H

#include "stamen.h"

#include <cstring>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class Menu {
  friend class Generator;

  using callback_f = stamen_callback_f;
  using item_t = stamen_item_t;

  static std::unordered_map<std::string, Menu> menu_lookup;
  static std::unordered_map<std::string, callback_f> free_lookup;

  struct private_ctor_t {};

public:
  // Tag type dispatch
  Menu(private_ctor_t, const std::string &code, const std::string &prompt)
      : Menu(code, prompt) {}

  Menu(const Menu &) = delete;
  Menu &operator=(const Menu &) = delete;

  static int dynamic() { return display_stub(-1); };
  static void read(const std::string &s);
  static void print(const std::string &entry) { print(entry, 1); }
  static void insert(const std::string &s, callback_f callback) {
    free_lookup.emplace(s, callback);
  }

  [[nodiscard]] const std::string &getCode() const { return code; }
  [[nodiscard]] const std::string &getTitle() const { return title; }

  [[nodiscard]] const item_t *getItemv() const { return entries.items.data(); }
  [[nodiscard]] std::size_t getSize() const { return entries.items.size(); }

  [[nodiscard]] const std::string &getCode(std::size_t idx) const {
    return entries.codes[idx].code;
  }

  [[nodiscard]] const std::string &getPrompt(std::size_t idx) const {
    return entries.codes[idx].prompt;
  }

  [[nodiscard]] callback_f getCallback(std::size_t idx) const {
    return entries.items[idx].callback;
  }

private:
  Menu(std::string code, std::string prompt)
      : code(std::move(code)), title(std::move(prompt)) {}

  static void print(const std::string &entry, const int depth);
  static int display_stub(int idx);

  static const Menu *getMenu(const std::string &code) {
    const auto it = menu_lookup.find(code);
    if (it == menu_lookup.end()) return nullptr;
    return &it->second;
  }

  struct Entries {
    struct code_t {
      const std::string code;
      const std::string prompt;
    };

    std::vector<code_t> codes;
    std::vector<item_t> items;

    void insert(const std::string &code, const std::string &prompt,
                callback_f callback = display_stub) {
      char *buffer = new char[prompt.size() + 1];
      strcpy(buffer, prompt.c_str());
      items.emplace_back(callback, buffer);

      codes.emplace_back(code, prompt);
    }
  };

  const std::string code, title;
  Entries entries;
};

#endif
