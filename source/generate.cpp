#include <format>
#include <fstream>
#include <iostream>
#include <string>

#include "poafloc/poafloc.hpp"
#include "stamen/menu.hpp"
#include "stamen/stamen.hpp"

struct arguments_t
{
  std::string config;
  std::string display;
  std::string header = "shared.h";
  bool cpp           = false;
  bool user          = false;
};

void generate_include(std::ostream& ost)
{
  ost << "#ifndef STAMEN_MENU_H\n";
  ost << "#define STAMEN_MENU_H\n\n";

  for (const auto& [code, menu] : stamen::menu::menu_lookup)
  {
    ost << std::format("int {}(size_t);\n", menu.get_code());
  }

  ost << "\n#endif\n";
}

void generate_source(std::ostream& ost, const arguments_t& args)
{
  ost << std::format("#include \"{}\"\n", args.header);
  if (args.user)
  {
    if (args.cpp) ost << "#include \"stamen.hpp\"\n\n";
    else ost << "#include \"stamen.h\"\n\n";
  }
  else
  {
    if (args.cpp) ost << "#include <stamen/stamen.hpp>\n\n";
    else ost << "#include <stamen/stamen.h>\n\n";
  }

  ost << std::format("extern int {}(const char *title, ", args.display);
  if (args.cpp) ost << "const stamen::item_t itemv[], size_t size);\n\n";
  else ost << "const stamen_item_t itemv[], size_t size);\n\n";

  for (const auto& [code, menu] : stamen::menu::menu_lookup)
  {
    ost << std::format("int {}(size_t /* unused */) {{\n", menu.get_code());

    if (args.cpp) ost << "\tstatic const stamen::item_t items[] = ";
    else ost << "\tstatic const stamen_item_t items[] = ";

    ost << "{\n";
    for (auto i = 0UL; i < menu.get_size(); i++)
    {
      ost << "\t\t{ " << menu.get_code(i);
      ost << ", \"" << menu.get_prompt(i) << "\" },\n";
    }
    ost << "\t};\n";

    ost << std::format("\treturn {}(\"{}\"", args.display, menu.get_title());
    ost << ", items, sizeof(items) / sizeof(items[0]));\n";
    ost << "}\n\n";
  }
}

int parse_opt(int key, const char* arg, poafloc::Parser* parser)
{
  auto* arguments = static_cast<arguments_t*>(parser->input());
  switch (key)
  {
    case 'd':
      arguments->display = arg;
      break;
    case 'h':
      arguments->header = arg;
      break;
    case 'u':
      arguments->user = true;
      break;
    case 666:
      arguments->cpp = false;
      break;
    case 777:
      arguments->cpp = true;
      break;
    case poafloc::ARG:
      if (!arguments->config.empty())
      {
        poafloc::failure(parser, 0, 0, "Too many arguments");
        poafloc::help(parser, stderr, poafloc::STD_USAGE);
      }
      arguments->config = arg;
      break;
    case poafloc::NO_ARGS:
      poafloc::failure(parser, 0, 0, "Missing an argument");
      poafloc::help(parser, stderr, poafloc::STD_USAGE);
      break;
    case poafloc::END:
      if (arguments->display.empty())
      {
        if (arguments->cpp) arguments->display = "stamen::builtin_display";
        else arguments->display = "stamen_builtin_display";
      }
      break;
    default:
      break;
  }
  return 0;
}

static const poafloc::option_t options[] {
    {nullptr, 0, nullptr, 0, "Output mode", 1},
    {"c", 666, nullptr, 0, "Generate files for C"},
    {"cpp", 777, nullptr, 0, "Generate files for C++"},
    {nullptr, 0, nullptr, 0, "Output settings", 2},
    {"display", 'd', "FUNC", 0, "Set display function to be called"},
    {"user", 'u', nullptr, 0, "Include user stamen headers"},
    {"header", 'h', "HDR", 0, "Header with free functions, default: shared.h"},
    {nullptr, 0, nullptr, 0, "Informational Options", -1},
    {nullptr},
};

static const poafloc::arg_t arg {
    options,
    parse_opt,
    "config_file",
    "",
};

int main(int argc, char* argv[])
{
  arguments_t args;

  if (poafloc::parse(&arg, argc, argv, 0, &args) != 0)
  {
    std::cerr << "There was an error while parsing arguments";
    return 1;
  }

  const auto& config = args.config;
  stamen::menu::read(config.c_str());

  const std::string::size_type pos = args.config.rfind('.');
  const std::string ext            = args.cpp ? "pp" : "";
  const std::string base =
      pos != std::string::npos ? config.substr(0, pos) : config;

  std::ofstream include(base + ".h" + ext);
  generate_include(include);

  std::ofstream source(base + ".c" + ext);
  generate_source(source, args);

  return 0;
}
