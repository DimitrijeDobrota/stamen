#include "menu.h"

int main(const int argc, const char *argv[]) {
  for (int i = 1; i < argc; i++) Menu::read(argv[i]);
  Menu::generate();
  return 0;
}
