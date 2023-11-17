#include "../include/stamen.h"
#include <cmath>
#include <format>
#include <iostream>
#include <ostream>

namespace stamen {

int builtinDisplay(const std::string &title, const Menu::item_t items[],
                   std::size_t size) {
  int choice;
  const int digits = std::log10(size) + 1;
  while (true) {
    std::cout << std::format("{}:\n", title);
    for (auto i = 0ul; i < size; i++) {
      std::cout << std::format(" {:{}}. {}\n", i, digits, items[i].getPrompt());
    }

    while (true) {
      std::cout << "Choose an option: ";
      if (std::cin >> choice && choice >= -1 && choice < (int)size) {
        if (choice == -1) {
          std::cout << "Choice: back\n";
          return 1;
        }

        const Menu::item_t &chosen = items[choice];
        std::cout << std::format("Choice: {}\n\n", chosen.getPrompt());
        const int res = chosen();
        if (res > 1) return res - 1;
        else break;

      } else if (std::cin.eof()) {
        std::cerr << "encountered end of input!\n";
        return std::numeric_limits<int>::max();
      } else {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
      std::cout << "Invalid option, please choose again!\n";
    }
    std::cout << std::endl;
  }

  return 1;
}

} // namespace stamen
