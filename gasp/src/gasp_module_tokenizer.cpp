#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include <sanelli/sanelli.hpp>

#include <gasp/common/gasp_error.hpp>
#include <gasp/module/gasp_module.hpp>
#include <gasp/module/gasp_module_tokenizer.hpp>
#include <gasp/blaise/tokenizer/tokenizer.hpp>

using namespace gasp;
using namespace gasp::common;
using namespace gasp::module;
using namespace gasp::blaise;

const std::string p_help = "--help";
const std::string p_help_short = "-h";
const std::string p_input_file = "--input";
const std::string p_input_file_short = "-i";
const std::string p_output_file = "--output";
const std::string p_output_file_short = "-o";
const std::string p_input_format = "--format";
const std::string p_input_format_short = "-f";

gasp_module_tokenizer::gasp_module_tokenizer()
    : _has_help_flag(false), _input_from_file(false), _output_from_file(false)
{
}

gasp_module_tokenizer::~gasp_module_tokenizer()
{
   if (_input_from_file)
      _input.close();
   if (_output_from_file)
      _output.close();
}

std::string gasp_module_tokenizer::name() const { return "tokenizer"; }
std::string gasp_module_tokenizer::description() const { return "Output the tokenized version of the input file."; }

void gasp_module_tokenizer::parse_command_line(int argc, char *argv[])
{
   bool infile_found = false;
   bool outfile_found = false;
   bool infile_format_found = false;

   for (auto arg_index = 2; arg_index < argc; ++arg_index)
   {
      auto arg = argv[arg_index];
      if (p_help == arg || p_help_short == arg)
      {
         _has_help_flag = true;
         break;
      }
      else if (p_input_file == arg || p_input_file_short == arg)
      {
         if ((arg_index + 1) >= argc)
            throw gasp_error("Missing input file name.");
         if (infile_found)
            throw gasp_error("Input file name already provided.");
         arg = argv[++arg_index];
         _input.open(arg);
         if (!_input.good())
            throw gasp_error(sanelli::make_string("Cannot open file '", arg, "'"));
         _input_from_file = true;
         infile_found = true;
      }
      else if (p_output_file == arg || p_output_file_short == arg)
      {
         if ((arg_index + 1) >= argc)
            throw gasp_error("Missing output file name.");
         if (outfile_found)
            throw gasp_error("Output file name already provided.");
         arg = argv[++arg_index];
         _output.open(arg);
         if (!_input.good())
            throw gasp_error(sanelli::make_string("Cannot open file '", arg, "'."));
         _output_from_file = true;
         outfile_found = true;
      }
      else if (p_output_file == arg || p_output_file_short == arg)
      {
         if ((arg_index + 1) >= argc)
            throw gasp_error("Missing format definition.");
         if (infile_format_found)
            throw gasp_error("Input format already provided.");
         arg = argv[++arg_index];
         _format = arg;
         if (_format != "blaise")
            throw gasp_error(sanelli::make_string("Unknown format '", arg, ".'"));
         infile_format_found = true;
      }
      else
      {
         throw gasp_error(sanelli::make_string("Unknown parameter '", arg, "'."));
      }
   }

   if (!infile_found)
   {
      _input.copyfmt(std::cin);
      _input.clear(std::cin.rdstate());
      _input.basic_ios<char>::rdbuf(std::cin.rdbuf());
   }

   if (!outfile_found)
   {
      _output.copyfmt(std::cout);
      _output.clear(std::cout.rdstate());
      _output.basic_ios<char>::rdbuf(std::cout.rdbuf());
   }
}

bool gasp_module_tokenizer::run(int argc, char *argv[])
{
   blaise_tokenizer tokenizer;
   std::vector<sanelli::token<gasp::blaise::blaise_token_type>> tokens;

   parse_command_line(argc, argv);

   try
   {
      tokenizer.tokenize(_input, tokens);
      for(const auto& token : tokens) { 
         _output << token << std::endl;
      }
      return true;
   }
   catch (sanelli::tokenizer_error &error)
   {
      std::cerr << "TOKENIZER_ERROR(" << error.line() << "," << error.column() << "): " << error.what() << std::endl;
      return false;
   }
   catch (std::exception &error)
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
