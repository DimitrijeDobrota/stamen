#include "demo_menu.hpp"
#include "stamen.h"

#include <iostream>

int test_display(const char *title, const stamen::item_t itemv[], int size) {
    for (auto i = 0ul; i < size; i++) {
        std::cout << i + 1 << ": " << itemv[i].prompt << '\n';
    }
    std::cout << "Auto calling option 1...\n";
    itemv[1].callback(1);
    return 0;
}

int operation1(int) {
    std::cout << "operation 1" << std::endl;
    std::cout << "Some operation is done" << std::endl;
    return 1;
}

int operation2(int) {
    std::cout << "operation 2" << std::endl;
    std::cout << "Some other operation is done" << std::endl;
    return 1;
}

int operation3(int) {
    std::cout << "operation 3" << std::endl;
    std::cout << "Yet another operation is done" << std::endl;
    return 1;
}

int finish(int) {
    std::cout << "finishing..." << std::endl;
    exit(0);
}

int main() {
    menu_main(0);
    return 0;
}
