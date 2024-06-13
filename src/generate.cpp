#include <args/args.hpp>

#include "menu.h"
#include "stamen.h"

#include <format>
#include <fstream>
#include <iostream>
#include <string>

namespace stamen {

const stamen_display_f stamen_display = stamen::builtin_display;

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

            if (cpp) os << "\treturn stamen::stamen_display";
            else os << "\treturn stamen_display";

            os << std::format("(\"{}\"", menu.getTitle());
            os << ", items, sizeof(items) / sizeof(items[0]));\n";
            os << "}\n\n";
        }
    }
};
} // namespace stamen

struct arguments_t {
    std::string config;
    bool cpp = false;
};

int parse_opt(int key, const char *arg, args::Parser *parser) {
    auto arguments = (arguments_t *)parser->input();
    switch (key) {
    case 666: arguments->cpp = false; break;
    case 777: arguments->cpp = true; break;
    case args::ARG:
        if (!arguments->config.empty()) {
            args::failure(parser, 1, 0, "Too many arguments");
        }
        arguments->config = arg;
        break;
    case args::NO_ARGS:
        args::failure(parser, 0, 0, "Missing an argument");
        args::help(parser, stderr, args::STD_USAGE);
    }
    return 0;
}

static const args::option_t options[]{
    {0, 0, 0, 0, "Output mode", 1},
    {"c", 666, 0, 0, "Generate files for C"},
    {"cpp", 777, 0, 0, "Generate files for C++"},
    {0, 0, 0, 0, "Informational Options", -1},
    {0},
};

static const args::argp_t argp{
    options,
    parse_opt,
    "config_file",
    "",
};

int main(int argc, char *argv[]) {
    arguments_t arguments;
    if (args::parse(&argp, argc, argv, 0, &arguments)) {
        std::cerr << "There was an error while parsing arguments";
        return 0;
    }

    const auto &config = arguments.config;
    stamen::Menu::read(config);

    std::string::size_type pos = arguments.config.rfind('.');
    std::string base =
        pos != std::string::npos ? config.substr(0, pos) : config;
    std::string ext = arguments.cpp ? "pp" : "";

    std::ofstream source(base + ".c" + ext), include(base + ".h" + ext);
    stamen::Generator::generateSource(source, arguments.cpp);
    stamen::Generator::generateInclude(include, arguments.cpp);

    return 0;
}
