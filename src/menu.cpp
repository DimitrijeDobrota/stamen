#include "menu.h"

void Menu::Read(std::istream &is) {
  json file = json::parse(is);
  for (auto &menu : file) {
    const json &items = menu["items"];
    const Menu_function *mf =
        new Menu_function(*this, menu["name"], {items.begin(), items.end()});
    function_lookup.insert({menu["code"], Menu_callback(mf)});
  }
}
