#include <deque>
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <tuple>
#include <utility>

#include "stamen/menu.hpp"

namespace stamen::menu {

// NOLINTBEGIN
std::unordered_map<std::string, menu_t> menu_lookup;
std::unordered_map<std::string, callback_f> free_lookup;
std::string display_stub_default;
display_f display;
// NOLINTEND

void read(const char* filename)
{
  std::fstream fst(filename);
  std::string line;
  std::string delim;
  std::string code;
  std::string prompt;

  auto last = menu_lookup.end();
  while (std::getline(fst, line))
  {
    if (line.empty()) continue;

    std::istringstream iss(line);
    iss >> delim >> code >> std::ws;
    std::getline(iss, prompt);

    if (delim != "+") last->second.insert(code, prompt);
    else
    {
      const auto [iter, succ] = menu_lookup.emplace(
          std::piecewise_construct,
          std::forward_as_tuple(code),
          std::forward_as_tuple(menu_t::private_ctor_t {}, code, prompt));
      last = iter;
    }
  }
}

void insert(const char* code, callback_f callback)
{
  free_lookup.emplace(code, callback);
}

int dynamic(const char* code, display_f disp)
{
  menu::display_stub_default = code;
  menu::display              = disp;
  return display_stub(0);
}

int display_stub(std::size_t idx)
{
  static std::deque<const menu_t*> stack;

  const std::string& code =
      !stack.empty() ? stack.back()->get_code(idx) : display_stub_default;

  const auto ml_it = menu_lookup.find(code);
  if (ml_it != menu_lookup.end())
  {
    const auto& m = ml_it->second;  // NOLINT

    stack.push_back(&m);
    const int ret =
        display(m.get_title().c_str(), m.get_itemv(), m.get_size());
    stack.pop_back();

    return ret;
  }

  const auto fl_it = free_lookup.find(code);
  if (fl_it != free_lookup.end()) return fl_it->second(0);

  std::cout << "Stamen: nothing to do..." << std::endl;
  return 1;
}

void menu_t::insert(const std::string& code,
                    const std::string& prompt,
                    callback_f callback)
{
  char* buffer = new char[prompt.size() + 1];  // NOLINT
  strcpy(buffer, prompt.c_str());  // NOLINT

  m_items.emplace_back(callback, buffer);
  m_codes.emplace_back(code, prompt);
}

}  // namespace stamen::menu
