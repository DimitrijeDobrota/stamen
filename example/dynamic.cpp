#include <iostream>
#include <span>

#include "stamen/menu.hpp"

int finish(size_t /* unused */)
{
  exit(0);
}

int operation1(size_t /* unused */)
{
  std::cout << "1" << std::endl;
  return 1;
}

int operation2(size_t /* unused */)
{
  std::cout << "2" << std::endl;
  return 1;
}

int operation3(size_t /* unused */)
{
  std::cout << "3" << std::endl;
  return 1;
}

int main(int argc, char* argv[])
{
  const std::span args(argv, argv + argc);

  // read the configuration
  for (const auto& arg : args.subspan(1)) stamen::menu::read(arg);

  // register free functions
  stamen::menu::insert("finish", finish);
  stamen::menu::insert("operation1", operation1);
  stamen::menu::insert("operation2", operation2);
  stamen::menu::insert("operation3", operation3);

  // start the menu on specific panel
  stamen::menu::dynamic("menu_main", stamen::builtin_display);

  return 0;
}
