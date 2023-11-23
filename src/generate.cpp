#include "stamen.h"
#include <string>

using namespace stamen;

namespace stamen {

class Generator {
public:
  class EGenerate : std::exception {
    virtual const char *what() const noexcept override {
      return "Trying to access unknown code";
    }
  };

  static void generateInclude(std::ostream &os) {
    os << "#ifndef STAMEN_MENU_H\n";
    os << "#define STAMEN_MENU_H\n\n";
    os << "#include <stamen.h>\n\n";
    os << "namespace stamen {\n\n";
    for (const auto &[code, _] : Menu::getLookup()) {
      const Menu *menu = Menu::getMenu(code);
      if (!menu) throw EGenerate();
      if (menu->callback) continue;
      os << std::format("int {}(void);\n", menu->code);
    }
    os << "\n}\n";
    os << "#endif\n";
  }

  static void generateSource(std::ostream &os) {
    os << "#include <stamen.h>\n";
    os << "#include \"shared.h\"\n";
    os << "\nnamespace stamen {\n";
    for (const auto &[code, _] : Menu::getLookup()) {
      const Menu *menu = Menu::getMenu(code);
      if (!menu) throw EGenerate();
      if (menu->callback) continue;

      os << std::format("int {}(void) {{\n", menu->code);
      os << std::format("\tstatic const Menu::item_t items[] = {{\n");
      for (const auto &item : menu->items) {
        os << std::format("\t\t{{{}, \"{}\"}},\n", item.getCode(),
                          item.getPrompt());
      }
      os << std::format("\t}};\n");
      os << std::format("\treturn Menu::display(\"{}\", items, "
                        "sizeof(items) / sizeof(Menu::item_t));\n",
                        menu->title);
      os << std::format("}}\n\n");
    }
    os << "\n}\n";
  }
};

} // namespace stamen

int main(const int argc, const char *argv[]) {
  if (argc != 2) {
    std::cout << "please enter exaclty one config file" << std::endl;
    return 1;
  }

  std::string path = argv[1];
  Menu::read(path);

  std::string::size_type pos = path.rfind('.');
  std::string base = pos != std::string::npos ? path.substr(0, pos) : path;
  std::ofstream cpp(base + ".cpp"), h(base + ".h");

  Generator::generateSource(cpp);
  Generator::generateInclude(h);

  return 0;
}
