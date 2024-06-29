#include <iostream>
#include <span>

#include "demo_menu.hpp"
#include "stamen/stamen.hpp"

int test_display(const char* title, const stamen::item_t itemv[], size_t size)
{
  const auto items = std::span(itemv, itemv + size);

  std::cout << title << std::endl;
  for (auto i = 0UL; i < size; i++)
  {
    std::cout << i + 1 << ": " << items[i].prompt << '\n';
  }
  std::cout << "Auto calling option 1...\n";
  items[1].callback(1);
  return 0;
}

int operation1(size_t /* unused */)
{
  std::cout << "operation 1" << std::endl;
  std::cout << "Some operation is done" << std::endl;
  return 1;
}

int operation2(size_t /* unused */)
{
  std::cout << "operation 2" << std::endl;
  std::cout << "Some other operation is done" << std::endl;
  return 1;
}

int operation3(size_t /* unused */)
{
  std::cout << "operation 3" << std::endl;
  std::cout << "Yet another operation is done" << std::endl;
  return 1;
}

int finish(size_t /* unused */)
{
  std::cout << "finishing..." << std::endl;
  exit(0);
}

int main()
{
  menu_main(0);
  return 0;
}
