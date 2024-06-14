#include <iostream>

#include "menu.hpp"

int finish(int) { exit(1); }

int operation1(int) {
    std::cout << "1" << std::endl;
    return 1;
}

int operation2(int) {
    std::cout << "2" << std::endl;
    return 1;
}

int operation3(int) {
    std::cout << "3" << std::endl;
    return 1;
}

int main() {
    // read the configuration
    stamen::menu::read("./bin/demo_menu.conf");

    // register free functions
    stamen::menu::insert("finish", finish);
    stamen::menu::insert("operation1", operation1);
    stamen::menu::insert("operation2", operation2);
    stamen::menu::insert("operation3", operation3);

    // start the menu on specific panel
    stamen::menu::dynamic("menu_main", stamen::builtin_display);
    return 0;
}
