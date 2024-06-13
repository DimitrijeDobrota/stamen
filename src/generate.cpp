#include <args/args.hpp>

#include "menu.h"
#include "stamen.h"

#include <format>
#include <fstream>
#include <iostream>
#include <string>

struct arguments_t {
    std::string config;
    bool cpp = false;
    bool user = false;
} opt;

void generateIncludeHeaders(std::ostream &os) {
    if (opt.user) {
        if (opt.cpp) os << "#include \"stamen.hpp\"\n\n";
        else os << "#include \"stamen.h\"\n\n";
    } else {
        if (opt.cpp) os << "#include <stamen/stamen.hpp>\n\n";
        else os << "#include <stamen/stamen.h>\n\n";
    }
}

void generateInclude(std::ostream &os) {
    os << "#ifndef STAMEN_MENU_H\n";
    os << "#define STAMEN_MENU_H\n\n";

    generateIncludeHeaders(os);

    for (const auto &[code, menu] : stamen::Menu::menu_lookup) {
        os << std::format("int {}(int);\n", menu.getCode());
    }

    os << "\n#endif\n";
}

void generateSource(std::ostream &os) {
    os << "#include \"shared.h\"\n";
    generateIncludeHeaders(os);

    for (const auto &[code, menu] : stamen::Menu::menu_lookup) {
        os << std::format("int {}(int) {{\n", menu.getCode());

        if (opt.cpp) os << "\tstatic const stamen::item_t items[] = ";
        else os << "\tstatic const stamen_item_t items[] = ";

        os << "{\n";
        for (int i = 0; i < menu.getSize(); i++) {
            os << "\t\t{ " << menu.getCode(i);
            os << ", \"" << menu.getPrompt(i) << "\" },\n";
        }
        os << "\t};\n";

        if (opt.cpp) os << "\treturn stamen::stamen_display";
        else os << "\treturn stamen_display";

        os << std::format("(\"{}\"", menu.getTitle());
        os << ", items, sizeof(items) / sizeof(items[0]));\n";
        os << "}\n\n";
    }
}

int parse_opt(int key, const char *arg, args::Parser *parser) {
    auto arguments = (arguments_t *)parser->input();
    switch (key) {
    case 'u': arguments->user = true; break;
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
    {0, 0, 0, 0, "Output settings", 2},
    {"user", 'u', 0, 0, "Include user stamen headers"},
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
    if (args::parse(&argp, argc, argv, 0, &opt)) {
        std::cerr << "There was an error while parsing arguments";
        return 0;
    }

    const auto &config = opt.config;
    stamen::Menu::read(config);

    std::string::size_type pos = opt.config.rfind('.');
    std::string base =
        pos != std::string::npos ? config.substr(0, pos) : config;
    std::string ext = opt.cpp ? "pp" : "";

    std::ofstream source(base + ".c" + ext), include(base + ".h" + ext);
    generateInclude(include);
    generateSource(source);

    return 0;
}
