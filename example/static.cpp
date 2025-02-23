#include <cmath>
#include <cstddef>
#include <format>
#include <iostream>
#include <limits>

#include "demo_menu.hpp"

namespace example {

int menu_t::visit(const menu_t& menu)
{
  const int sizei   = static_cast<int>(menu.items.size());
  const size_t dgts = static_cast<size_t>(std::log10(sizei)) + 1;
  int choice        = 0;

  while (true)
  {
    std::cout << std::format("{}:\n", menu.title);
    for (auto i = 0UL; i < menu.items.size(); i++)
    {
      std::cout << std::format(" {:{}}. {}\n", i, dgts, menu.items[i].prompt);
    }

    while (true)
    {
      std::cout << "Choose an option: ";
      if (std::cin >> choice && choice >= -1 && choice < sizei)
      {
        if (choice == -1)
        {
          std::cout << "Choice: back\n";
          return 1;
        }

        const auto uchoice = static_cast<size_t>(choice);
        std::cout << "Choice: " << menu.items[uchoice].prompt << "\n\n";
        const int res = menu.items[uchoice].callback(uchoice);

        if (res < 2) break;
        return res - 1;
      }

      if (std::cin.eof())
      {
        std::cerr << "encountered end of input!\n";
        return std::numeric_limits<int>::max();
      }

      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "Invalid option, please choose again!\n";
    }

    std::cout << '\n' << std::flush;
  }

  return 1;
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
