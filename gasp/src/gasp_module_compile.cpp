#include <string>
#include <stdexcept>

#include <sanelli/sanelli.hpp>

#include <gasp/common/gasp_error.hpp>
#include <gasp/common/internal_error.hpp>
#include <gasp/blaise/tokenizer/tokenizer.hpp>
#include <gasp/blaise/parser/parser.hpp>
#include <gasp/blaise/ast.hpp>
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
   set_output_format("torricelly-text");
   parse_command_line(argc, argv);

   blaise_tokenizer tokenizer;
   blaise_parser parser;
   blaise_parser_context context;
   std::vector<std::shared_ptr<torricelly_module>> modules;
   torricelly_text_output torricelly_output(output());

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

   if (output_format() != "torricelly-text")
   {
      std::cerr << "Output format '" << output_format() << "' is not supported." << std::endl;
      return false;
   }

   try
   {
      tokenizer.tokenize(input(), context);
      parser.parse(context);
      translator translator(context.module());
      translator.execute(modules);
      for (auto module : modules)
         torricelly_output << module;
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
