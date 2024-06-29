#pragma once

#include <cstring>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "stamen/stamen.hpp"

namespace stamen::menu {

class menu_t;

// NOLINTBEGIN
extern std::unordered_map<std::string, callback_f> free_lookup;
extern std::unordered_map<std::string, menu_t> menu_lookup;
extern std::string display_stub_default;
extern display_f display;
// NOLINTEND

void read(const char* filename);
void insert(const char* code, callback_f callback);
int dynamic(const char* code, display_f disp);
int display_stub(std::size_t idx);

class menu_t
{
  struct private_ctor_t
  {
  };
  friend void read(const char* filename);

public:
  // Tag type dispatch
  menu_t(private_ctor_t,  // NOLINT
         const std::string& code,
         const std::string& prompt)
      : menu_t(code, prompt)
  {
  }

  menu_t(const menu_t&)            = delete;
  menu_t& operator=(const menu_t&) = delete;
  menu_t(menu_t&&)                 = delete;
  menu_t& operator=(menu_t&&)      = delete;

  ~menu_t() noexcept
  {
    for (const auto [_, prompt] : m_items)
    {
      delete[] prompt;  // NOLINT
    }
  }

  const std::string& get_code() const { return m_code; }
  const std::string& get_title() const { return m_title; }

  const item_t* get_itemv() const { return m_items.data(); }
  std::size_t get_size() const { return m_items.size(); }

  auto get_callback(std::size_t idx) const { return m_items[idx].callback; }
  const auto& get_code(std::size_t idx) const { return m_codes[idx].code; }
  const auto& get_prompt(std::size_t idx) const { return m_codes[idx].prompt; }

private:
  menu_t(std::string code, std::string prompt)
      : m_code(std::move(code))
      , m_title(std::move(prompt))
  {
  }

  void insert(const std::string& code,
              const std::string& prompt,
              callback_f callback = display_stub);

  struct code_t
  {
    std::string code;
    std::string prompt;
  };

  std::string m_code;
  std::string m_title;
  std::vector<code_t> m_codes;
  std::vector<item_t> m_items;
};

}  // namespace stamen::menu
