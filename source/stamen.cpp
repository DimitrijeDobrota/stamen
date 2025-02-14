#include <cmath>
#include <format>
#include <iostream>

#include "stamen/stamen.h"

extern "C"
{
namespace stamen {

int builtin_display(const char* title,
                    const stamen_item_t itemv[],
                    size_t size)
{
  const auto items  = std::span(itemv, size);
  const size_t dgts = static_cast<size_t>(std::log10(size)) + 1;
  int choice        = 0;

  while (true)
  {
    std::cout << std::format("{}:\n", title);
    for (auto i = 0UL; i < size; i++)
    {
      std::cout << std::format(" {:{}}. {}\n", i, dgts, items[i].prompt);
    }

    while (true)
    {
      std::cout << "Choose an option: ";
      if (std::cin >> choice && choice >= -1
          && choice < static_cast<int>(size))
      {
        if (choice == -1)
        {
          std::cout << "Choice: back\n";
          return 1;
        }

        const auto uchoice = static_cast<size_t>(choice);
        std::cout << "Choice: " << items[uchoice].prompt << "\n\n";
        const int res = items[uchoice].callback(uchoice);

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
    std::cout << std::endl;
  }

  return 1;
}

}  // namespace stamen
}
