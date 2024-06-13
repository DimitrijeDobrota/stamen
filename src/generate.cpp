#include "menu.h"
#include "stamen.h"

#include <format>
#include <fstream>
#include <iostream>
#include <string>

namespace stamen {

class Generator {
public:
  static void generateInclude(std::ostream &os, bool cpp) {
    os << "#ifndef STAMEN_MENU_H\n";
    os << "#define STAMEN_MENU_H\n\n";

    if (cpp) os << "#include \"stamen.hpp\"\n\n";
    else os << "#include \"stamen.h\"\n\n";

    for (const auto &[code, menu] : Menu::menu_lookup) {
      os << std::format("int {}(int);\n", menu.getCode());
    }

    os << "\n#endif\n";
  }

  static void generateSource(std::ostream &os, bool cpp) {
    if (cpp) os << "#include \"stamen.hpp\"\n\n";
    else os << "#include \"stamen.h\"\n\n";

    os << "#include \"shared.h\"\n\n";
    for (const auto &[code, menu] : Menu::menu_lookup) {
      os << std::format("int {}(int) {{\n", menu.getCode());

      if (cpp) os << "\tstatic const stamen::item_t items[] = ";
      else os << "\tstatic const stamen_item_t items[] = ";

      os << "{\n";
      for (int i = 0; i < menu.getSize(); i++) {
        os << "\t\t{ " << menu.getCode(i);
        os << ", \"" << menu.getPrompt(i) << "\" },\n";
      }
      os << "\t};\n";

      if (cpp) os << "\treturn stamen::display";
      else os << "\treturn stamen_display";

      os << std::format("(\"{}\"", menu.getTitle());
      os << ", items, sizeof(items) / sizeof(items[0]));\n";
      os << "}\n\n";
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
  Generator::generateSource(source, cpp);
  Generator::generateInclude(include, cpp);

  return 0;
}
