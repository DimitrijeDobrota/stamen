#pragma once

#include <cstring>
#include <functional>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

namespace stamen {

class Menu;

using callback_f = std::function<int(size_t)>;

struct item_t
{
  std::string code;
  std::string prompt;
  callback_f callback;
};

class Menu
{
public:
  Menu(std::string code, std::string prompt)
      : m_code(std::move(code))
      , m_title(std::move(prompt))
  {
  }

  Menu(const Menu&)            = delete;
  Menu& operator=(const Menu&) = delete;

  Menu(Menu&&)            = default;
  Menu& operator=(Menu&&) = default;

  ~Menu() = default;

  const std::string& code() const { return m_code; }
  const std::string& title() const { return m_title; }

  std::size_t size() const { return m_items.size(); }

  const item_t& item(std::size_t idx) const { return m_items[idx]; }
  const auto& items() const { return m_items; }

  void insert(const std::string& code,
              const std::string& prompt,
              const callback_f& callback);

private:
  std::string m_code;
  std::string m_title;
  std::vector<item_t> m_items;
};

class Stamen
{
public:
  using display_f = std::function<int(const Menu&)>;

  explicit Stamen(std::istream& ist);

  void insert(const std::string& code, const callback_f& callback);

  int dynamic(const std::string& code, const display_f& disp);

  const auto& free_lookup() const { return m_free_lookup; }
  const auto& menu_lookup() const { return m_menu_lookup; }

private:
  int display_stub(std::size_t idx);

  std::unordered_map<std::string, callback_f> m_free_lookup;
  std::unordered_map<std::string, Menu> m_menu_lookup;

  display_f m_stub_display;
  std::string m_stub_default;
  std::stack<const Menu*> m_stub_stack;
};

}  // namespace stamen
