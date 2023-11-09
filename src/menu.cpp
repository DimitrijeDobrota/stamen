#include "menu.h"
#include <cmath>
#include <format>
#include <iostream>
#include <stack>
#include <unordered_set>

std::unordered_map<std::string, Menu> Menu::lookup;
const std::string Menu::entry = "menu_main";

int Menu::display(const std::string &name, const item_t items[], int size) {
  int choice;
  const int digits = std::log10(size) + 1;
  while (true) {
    std::cout << std::format("{}:\n", name);
    for (auto i = 0ul; i < size; i++) {
      std::cout << std::format(" {:{}}. {}\n", i, digits, items[i].prompt);
    }

    while (true) {
      std::cout << "Choose an option: ";
      if (std::cin >> choice && choice >= -1 && choice < size) {
        if (choice == -1) {
          std::cout << "choice: back\n";
          return 1;
        }

        const item_t &chosen = items[choice];
        std::cout << std::format("Choice: {}\n\n", chosen.prompt);
        int res = chosen.func ? chosen.func() : getMenu(chosen.callback)();
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

void Menu::generate(const std::string &code) {
  std::unordered_set<std::string> seen;
  std::vector<std::string> order;
  std::stack<std::string> st;

  st.push(code);
  while (!st.empty()) {
    if (!st.top().empty()) {
      const std::string &code = st.top();
      st.push("");
      for (const auto &[_, code, func] : getMenu(code).items) {
        if (lookup.count(code)) st.push(code);
      }
      continue;
    }

    st.pop();
    const std::string &code = st.top();
    st.pop();

    if (seen.count(code)) continue;
    seen.insert(code);
    order.push_back(code);
  }

  for (const auto &code : order) {
    const Menu menu = getMenu(code);
    if (menu.callback) continue;

    std::cout << std::format("int {}(void) {{\n", menu.code);
    std::cout << std::format("\tstatic const Menu::item_t items[] = {{\n");
    for (const auto &item : menu.items) {
      std::cout << std::format("\t\t{{ \"{}\", \"{}\", {} }},\n", item.prompt,
                               item.callback, item.callback);
    }
    std::cout << std::format("\t}};\n");
    std::cout << std::format("\treturn Menu::display(\"{}\", items, "
                             "sizeof(items) / sizeof(Menu::item_t));\n",
                             menu.name);
    std::cout << std::format("}}\n\n");
  }
}

void Menu::print(const std::string &code, const int depth) {
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
