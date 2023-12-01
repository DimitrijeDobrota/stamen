#include "stamen.h"
#include <string>

using namespace stamen;

namespace stamen {

class Generator {
public:
  class EGenerate : std::exception {
    [[nodiscard]] const char *what() const noexcept override {
      return "Trying to access unknown code";
    }
  };

  static void generateInclude(std::ostream &os, bool cpp) {
    os << "#ifndef STAMEN_MENU_H\n";
    os << "#define STAMEN_MENU_H\n\n";
    if (cpp) os << "namespace stamen {\n\n";
    for (const auto &[code, _] : Menu::getLookup()) {
      const Menu *menu = Menu::getMenu(code);
      if (!menu) throw EGenerate();
      if (menu->callback) continue;
      os << std::format("int {}(void);\n", menu->code);
    }
    if (cpp) os << "\n}\n";
    os << "\n#endif\n";
  }

  static void generateSource(std::ostream &os, bool cpp) {
    os << "#include <stamenc.h>\n";
    os << "#include \"shared.h\"\n\n";
    if (cpp) os << "namespace stamen {\n\n";
    for (const auto &[code, _] : Menu::getLookup()) {
      const Menu *menu = Menu::getMenu(code);
      if (!menu) throw EGenerate();
      if (menu->callback) continue;

      os << std::format("int {}(void) {{\n", menu->code);

      os << std::format("\tstatic const item_t items[] = {{\n");
      for (const auto &[code, prompt] : menu->items) {
        os << std::format("\t\t{{ {}, \"{}\" }},\n", code, prompt);
      }
      os << std::format("\t}};\n");

      os << std::format("\treturn stamen_display(\"{}\", items, "
                        "sizeof(items) / sizeof(item_t));\n",
                        menu->title);
      os << std::format("}}\n\n");
    }
    if (cpp) os << "}\n";
  }
};

} // namespace stamen

int main(const int argc, const char *argv[]) {
  const auto args = std::span(argv, size_t(argc));

  if (argc != 2 && argc != 3) {
    std::cout << "please enter exaclty one config file" << std::endl;
    return 1;
  }

  const bool cpp = argc == 2 || std::string(args[2]) == "cpp";

  std::string path = args[1];
  Menu::read(path);

  std::string::size_type pos = path.rfind('.');
  std::string base = pos != std::string::npos ? path.substr(0, pos) : path;
  std::string ext = cpp ? "pp" : "";

  std::ofstream source(base + ".c" + ext), include(base + ".h" + ext);
  Generator::generateSource(source, cpp);
  Generator::generateInclude(include, cpp);

  return 0;
}
