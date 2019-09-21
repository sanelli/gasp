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

const std::string p_output_format = "--out-format";
const std::string p_output_format_short = "-of";

gasp_module_compile::gasp_module_compile() : _out_format("torricelly-text") {}
gasp_module_compile::~gasp_module_compile() {}

std::string gasp_module_compile::name() const { return "compile"; }
std::string gasp_module_compile::description() const { return "Compile the input source file into the desired format."; }

std::string gasp_module_compile::out_format() const { return _out_format; }
std::string gasp_module_compile::extended_help() const
{
   return "   " + p_output_format + "/" + p_output_format_short + ": Select the output format (optional; default: torricelly-text)";
}

bool gasp_module_compile::parse_command_line_hook(int &arg_index, int argc, char *argv[])
{
   auto arg = argv[arg_index];
   if (p_output_format == arg || p_output_format_short == arg)
   {
      if (arg_index + 1 < argc)
      {
         _out_format = arg;
         ++arg_index;
         return true;
      }
      else
         throw gasp_error(sanelli::make_string("Expected value after parameter ", arg, "."));
   }

   return false;
}

bool gasp_module_compile::run(int argc, char *argv[])
{
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

   if (format() != "blaise")
   {
      std::cerr << "Input format '" << format() << "' is not supported." << std::endl;
      return false;
   }

   if (out_format() != "torricelly-text")
   {
      std::cerr << "Output format '" << out_format() << "' is not supported." << std::endl;
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
   catch (sanelli::tokenizer_error &error)
   {
      std::cerr << "TOKENIZER_ERROR(" << error.line() << "," << error.column() << "): " << error.what() << std::endl;
      return false;
   }
      catch (sanelli::parser_error &error)
   {
      std::cerr << "PARSER_ERROR(" << error.line() << "," << error.column() << "): " << error.what() << std::endl;
      return false;
   }
   catch (gasp::blaise::ast::blaise_ast_error &error)
   {
      std::cerr << "BLAISE_AST_ERROR(" << error.line() << "," << error.column() << "): " << error.what() << std::endl;
      return false;
   }
   catch (gasp::common::gasp_internal_error &error)
   {
      std::cerr << "INTERNAL_ERROR: " << error.what() << std::endl;
      return false;
   }
   catch (gasp::blaise_to_torricelly::blaise_to_torricelly_internal_error &error)
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
