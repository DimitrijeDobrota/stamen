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

int main(void) {
  Menu menu("menu.json");
  menu.insert({
      {"algorithms", algorithms},
      {  "settings",   settings},
      {    "finish",     finish}
  });
  menu();
  return 0;
}
