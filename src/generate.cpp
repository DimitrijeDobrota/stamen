#include <poafloc/poafloc.hpp>

#include "menu.hpp"
#include "stamen.hpp"

#include <format>
#include <fstream>
#include <iostream>
#include <string>

struct arguments_t {
    std::string config;
    std::string display;
    std::string header = "shared.h";
    bool cpp = false;
    bool user = false;
} opt;

void generateIncludeHeaders(std::ostream &os) {}

void generateInclude(std::ostream &os) {
    os << "#ifndef STAMEN_MENU_H\n";
    os << "#define STAMEN_MENU_H\n\n";

    for (const auto &[code, menu] : stamen::menu::menu_lookup) {
        os << std::format("int {}(int);\n", menu.getCode());
    }

    os << "\n#endif\n";
}

void generateSource(std::ostream &os) {
    os << std::format("#include \"{}\"\n", opt.header);
    if (opt.user) {
        if (opt.cpp) os << "#include \"stamen.hpp\"\n\n";
        else os << "#include \"stamen.h\"\n\n";
    } else {
        if (opt.cpp) os << "#include <stamen/stamen.hpp>\n\n";
        else os << "#include <stamen/stamen.h>\n\n";
    }

    os << std::format("extern int {}(const char *title, ", opt.display);
    if (opt.cpp) os << "const stamen::item_t itemv[], int size);\n\n";
    else os << "const stamen_item_t itemv[], int size);\n\n";

    for (const auto &[code, menu] : stamen::menu::menu_lookup) {
        os << std::format("int {}(int) {{\n", menu.getCode());

        if (opt.cpp) os << "\tstatic const stamen::item_t items[] = ";
        else os << "\tstatic const stamen_item_t items[] = ";

        os << "{\n";
        for (int i = 0; i < menu.getSize(); i++) {
            os << "\t\t{ " << menu.getCode(i);
            os << ", \"" << menu.getPrompt(i) << "\" },\n";
        }
        os << "\t};\n";

        os << std::format("\treturn {}(\"{}\"", opt.display, menu.getTitle());
        os << ", items, sizeof(items) / sizeof(items[0]));\n";
        os << "}\n\n";
    }
}

int parse_opt(int key, const char *arg, poafloc::Parser *parser) {
    auto arguments = (arguments_t *)parser->input();
    switch (key) {
    case 'd': arguments->display = arg; break;
    case 'h': arguments->header = arg; break;
    case 'u': arguments->user = true; break;
    case 666: arguments->cpp = false; break;
    case 777: arguments->cpp = true; break;
    case poafloc::ARG:
        if (!arguments->config.empty()) {
            poafloc::failure(parser, 0, 0, "Too many arguments");
            poafloc::help(parser, stderr, poafloc::STD_USAGE);
        }
        arguments->config = arg;
        break;
    case poafloc::NO_ARGS:
        poafloc::failure(parser, 0, 0, "Missing an argument");
        poafloc::help(parser, stderr, poafloc::STD_USAGE);
    case poafloc::END:
        if (arguments->display.empty()) {
            if (arguments->cpp) arguments->display = "stamen::builtin_display";
            else arguments->display = "stamen_builtin_display";
        }
        break;
    }
    return 0;
}

static const poafloc::option_t options[]{
    {0, 0, 0, 0, "Output mode", 1},
    {"c", 666, 0, 0, "Generate files for C"},
    {"cpp", 777, 0, 0, "Generate files for C++"},
    {0, 0, 0, 0, "Output settings", 2},
    {"display", 'd', "FUNC", 0, "Set display function to be called"},
    {"user", 'u', 0, 0, "Include user stamen headers"},
    {"header", 'h', "FUNC", 0,
     "Header with free functions, default: shared.h"},
    {0, 0, 0, 0, "Informational Options", -1},
    {0},
};

static const poafloc::arg_t arg{
    options,
    parse_opt,
    "config_file",
    "",
};

int main(int argc, char *argv[]) {
    if (poafloc::parse(&arg, argc, argv, 0, &opt)) {
        std::cerr << "There was an error while parsing arguments";
        return 0;
    }

    const auto &config = opt.config;
    stamen::menu::read(config.c_str());

    std::string::size_type pos = opt.config.rfind('.');
    std::string base =
        pos != std::string::npos ? config.substr(0, pos) : config;
    std::string ext = opt.cpp ? "pp" : "";

    std::ofstream source(base + ".c" + ext), include(base + ".h" + ext);
    generateInclude(include);
    generateSource(source);

    return 0;
}
