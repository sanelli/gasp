
#include <memory>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <stdexcept>

#include <sanelli/sanelli.hpp>

#include <gasp/common/internal_error.hpp>
#include <gasp/blaise/ast.hpp>
#include <gasp/blaise/tokenizer/tokenizer.hpp>
#include <gasp/blaise/parser/parser.hpp>
#include <gasp/blaise/blaise_module_loader.hpp>

#include <iostream>

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
      locations.push_back(base_directory / p / (module + ".mblaise"));
      locations.push_back(base_directory / p / (module + ".blaise"));
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

   try
   {
      tokenizer.tokenize(input_stream, context);
      parser.parse(context);
   }
   catch (const sanelli::tokenizer_error &error)
   {
      auto message = sanelli::make_string(" @", module," (",module_path,"): ", error.what());
      throw sanelli::tokenizer_error(error.line(), error.column(), message);
   }
   catch (const sanelli::parser_error &error)
   {
      auto message = sanelli::make_string(" @", module," (",module_path,"): ", error.what());
      throw sanelli::parser_error(error.line(), error.column(), message);
   }
   catch (const gasp::blaise::ast::blaise_ast_error &error)
   {
      auto message = sanelli::make_string(" @", module," (",module_path,"): ", error.what());
      throw gasp::blaise::ast::blaise_ast_error(error.line(), error.column(), message);
   }
   catch (const gasp::common::gasp_internal_error &error)
   {
      auto message = sanelli::make_string(" @", module," (",module_path,"): ", error.what());
      throw gasp::common::gasp_internal_error(message);
   }
   return context.module();
}
