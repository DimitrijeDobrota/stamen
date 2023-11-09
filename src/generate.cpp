#include "json.hpp"
#include <format>
#include <fstream>
#include <iostream>
#include <string>

using json = nlohmann::json;

int main(const int argc, const char *argv[]) {
  for (int i = 1; i < argc; i++) {
    const char *filename = argv[i];
    for (const auto &menu : json::parse(std::fstream(filename))) {
      std::cout << std::format("{}({}):\n", (std::string)menu["name"],
                               (std::string)menu["code"]);
      for (const auto &item : menu["items"]) {
        std::cout << std::format("* {}({})\n", (std::string)item["prompt"],
                                 (std::string)item["callback"]);
      }
    }
  }
  return 0;
}
