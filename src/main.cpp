#include "menu.h"
#include <iostream>

int algorithms(void) {
  std::cout << "algorithms" << std::endl;
  return 1;
}

int settings(void) {
  std::cout << "settings" << std::endl;
  return 1;
}

int finish(void) {
  std::cout << "finish" << std::endl;
  exit(0);
}

int main(void) {
  Menu menu("menu.json");
  menu.Register("algorithms", algorithms)
      .Register("settings", settings)
      .Register("finish", finish);
  menu.Start();
  return 0;
}
