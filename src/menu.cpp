#include "menu.h"
#include <cmath>
#include <format>
#include <iostream>

std::unordered_map<std::string, Menu> Menu::lookup;

int Menu::operator()() const {
  if (callback) return callback();

  int choice;
  const int n = items.size(), digits = std::log10(n) + 1;
  while (true) {
    std::cout << std::format("{}:\n", name);
    for (auto i = 0ul; i < n; i++) {
      std::cout << std::format(" {:{}}. {}\n", i, digits, items[i].prompt);
    }

    while (true) {
      std::cout << "Choose an option: ";
      if (std::cin >> choice && choice >= -1 && choice < n) {
        if (choice == -1) {
          std::cout << "choice: back\n";
          return 1;
        }

        std::cout << std::format("Choice: {}\n\n", items[choice].prompt);
        int res = getMenu(items[choice].callback)();
        if (--res) return res;

        break;
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

void Menu::print(const std::string code, const int depth) {
  const auto it = lookup.find(code);
  if (it == lookup.end()) return;
  const Menu &menu = it->second;

  if (depth == 1) { std::cout << std::format("{}({})\n", menu.name, code); }

  if (!menu.callback) {
    for (const auto &item : menu.items) {
      std::cout << std::format("{}{}({})\n", std::string(depth << 1, ' '),
                               item.prompt, item.callback);
      menu.print(item.callback, depth + 1);
    }
  }
}
