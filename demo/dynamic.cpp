#include <stamen.h>
#include <iostream>

const stamen_display_f stamen_display = stamen_builtin_display;

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
  stamen_read("./bin/demo_menu.conf");
  stamen_insert("finish", finish);
  stamen_insert("operation1", operation1);
  stamen_insert("operation2", operation2);
  stamen_insert("operation3", operation3);
  stamen_dynamic();
  return 0;
}
