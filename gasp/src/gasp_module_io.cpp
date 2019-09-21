#include <string>
#include <fstream>

#include <sanelli/sanelli.hpp>

#include <gasp/common/gasp_error.hpp>
#include <gasp/module/gasp_module_io.hpp>

using namespace gasp;
using namespace gasp::common;
using namespace gasp::module;

const std::string p_help = "--help";
const std::string p_help_short = "-h";
const std::string p_input_file = "--input";
const std::string p_input_file_short = "-i";
const std::string p_output_file = "--output";
const std::string p_output_file_short = "-o";
const std::string p_input_format = "--format";
const std::string p_input_format_short = "-f";

gasp_module_io::gasp_module_io()
    : _has_help_flag(false), _input_from_file(false), _output_from_file(false), _format("blaise") {}

gasp_module_io::~gasp_module_io()
{
   if (input_from_file())
      _input.close();
   if (output_from_file())
      _output.close();
}

bool gasp_module_io::is_help() const { return _has_help_flag; }
std::string gasp_module_io::help() const
{
   auto help_message = "   " + p_help + "/" + p_help_short + " : Display the help message.\n" +
                       "   " + p_input_file + "/" + p_input_file_short + " : Input file (optiona; default: stdin).\n" +
                       "   " + p_output_file + "/" + p_output_file_short + " : Output file (optional; default: stdout).\n" +
                       "   " + p_input_format + "/" + p_input_format_short + " : Input file format (optional; default: blaise).";
   auto extended_help_message = extended_help();
   if (extended_help_message == "")
      return help_message;
   else
      return help_message + "\n" + extended_help_message;
}

std::string gasp_module_io::extended_help() const { return ""; }

bool gasp_module_io::input_from_file() const { return _input_from_file; }
std::ifstream &gasp_module_io::input() { return _input; }
bool gasp_module_io::output_from_file() const { return _output_from_file; }
std::ofstream &gasp_module_io::output() { return _output; }
std::string gasp_module_io::format() const { return _format; }

bool gasp_module_io::parse_command_line_hook(int &arg_index, int argc, char *argv[]) { return false; }

void gasp_module_io::parse_command_line(int argc, char *argv[])
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
         if (!parse_command_line_hook(arg_index, argc, argv))
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