#include <cstddef>
#include <deque>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_set>
#include <utility>

#include "stamen/stamen.hpp"

namespace stamen {

// NOLINTBEGIN
std::unordered_map<std::string, menu_t> menu_lookup;
std::unordered_map<std::string, callback_f> free_lookup;
std::string display_stub_default;
display_f display;
// NOLINTEND

void read(const char* filename)
{
  std::unordered_set<std::string> refd;
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

    if (delim != "+")
    {
      last->second.insert(code, prompt);
      refd.insert(code);
    }
    else
    {
      const auto [iter, succ] = menu_lookup.emplace(
          std::piecewise_construct,
          std::forward_as_tuple(code),
          std::forward_as_tuple(menu_t::private_ctor_t {}, code, prompt));
      last = iter;
    }
  }

  for (const auto& ref : refd)
  {
    if (!menu_lookup.contains(ref))
    {
      free_lookup.emplace(ref, nullptr);
    }
  }
}

void insert(const char* code, const callback_f& callback)
{
  auto itr = free_lookup.find(code);
  if (itr == free_lookup.end())
  {
    std::cout << "Stamen: unknown callback registration...\n" << std::flush;
    return;
  }
  free_lookup.emplace(code, callback);
}

int dynamic(const char* code, const display_f& disp)
{
  display_stub_default = code;
  display              = disp;
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
    stack.push_back(&ml_it->second);
    const int ret = display(ml_it->second);
    stack.pop_back();

    return ret;
  }

  const auto fl_it = free_lookup.find(code);
  if (fl_it != free_lookup.end()) return fl_it->second(0);

  std::cout << "Stamen: nothing to do...\n" << std::flush;
  return 1;
}

void menu_t::insert(const std::string& code,
                    const std::string& prompt,
                    const callback_f& callback)
{
  char* buffer = new char[prompt.size() + 1];  // NOLINT
  strcpy(buffer, prompt.c_str());  // NOLINT

  m_items.emplace_back(callback, buffer);
  m_codes.emplace_back(code, prompt);
}

}  // namespace stamen
