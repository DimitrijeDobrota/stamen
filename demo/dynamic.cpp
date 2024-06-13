#include "stamen.hpp"
#include <iostream>

// need to link against stamen library in order to use stamen::builtin_display
const stamen::display_f stamen::stamen_display = stamen::builtin_display;

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
    stamen::read("./bin/demo_menu.conf");

    // register free functions
    stamen::insert("finish", finish);
    stamen::insert("operation1", operation1);
    stamen::insert("operation2", operation2);
    stamen::insert("operation3", operation3);

    // start the menu on specific panel
    stamen::dynamic("menu_main");
    return 0;
}
