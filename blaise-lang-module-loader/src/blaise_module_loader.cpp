
#include <memory>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>

#include <sanelli/sanelli.hpp>

#include <gasp/blaise/ast.hpp>
#include <gasp/blaise/tokenizer/tokenizer.hpp>
#include <gasp/blaise/parser/parser.hpp>
#include <gasp/blaise/blaise_module_loader.hpp>

using namespace gasp;
using namespace gasp::blaise;
using namespace gasp::blaise::ast;

blaise_module_loader::blaise_module_loader(std::string current_folder, const std::vector<std::string> path)
    : _current_folder(current_folder), _path(path) {}

std::string blaise_module_loader::get_path_for_module(std::string module) const
{
   std::filesystem::path base_directory{_current_folder};
   std::vector<std::filesystem::path> locations{
       base_directory / (module + ".mblaise"),
       base_directory / (module + ".blaise")};
   for (const auto p : _path)
   {
      locations.push_back(base_directory / p / ".mblaise");
      locations.push_back(base_directory / p / ".mblaise");
   }
   auto found = std::find_if(locations.begin(), locations.end(), [](const auto p) { return std::filesystem::exists(p); });
   if (found == locations.end())
      return "";
   return *found;
}

std::shared_ptr<ast::blaise_ast_module> blaise_module_loader::get_module(std::string module) const
{
   auto module_path = get_path_for_module(module);
   if (module_path == "")
      return nullptr;

   blaise_tokenizer tokenizer;
   blaise_parser parser;
   std::vector<std::string> module_loader_path{"library"};
   blaise_parser_context context([this](std::string dependency) { return this->get_module(dependency); });

   std::ifstream input_stream(module_path);

   tokenizer.tokenize(input_stream, context);
   parser.parse(context);

   return context.module();
}
