#include "demo_menu.hpp"
#include "stamen.h"

#include <iostream>

// need to link against stamen library
// in order to use stamen_builtin_display
const stamen_display_f stamen_display = stamen_builtin_display;

int operation1() {
  std::cout << "operation 1" << std::endl;
  std::cout << "Some operation is done" << std::endl;
  return 1;
}

int operation2() {
  std::cout << "operation 2" << std::endl;
  std::cout << "Some other operation is done" << std::endl;
  return 1;
}

int operation3() {
  std::cout << "operation 3" << std::endl;
  std::cout << "Yet another operation is done" << std::endl;
  return 1;
}

int finish() {
  std::cout << "finishing..." << std::endl;
  exit(0);
}

int main() {
  menu_main();
  return 0;
}
