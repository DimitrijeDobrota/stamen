#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <utility>

#include "stamen/stamen.hpp"

namespace {

void warning(const std::string& message, const std::string& addition)
{
  std::cerr << "Stamen: " << message;
  if (!addition.empty())
  {
    std::cerr << ":  " + addition;
  }
  std::cerr << '\n' << std::flush;
}

}  // namespace

namespace stamen {

Stamen::Stamen(std::istream& ist)
{
  using namespace std::placeholders;  // NOLINT

  std::unordered_set<std::string> refd;
  std::string line;
  std::string delim;
  std::string code;
  std::string prompt;

  auto last = m_menu_lookup.end();
  while (std::getline(ist, line))
  {
    if (line.empty()) continue;

    std::istringstream iss(line);
    iss >> delim >> code >> std::ws;
    std::getline(iss, prompt);

    if (delim != "+")
    {
      last->second.insert(code,
                          prompt,
                          [&](std::size_t idx)
                          { return this->display_stub(idx); });
      refd.insert(code);
    }
    else
    {
      const auto [iter, succ] =
          m_menu_lookup.emplace(code, Menu(code, prompt));
      last = iter;
    }
  }

  for (const auto& ref : refd)
  {
    if (!m_menu_lookup.contains(ref))
    {
      m_free_lookup.emplace(ref, nullptr);
    }
  }
}

void Stamen::insert(const std::string& code, const callback_f& callback)
{
  auto itr = m_free_lookup.find(code);
  if (itr == m_free_lookup.end())
  {
    warning("unknown callback registration", code);
    return;
  }
  itr->second = callback;
}

int Stamen::dynamic(const std::string& code, const display_f& disp)
{
  m_stub_default = code;
  m_stub_display = disp;
  return display_stub(0);
}

int Stamen::display_stub(std::size_t idx)
{
  const std::string& code = !m_stub_stack.empty()
      ? m_stub_stack.top()->item(idx).code
      : m_stub_default;

  const auto mit = m_menu_lookup.find(code);
  if (mit != m_menu_lookup.end())
  {
    m_stub_stack.push(&mit->second);
    const int ret = m_stub_display(mit->second);
    m_stub_stack.pop();

    return ret;
  }

  const auto fit = m_free_lookup.find(code);
  if (fit != m_free_lookup.end() && fit->second != nullptr)
  {
    return fit->second(0);
  }

  warning("no callback for", code);

  return 1;
}

void Menu::insert(const std::string& code,
                    const std::string& prompt,
                    const callback_f& callback)
{
  m_items.emplace_back(code, prompt, callback);
}

}  // namespace stamen
