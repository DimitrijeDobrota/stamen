#include "../include/stamen.h"
#include <cmath>
#include <format>
#include <fstream>
#include <iostream>
#include <ostream>
#include <stack>
#include <unordered_set>

std::unordered_map<std::string, Menu> Menu::lookup;

int Menu::builtinDisplay(const std::string &title, const item_t items[],
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

        const item_t &chosen = items[choice];
        std::cout << std::format("Choice: {}\n\n", chosen.getPrompt());
        const int res = chosen();
        if (res > 1) return res - 1;

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

void Menu::generateInclude(std::ostream &os) {
  os << "#ifndef STAMEN_MENU_H\n";
  os << "#define STAMEN_MENU_H\n\n";
  os << "#include <stamen.h>\n\n";
  os << "namespace stamen {\n\n";
  for (const auto &[code, _] : lookup) {
    const Menu &menu = getMenu(code);
    if (menu.callback) continue;
    os << std::format("int {}(void);\n", menu.code);
  }
  os << "\n}\n";
  os << "#endif\n";
}

void Menu::generateSource(std::ostream &os) {
  os << "#include <stamen.h>\n";
  os << "#include \"shared.h\"\n";
  os << "\nnamespace stamen {\n";
  for (const auto &[code, _] : lookup) {
    const Menu &menu = getMenu(code);
    if (menu.callback) continue;

    os << std::format("int {}(void) {{\n", menu.code);
    os << std::format("\tstatic const Menu::item_t items[] = {{\n");
    for (const auto &item : menu.items) {
      os << std::format("\t\t{{{}, \"{}\"}},\n", item.code, item.prompt);
    }
    os << std::format("\t}};\n");
    os << std::format("\treturn Menu::display(\"{}\", items, "
                      "sizeof(items) / sizeof(Menu::item_t));\n",
                      menu.title);
    os << std::format("}}\n\n");
  }
  os << "\n}\n";
}

void Menu::print(const std::string &code, const int depth) {
  const auto it = lookup.find(code);
  if (it == lookup.end()) return;
  const Menu &menu = it->second;

  if (depth == 1) std::cout << std::format("{}({})\n", menu.title, code);

  if (!menu.callback) {
    for (const auto &item : menu.items) {
      std::cout << std::format("{}{}({})\n", std::string(depth << 1, ' '),
                               item.prompt, item.code);
      menu.print(item.code, depth + 1);
    }
  }
}
