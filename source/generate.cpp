#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include <cemplate/cemplate.hpp>
#include <poafloc/poafloc.hpp>

#include "stamen/stamen.hpp"

struct arguments_t
{
  std::filesystem::path config;
  std::string nspace = "stamen";
};

namespace {

auto accumulate_items(const stamen::menu_t& lmenu)
{
  using namespace cemplate;  // NOLINT

  initlist items;
  for (const auto& [code, prompt, _] : lmenu.items())
  {
    items.emplace_back(initlist({
        string(prompt),
        code,
    }));
  }

  return initlist_elem(items);
}

void generate_include(std::ostream& ost, const arguments_t& args)
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
class menu_t
{
public:
  using callback_f = std::function<int(std::size_t)>;

  static int visit(const menu_t& menu);

  struct item_t
  {
    std::string prompt;
    callback_f callback;
  };

  std::string title;
  callback_f callback;
  std::vector<item_t> items;
};

)";

  ost << "// generated function\n";
  for (const auto& [code, _] : stamen::menu_lookup)
  {
    ost << func_decl(code, "int", {{"std::size_t", "/* unused */"}});
  }

  ost << "\n// free function\n";
  for (const auto& [code, _] : stamen::free_lookup)
  {
    ost << func_decl(code, "int", {{"std::size_t", "/* unused */"}});
  }

  ost << nspace(args.nspace);
}

void generate_source(std::ostream& ost,
                     const arguments_t& args,
                     const std::string& include_name)
{
  using namespace cemplate;  // NOLINT

  ost << include("cstddef");
  ost << '\n';

  ost << include(include_name, true);
  ost << '\n';

  ost << nspace(args.nspace);

  // clang-format off
  for (const auto& [code, menu] : stamen::menu_lookup)
  {
    ost << func(
            menu.code(),
            "extern int",
            {{"std::size_t", "/* unused */"}}
        )
        << decl("static const menu_t", "menu")
        << initlist({
               string(menu.title()),
               menu.code(),
               accumulate_items(menu),
           })
        << ret("menu_t::visit(menu)")
        << func(menu.code());
  }
  // clang-format on

  ost << nspace(args.nspace);
}

int parse_opt(int key, const char* arg, poafloc::Parser* parser)
{
  auto* arguments = static_cast<arguments_t*>(parser->input());
  switch (key)
  {
    case 'n':
      arguments->nspace = arg;
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
    default:
      break;
  }
  return 0;
}

}  // namespace

// clang-format off
static const poafloc::option_t options[] {
    {nullptr, 0, nullptr, 0, "Output settings", 2},
    {"namespace", 'n', "name", 0, "Namespace, C++ only"},
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
  stamen::read(config.c_str());

  const auto include_filename = args.config.stem().replace_extension(".hpp");
  std::ofstream include(include_filename);
  generate_include(include, args);

  const auto source_filename = args.config.stem().replace_extension(".cpp");
  std::ofstream source(source_filename);
  generate_source(source, args, include_filename);

  return 0;
}
