#include "stamen.hpp"
#include "menu.h"

#include <cmath>
#include <format>
#include <iostream>
#include <ostream>
#include <variant>

namespace stamen {

int builtin_display(const char *title, const item_t itemv[], int size) {
    const auto items = std::span(itemv, size_t(size));
    const size_t dgts = size_t(std::log10(size)) + 1;
    int choice = 0;

    while (true) {
        std::cout << std::format("{}:\n", title);
        for (auto i = 0ul; i < size; i++) {
            std::cout << std::format(" {:{}}. {}\n", i, dgts, items[i].prompt);
        }

        while (true) {
            std::cout << "Choose an option: ";
            if (std::cin >> choice && choice >= -1 && choice < (int)size) {
                if (choice == -1) {
                    std::cout << "Choice: back\n";
                    return 1;
                }

                std::cout << "Choice: " << items[choice].prompt << "\n\n";
                const int res = items[choice].callback(choice);

                if (res < 2) break;
                return res - 1;
            } else if (std::cin.eof()) {
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

} // namespace stamen
