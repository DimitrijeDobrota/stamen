#include <format>
#include <fstream>
#include <iostream>
#include <string>

#include <cemplate/cemplate.hpp>
#include <poafloc/poafloc.hpp>

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

auto accumulate_items(const stamen::menu::menu_t& lmenu)
{
  using namespace cemplate;  // NOLINT

  initlist items; for (auto i = 0UL; i < lmenu.get_size(); i++)
  {
    items.emplace_back(initlist({
        lmenu.get_code(i),
        string(lmenu.get_prompt(i)),
    }));
  }

  return initlist_elem(items);
}

void generate_include_c(std::ostream& ost, const arguments_t& args)
{
  using namespace cemplate;  // NOLINT

  ost << pragma_once();

  for (const auto& [code, menu] : stamen::menu::menu_lookup)
  {
    ost << func_decl(menu.get_code(), "int", {{"size_t", "/* unused */"}});
  }

  (void)args;
}

void generate_source_c(std::ostream& ost, const arguments_t& args)
{
  using namespace cemplate;  // NOLINT

  if (args.user)
  {
    ost << include("stamen.h", true);
  }
  else
  {
    ost << include("stamen/stamen.h");
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
  using namespace cemplate;  // NOLINT

  ost << pragma_once();

  ost << include("cstddef");
  ost << include("functional");
  ost << include("string");
  ost << include("vector");
  ost << '\n';

  ost << nspace(args.nspace);

  ost << R"(
struct menu_t {
  using callback_f = std::function<int(std::size_t)>;

  struct item_t {
    callback_f callback;
    std::string prompt;
  };

  std::string title;
  callback_f callback;
  std::vector<item_t> items;

  static int visit(const menu_t& menu);
};

)";

  for (const auto& [code, menu] : stamen::menu::menu_lookup)
  {
    ost << func_decl(
        menu.get_code(), "int", {{"std::size_t", "/* unused */"}});
  }

  ost << nspace(args.nspace);
}

void generate_source_cpp(std::ostream& ost,
                         const arguments_t& args,
                         const std::string& include_name)
{
  using namespace cemplate;  // NOLINT

  ost << include(include_name, true);
  ost << '\n';

  ost << nspace(args.nspace);

  for (const auto& [code, _] : stamen::menu::free_lookup)
  {
    ost << func_decl(code, "extern int", {{"std::size_t", "/* unused */"}});
  }
  ost << '\n';

  // clang-format off
  for (const auto& [code, menu] : stamen::menu::menu_lookup)
  {
    ost << func(
            menu.get_code(),
            "extern int",
            {{"std::size_t", "/* unused */"}}
        )
        << decl("static const menu_t", "menu")
        << initlist({
               string(menu.get_title()),
               menu.get_code(),
               accumulate_items(menu),
           })
        << ret("menu_t::visit(menu)")
        << func(menu.get_code());
  }
  // clang-format on

  ost << nspace(args.nspace);
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
