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

void generate_include(std::ostream& ost,
                      const stamen::Stamen& inst,
                      const arguments_t& args)
{
  using namespace std::string_literals;  // NOLINT
  using namespace cemplate;  // NOLINT

  Program prog(ost);

  const auto func_f = [&prog](const auto& pair)
  {
    prog.function_decl(
        pair.first, "int", {{{"std::size_t"s, "/* unused */"s}}});
  };

  prog.pragma("once")
      .line_empty()
      .include("cstddef")
      .include("functional")
      .include("string")
      .include("vector")
      .line_empty()
      .namespace_open(args.nspace)
      .value(R"(
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

)");

  prog.comment("generated function");
  std::for_each(
      std::begin(inst.menu_lookup()), std::end(inst.menu_lookup()), func_f);
  prog.line_empty();

  prog.comment("free function");
  std::for_each(
      std::begin(inst.free_lookup()), std::end(inst.free_lookup()), func_f);

  prog.namespace_close(args.nspace);
}

void generate_source(std::ostream& ost,
                     const stamen::Stamen& inst,
                     const arguments_t& args,
                     const std::string& include_name)
{
  using namespace std::string_literals;  // NOLINT
  using namespace cemplate;  // NOLINT

  const auto init_f = [](const auto& menu)
  {
    return InitlistNode({
        string(menu.title()),
        menu.code(),
        InitlistNode(std::begin(menu.items()),
                     std::end(menu.items()),
                     [](const auto& item) -> InitlistNode
                     {
                       return {
                           string(item.prompt),
                           item.code,
                       };
                     }),
    });
  };

  Program prog(ost);

  prog.include("cstddef")
      .line_empty()
      .includeL(include_name)
      .line_empty()
      .namespace_open(args.nspace);

  for (const auto& [_, menu] : inst.menu_lookup())
  {
    prog.function_open(
            menu.code(), "extern int", {{{"std::size_t"s, "/* unused */"s}}})
        .declaration("static const menu_t", "menu", init_f(menu))
        .ret("menu_t::visit(menu)")
        .function_close(menu.code());
  }

  prog.namespace_close(args.nspace);
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
