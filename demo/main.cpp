#include <iostream>

#include "demo_menu.h"

const Menu::display_f Menu::display = Menu::builtinDisplay;

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

int menu_static_run(void) { return menu::menu_main(); }
int menu_dynamic_run(void) { return Menu::start("menu_main"); }

int menu_dynamic_print(void) {
  Menu::print("menu_main");
  return 1;
}

int main(int argc, const char *argv[]) {
  std::string name(argv[0]);
  std::string::size_type pos = name.rfind('/');
  std::string base = pos != std::string::npos ? name.substr(0, pos) : ".";

  Menu::read(base + "/demo_menu.conf");
  Menu::insert("finish", finish);
  Menu::insert("operation1", operation1);
  Menu::insert("operation2", operation2);
  Menu::insert("operation3", operation3);

  static const Menu::item_t items[] = {
      {   menu_static_run, "Run statically generated menu"},
      {  menu_dynamic_run,              "Run dynamic menu"},
      {menu_dynamic_print,            "Print dynamic menu"},
      {            finish,                          "Quit"},
  };
  Menu::display("Menu demo program", items,
                sizeof(items) / sizeof(Menu::item_t));
  return 0;
}
