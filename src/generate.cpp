#include "menu.h"
#include "stamen.h"

#include <format>
#include <fstream>
#include <iostream>
#include <string>

namespace stamen {

class Generator {
public:
  static void generateInclude(std::ostream &os) {
    os << "#ifndef STAMEN_MENU_H\n";
    os << "#define STAMEN_MENU_H\n\n";
    os << "#include <stamen.h>\n\n";
    for (const auto &[code, menu] : Menu::menu_lookup) {
      os << std::format("int {}(int);\n", menu.getCode());
    }
    os << "\n#endif\n";
  }

  static void generateSource(std::ostream &os) {
    os << "#include <stamen.h>\n";
    os << "#include \"shared.h\"\n\n";
    for (const auto &[code, menu] : Menu::menu_lookup) {
      os << std::format("int {}(int) {{\n", menu.getCode());

      os << std::format("\tstatic const stamen_item_t items[] = {{\n");
      for (int i = 0; i < menu.getSize(); i++) {
        os << std::format("\t\t{{ {}, \"{}\" }},\n", menu.getCode(i),
                          menu.getPrompt(i));
      }
      os << std::format("\t}};\n");

      os << std::format("\treturn stamen_display(\"{}\", items, "
                        "sizeof(items) / sizeof(items[0]));\n",
                        menu.getTitle());
      os << std::format("}}\n\n");
    }
  }
};

} // namespace stamen

using namespace stamen;

int main(const int argc, const char *argv[]) {
  const auto args = std::span(argv, size_t(argc));

  if (argc != 2 && argc != 3) {
    std::cout << argv[0] << " config_file [c/cpp]" << std::endl;
    return 1;
  }

  const bool cpp = argc == 2 || std::string(args[2]) == "cpp";

  std::string path = args[1];
  Menu::read(path);

  std::string::size_type pos = path.rfind('.');
  std::string base = pos != std::string::npos ? path.substr(0, pos) : path;
  std::string ext = cpp ? "pp" : "";

  std::ofstream source(base + ".c" + ext), include(base + ".h" + ext);
  Generator::generateSource(source);
  Generator::generateInclude(include);

  return 0;
}
