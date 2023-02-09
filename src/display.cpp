#include "menu.h"
#include <iostream>

int Menu::Menu_function::display() const {
  unsigned long choice;
  while (true) {
    std::cout << name << std::endl;

    for (auto i = 0ul; i < items.size(); i++)
      std::cout << i << ". " << items[i].prompt << std::endl;

    while (true) {
      std::cout << "Choose an option: ";
      if (std::cin >> choice && choice < items.size()) {
        std::cout << "Chosen: " << items[choice].prompt << "\n\n";
        int res = menu.get_callback(items[choice].callback)();
        if (--res) return res;
        break;
      } else if (std::cin.eof()) {
        std::cerr << "encountered end of input!\n";
        return std::numeric_limits<int>::max();
      } else {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
      std::cout << "Invalid option, please choose again!" << std::endl;
    }
    std::cout << std::endl;
  }

  return 1;
}
