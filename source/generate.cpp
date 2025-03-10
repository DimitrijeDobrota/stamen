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

auto accumulate_items(const stamen::Menu& lmenu)
{
  using namespace cemplate;  // NOLINT

  Initlist items;
  for (const auto& [code, prompt, _] : lmenu.items())
  {
    items.emplace_back(Initlist({
        String(prompt),
        code,
    }));
  }

  return InitlistElem(items);
}

void generate_include(std::ostream& ost,
                      const stamen::Stamen& inst,
                      const arguments_t& args)
{
  using namespace std::string_literals;  // NOLINT
  using namespace cemplate;  // NOLINT

  ost << Pragma("once") << '\n';

  ost << Include("cstddef");
  ost << Include("functional");
  ost << Include("string");
  ost << Include("vector");
  ost << '\n';

  ost << Namespace(args.nspace);

  ost << R"(
struct menu_t
{
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
  for (const auto& [code, _] : inst.menu_lookup())
  {
    ost << FunctionD(code, "int", {{{"std::size_t"s, "/* unused */"s}}});
  }

  ost << "\n// free function\n";
  for (const auto& [code, _] : inst.free_lookup())
  {
    ost << FunctionD(code, "int", {{{"std::size_t"s, "/* unused */"s}}});
  }

  ost << Namespace(args.nspace);
}

void generate_source(std::ostream& ost,
                     const stamen::Stamen& inst,
                     const arguments_t& args,
                     const std::string& include_name)
{
  using namespace std::string_literals;  // NOLINT
  using namespace cemplate;  // NOLINT

  ost << Include("cstddef") << '\n';
  ost << IncludeL(include_name) << '\n';

  ost << Namespace(args.nspace);

  // clang-format off
  for (const auto& [code, menu] : inst.menu_lookup())
  {
    ost << Function(
            menu.code(),
            "extern int",
            {{{"std::size_t"s, "/* unused */"s}}}
        )
        << Declaration("static const menu_t", "menu",
         Initlist({
               String(menu.title()),
               menu.code(),
               accumulate_items(menu),
           }))
        << Return("menu_t::visit(menu)")
        << Function(menu.code());
  }
  // clang-format on

  ost << Namespace(args.nspace);
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
    {    nullptr,   0, nullptr, 0, "Output settings", 2},
    {"namespace", 'n',  "name", 0, "Name of the nemespace to generate in"},
    {    nullptr,   0, nullptr, 0, "Informational Options", -1},
    {    nullptr},
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

  std::ifstream ifs(args.config);
  const stamen::Stamen inst(ifs);

  const auto include_filename = args.config.stem().replace_extension(".hpp");
  std::ofstream include(include_filename);
  generate_include(include, inst, args);

  const auto source_filename = args.config.stem().replace_extension(".cpp");
  std::ofstream source(source_filename);
  generate_source(source, inst, args, include_filename);

  return 0;
}
