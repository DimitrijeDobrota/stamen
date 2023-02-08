#include "menu.h"
#include <fstream>
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
  std::fstream f("menu.json");
  Menu::Register("algorithms", algorithms);
  Menu::Read(f);
  Menu::Register("settings", settings);
  Menu::Register("finish", finish);
  Menu::Start();
  return 0;
}
