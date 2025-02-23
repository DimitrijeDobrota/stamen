#pragma once

#include <cstring>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace stamen {

class menu_t;

using callback_f = std::function<int(size_t)>;
using display_f  = std::function<int(const menu_t&)>;

struct item_t
{
  std::string code;
  std::string prompt;
  callback_f callback;
};

// NOLINTBEGIN
extern std::unordered_map<std::string, callback_f> free_lookup;
extern std::unordered_map<std::string, menu_t> menu_lookup;
extern std::string display_stub_default;
// NOLINTEND

void read(const char* filename);
void insert(const char* code, const callback_f& callback);
int dynamic(const char* code, const display_f& disp);
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

  ~menu_t() = default;

  const std::string& code() const { return m_code; }
  const std::string& title() const { return m_title; }

  std::size_t size() const { return m_items.size(); }
  const item_t& item(std::size_t idx) const { return m_items[idx]; }
  item_t& item(std::size_t idx) { return m_items[idx]; }

  const auto& items() const { return m_items; }
  auto& items() { return m_items; }

private:
  menu_t(std::string code, std::string prompt)
      : m_code(std::move(code))
      , m_title(std::move(prompt))
  {
  }

  void insert(const std::string& code,
              const std::string& prompt,
              const callback_f& callback = display_stub);

  std::string m_code;
  std::string m_title;
  std::vector<item_t> m_items;
};

}  // namespace stamen
