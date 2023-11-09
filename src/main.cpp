#include "menu.h"
#include <iostream>

int algorithms(void) {
  std::cout << "algorithms" << std::endl;
  std::cout << "nothing to do here" << std::endl;
  return 1;
}

int settings(void) {
  std::cout << "settings" << std::endl;
  std::cout << "nothing to do here" << std::endl;
  return 1;
}

int finish(void) {
  std::cout << "finish" << std::endl;
  exit(0);
}

int foo(void) { exit(1); }

int main(void) {
  Menu::read("menu.json");
  // Menu::insert("algorithms", algorithms);
  // Menu::insert("settings", settings);
  // Menu::insert("finish", finish);
  Menu::generate();
  // Menu::print();
  // Menu::start();

  return 0;
}
