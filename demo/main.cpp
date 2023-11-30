#include "demo_menu.hpp"
#include "stamen.h"

#include <iostream>

// need to link against stamen library
// in order to use stamen::BuiltinDisplay
const stamen_display_f stamen_display = stamen::builtinDisplay;

int operation1(void) {
  std::cout << "operation 1" << std::endl;
  std::cout << "Some operation is done" << std::endl;
  return 1;
}

int operation2(void) {
  std::cout << "operation 2" << std::endl;
  std::cout << "Some other operation is done" << std::endl;
  return 1;
}

int operation3(void) {
  std::cout << "operation 3" << std::endl;
  std::cout << "Yet another operation is done" << std::endl;
  return 1;
}

int finish(void) {
  std::cout << "finishing..." << std::endl;
  exit(0);
}

int main(void) {
  stamen::menu_main();
  return 0;
}
