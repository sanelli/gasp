#include <string>
#include <stdexcept>
#include <filesystem>

#include <sanelli/sanelli.hpp>

#include <gasp/common/gasp_error.hpp>
#include <gasp/common/internal_error.hpp>
#include <gasp/blaise/tokenizer/tokenizer.hpp>
#include <gasp/blaise/parser/parser.hpp>
#include <gasp/blaise/ast.hpp>
#include <gasp/blaise/blaise_module_loader.hpp>
#include <gasp/torricelly/torricelly.hpp>
#include <gasp/blaise-to-torricelly/blaise-to-torricelly.hpp>
#include <gasp/torricelly/torricelly_io.hpp>

#include <gasp/module/gasp_module_compile.hpp>

using namespace gasp;
using namespace gasp::module;
using namespace gasp::common;
using namespace gasp::blaise;
using namespace gasp::blaise::ast;
using namespace gasp::torricelly;
using namespace gasp::blaise_to_torricelly;

gasp_module_compile::gasp_module_compile() {}
gasp_module_compile::~gasp_module_compile() {}

std::string gasp_module_compile::name() const { return "compile"; }
std::string gasp_module_compile::description() const { return "Compile the input source file into the desired format."; }

bool gasp_module_compile::run(int argc, char *argv[])
{
   set_input_format("blaise");
   set_output_format("torricelly-binary");
   parse_command_line(argc, argv);

   blaise_tokenizer tokenizer;
   blaise_parser parser;
   std::vector<std::string> module_loader_path{"library"};
   auto loader = sanelli::memory::make_shared<blaise_module_loader>(std::filesystem::current_path().string(), module_loader_path);
   blaise_parser_context context([loader](std::string dependency) { return loader->get_module(dependency); });
   std::vector<std::shared_ptr<torricelly_module>> modules;

   if (is_help())
   {
      std::cerr << "Command line: " << argv[0] << " " << name() << " [options]" << std::endl;
      std::cerr << "Options:" << std::endl;
      std::cerr << help() << std::endl;
      return true;
   }

   if (input_format() != "blaise")
   {
      std::cerr << "Input format '" << input_format() << "' is not supported." << std::endl;
      return false;
   }

   if (output_format() != "torricelly-text" && output_format() != "torricelly-binary")
   {
      std::cerr << "Output format '" << output_format() << "' is not supported (Valid values are 'torricelly-text' and 'torricelly-binary')." << std::endl;
      return false;
   }

   try
   {
      tokenizer.tokenize(input(), context);
      parser.parse(context);
      translator translator(context.module());
      translator.execute(modules);
      if (output_format() == "torricelly-text")
      {
         torricelly_text_output torricelly_output(output());
         torricelly_output << modules.at(0);
      }
      else if (output_format() == "torricelly-binary")
      {
         torricelly_binary_output torricelly_output(output());
         torricelly_output << modules.at(0);
      }
      return true;
   }
   catch (const sanelli::tokenizer_error &error)
   {
      std::cerr << "TOKENIZER_ERROR(" << error.line() << "," << error.column() << "): " << error.what() << std::endl;
      return false;
   }
   catch (const sanelli::parser_error &error)
   {
      std::cerr << "PARSER_ERROR(" << error.line() << "," << error.column() << "): " << error.what() << std::endl;
      return false;
   }
   catch (const gasp::blaise::ast::blaise_ast_error &error)
   {
      std::cerr << "BLAISE_AST_ERROR(" << error.line() << "," << error.column() << "): " << error.what() << std::endl;
      return false;
   }
   catch (const gasp::common::gasp_internal_error &error)
   {
      std::cerr << "INTERNAL_ERROR: " << error.what() << std::endl;
      return false;
   }
   catch (const gasp::blaise_to_torricelly::blaise_to_torricelly_internal_error &error)
   {
      std::cerr << "BLAISE_TO_TORRICELLI_INTERNAL_ERROR: " << error.what() << std::endl;
      return false;
   }
   catch (gasp::blaise_to_torricelly::blaise_to_torricelly_error &error)
   {
      std::cerr << "BLAISE_TO_TORRICELLI_ERROR(" << error.line() << "," << error.column() << "): " << error.what() << std::endl;
      return false;
   }
   catch (const std::exception &error)
   {
      std::cerr << "GENERIC_ERROR: " << error.what() << std::endl;
      return false;
   }
   catch (...)
   {
      std::cerr << "UNKNOWN ERROR -- No further information available." << std::endl;
      return false;
   }
}
