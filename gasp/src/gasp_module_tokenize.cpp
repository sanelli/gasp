#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>

#include <sanelli/sanelli.hpp>

#include <gasp/common/gasp_error.hpp>
#include <gasp/module/gasp_module.hpp>
#include <gasp/module/gasp_module_tokenize.hpp>
#include <gasp/blaise/tokenizer/tokenizer.hpp>

using namespace gasp;
using namespace gasp::common;
using namespace gasp::module;
using namespace gasp::blaise;

gasp_module_tokenize::gasp_module_tokenize() {}
gasp_module_tokenize::~gasp_module_tokenize() {}

std::string gasp_module_tokenize::name() const { return "tokenize"; }
std::string gasp_module_tokenize::description() const { return "Output the tokenized version of the input file."; }

bool gasp_module_tokenize::run(int argc, char *argv[])
{
   blaise_tokenizer tokenizer;
   std::vector<sanelli::token<gasp::blaise::blaise_token_type>> tokens;

   set_input_format("blaise");
   set_output_format("text");
   parse_command_line(argc, argv);

   if (is_help())
   {
      std::cerr << "Command line: " << argv[0] << " " << name() << " [options]" << std::endl;
      std::cerr << "Options:" << std::endl;
      std::cerr << help() << std::endl;
      return true;
   }

   if (input_format() != "blaise")
   {
      std::cerr << "Inut format '" << input_format() << "' is not supported." << std::endl;
      return false;
   }

   if (output_format() != "text")
   {
      std::cerr << "Output format '" << output_format() << "' is not supported." << std::endl;
      return false;
   }

   try
   {
      tokenizer.tokenize(input(), tokens);
      for (const auto &token : tokens)
      {
         output() << token << std::endl;
      }
      return true;
   }
   catch (sanelli::tokenizer_error &error)
   {
      std::cerr << "TOKENIZER_ERROR(" << error.line() << "," << error.column() << "): " << error.what() << std::endl;
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
