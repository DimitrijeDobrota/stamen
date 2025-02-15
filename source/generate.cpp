#include <format>
#include <fstream>
#include <iostream>
#include <string>

#include "poafloc/poafloc.hpp"
#include "stamen/menu.hpp"

struct arguments_t
{
  std::string config;
  std::string display;
  std::string nspace = "stamen";
  bool cpp           = false;
  bool user          = false;
};

namespace {

void generate_include_c(std::ostream& ost, const arguments_t& args)
{
  ost << "#ifndef STAMEN_MENU_H\n";
  ost << "#define STAMEN_MENU_H\n\n";

  for (const auto& [code, menu] : stamen::menu::menu_lookup)
  {
    ost << std::format("int {}(size_t /* unused */);\n", menu.get_code());
  }

  ost << "\n#endif\n";
  (void)args;
}

void generate_source_c(std::ostream& ost, const arguments_t& args)
{
  if (args.user)
  {
    ost << "#include \"stamen.h\"\n\n";
  }
  else
  {
    ost << "#include <stamen/stamen.h>\n\n";
  }

  ost << std::format("extern int {}(const char *title, ", args.display);
  ost << "const stamen_item_t itemv[], size_t size);\n\n";

  for (const auto& [code, _] : stamen::menu::free_lookup)
  {
    ost << std::format("extern int {}(size_t);\n", code);
  }
  ost << '\n';

  for (const auto& [code, menu] : stamen::menu::menu_lookup)
  {
    ost << std::format("int {}(size_t /* unused */) {{\n", menu.get_code());

    ost << "\tstatic const stamen_item_t items[] = ";

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

void generate_include_cpp(std::ostream& ost, const arguments_t& args)
{
  ost << "#pragma once\n\n";

  ost << "#include <cstddef>\n";
  ost << "#include <string>\n";
  ost << "#include <vector>\n\n";

  ost << (args.user ? "#include \"stamen.hpp\"\n\n"
                    : "#include <stamen/stamen.hpp>\n\n");

  ost << std::format("namespace {}\n{{\n", args.nspace);

  ost << R"(
struct menu_t {
  std::string title;
  stamen::callback_f callback;
  std::vector<stamen::item_t> items;

  static int visit(const menu_t& menu);
};

)";

  for (const auto& [code, menu] : stamen::menu::menu_lookup)
  {
    ost << std::format("int {}(std::size_t /* unused */);\n", menu.get_code());
  }

  ost << std::format("\n}} // namespace {}\n", args.nspace);
}

void generate_source_cpp(std::ostream& ost,
                         const arguments_t& args,
                         const std::string& include)
{
  ost << "#include <cstddef>\n\n";

  ost << std::format("#include \"{}\"\n\n", include);

  ost << std::format("namespace {}\n{{\n\n", args.nspace);

  ost << std::format("extern int {}(const char *title, ", args.display);
  ost << "const stamen::item_t itemv[], size_t size);\n\n";

  for (const auto& [code, _] : stamen::menu::free_lookup)
  {
    ost << std::format("extern int {}(std::size_t);\n", code);
  }
  ost << '\n';

  for (const auto& [code, menu] : stamen::menu::menu_lookup)
  {
    ost << std::format("int {}(size_t /* unused */) //  NOLINT\n{{\n",
                       menu.get_code());

    ost << "\tstatic const menu_t menu = {\n";
    ost << std::format("\t\t.title = \"{}\",\n", menu.get_title());
    ost << std::format("\t\t.callback = {},\n", menu.get_code());
    ost << std::format("\t\t.items = {{\n");
    for (auto i = 0UL; i < menu.get_size(); i++)
    {
      ost << std::format("\t\t\t{{.callback = {}, .prompt = \"{}\"}},\n",
                         menu.get_code(i),
                         menu.get_prompt(i));
    }
    ost << "\t\t}\n\t};\n\n";

    ost << "\treturn menu_t::visit(menu);\n";

    ost << "}\n\n";
  }

  ost << std::format("\n}} // namespace {}\n", args.nspace);
}

int parse_opt(int key, const char* arg, poafloc::Parser* parser)
{
  auto* arguments = static_cast<arguments_t*>(parser->input());
  switch (key)
  {
    case 'd':
      arguments->display = arg;
      break;
    case 'n':
      if (!arguments->cpp)
      {
        poafloc::failure(parser, 0, 0, "Namespace only available in C++ mode");
        poafloc::help(parser, stderr, poafloc::STD_USAGE);
        break;
      }
      arguments->nspace = arg;
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

}  // namespace

// clang-format off
static const poafloc::option_t options[] {
    {nullptr, 0, nullptr, 0, "Output mode", 1},
    {"c", 666, nullptr, 0, "Generate files for C"},
    {"cpp", 777, nullptr, 0, "Generate files for C++"},
    {nullptr, 0, nullptr, 0, "Output settings", 2},
    {"display", 'd', "name", 0, "Set display function to be called"},
    {"namespace", 'n', "name", 0, "Namespace, C++ only"},
    {"user", 'u', nullptr, 0, "Include user stamen headers"},
    {nullptr, 0, nullptr, 0, "Informational Options", -1},
    {nullptr},
};
// clang-format on

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
  const std::string base =
      pos != std::string::npos ? config.substr(0, pos) : config;

  if (args.cpp)
  {
    const auto include_filename = base + ".hpp";
    std::ofstream include(include_filename);
    generate_include_cpp(include, args);

    const auto source_filename = base + ".cpp";
    std::ofstream source(source_filename);
    generate_source_cpp(source, args, include_filename);
  }
  else
  {
    const auto include_filename = base + ".h";
    std::ofstream include(include_filename);
    generate_include_c(include, args);

    const auto source_filename = base + ".c";
    std::ofstream source(source_filename);
    generate_source_c(source, args);
  }

  return 0;
}
