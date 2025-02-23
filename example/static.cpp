#include <cstddef>
#include <iostream>

#include "demo_menu.hpp"

namespace example {

int menu_t::visit(const menu_t& menu)
{
  std::cout << menu.title << '\n';
  for (auto i = 0UL; i < menu.items.size(); i++)
  {
    std::cout << i + 1 << ": " << menu.items[i].prompt << '\n';
  }
  std::cout << "Auto calling option 1...\n";
  menu.items[1].callback(1);
  return 0;
}

int operation1(std::size_t /* unused */)
{
  std::cout << "operation 1\n\n";
  std::cout << "Some operation is done\n\n";
  std::cout << std::flush;
  return 1;
}

int operation2(std::size_t /* unused */)
{
  std::cout << "operation 2\n";
  std::cout << "Some other operation is done\n";
  std::cout << std::flush;
  return 1;
}

int operation3(std::size_t /* unused */)
{
  std::cout << "operation 3\n";
  std::cout << "Yet another operation is done\n";
  std::cout << std::flush;
  return 1;
}

int finish(std::size_t /* unused */)
{
  std::cout << "finishing...\n";
  std::cout << std::flush;
  exit(0);  // NOLINT
}

}  // namespace example

int main()
{
  example::menu_main(0);
  return 0;
}
