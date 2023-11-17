#include "stamen.h"
#include <string>

using namespace stamen;

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
  Menu::generateSource(cpp);
  Menu::generateInclude(h);

  return 0;
}
