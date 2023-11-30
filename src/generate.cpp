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

  static void generateInclude(std::ostream &os, bool cpp) {
    os << "#ifndef STAMEN_MENU_H\n";
    os << "#define STAMEN_MENU_H\n\n";
    os << std::format("#include <stamen{}.h>\n\n", !cpp ? "c" : "");
    if (cpp) os << "namespace stamen {\n\n";
    for (const auto &[code, _] : Menu::getLookup()) {
      const Menu *menu = Menu::getMenu(code);
      if (!menu) throw EGenerate();
      if (menu->callback) continue;
      os << std::format("int {}(void);\n", menu->code);
    }
    if (cpp) os << "\n}\n";
    os << "#endif\n";
  }

  static void generateSource(std::ostream &os, bool cpp) {
    os << std::format("#include <stamen{}.h>\n", !cpp ? "c" : "");
    os << "#include \"shared.h\"\n\n";
    if (cpp) os << "namespace stamen {\n";
    for (const auto &[code, _] : Menu::getLookup()) {
      const Menu *menu = Menu::getMenu(code);
      if (!menu) throw EGenerate();
      if (menu->callback) continue;

      os << std::format("int {}(void) {{\n", menu->code);
      os << std::format("\tstatic const {}item_t items[] = {{\n",
                        cpp ? "Menu::" : "");
      for (const auto &item : menu->items) {
        os << std::format("\t\t{{ {}, \"{}\" }},\n", item.getCode(),
                          item.getPrompt());
      }
      os << std::format("\t}};\n");
      os << std::format("\treturn {0}display(\"{1}\", items, "
                        "sizeof(items) / sizeof({0}item_t));\n",
                        cpp ? "Menu::" : "", menu->title);
      os << std::format("}}\n\n");
    }
    if (cpp) os << "\n}\n";
  }
};

} // namespace stamen

int main(const int argc, const char *argv[]) {
  if (argc != 2 && argc != 3) {
    std::cout << "please enter exaclty one config file" << std::endl;
    return 1;
  }

  const bool cpp = argc == 2 || std::string(argv[2]) == "cpp";

  std::string path = argv[1];
  Menu::read(path);

  std::string::size_type pos = path.rfind('.');
  std::string base = pos != std::string::npos ? path.substr(0, pos) : path;
  std::string ext = cpp ? "pp" : "";

  std::ofstream source(base + ".c" + ext), include(base + ".h" + ext);
  Generator::generateSource(source, cpp);
  Generator::generateInclude(include, cpp);

  return 0;
}
