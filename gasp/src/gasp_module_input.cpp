#include <string>
#include <fstream>
#include <map>

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
const std::string p_input_format = "--input-format";
const std::string p_input_format_short = "-if";

gasp_module_input::gasp_module_input()
    : _has_help_flag(false), _input_from_file(false), _input_format("blaise") {}

gasp_module_input::~gasp_module_input()
{
   if (input_from_file())
      input().close();
}

bool gasp_module_input::is_help() const { return _has_help_flag; }
std::string gasp_module_input::help() const
{
   auto help_message = "   " + p_help + "/" + p_help_short + ": Display the help message.\n" +
                       "   " + p_input_file + "/" + p_input_file_short + ": Input file (optional)." +
                        "   " + p_input_format + "/" + p_input_format_short + ": Input file format (optional).";
   auto extended_help_message = extended_help();
   if (extended_help_message == "")
      return help_message;
   else
      return help_message + "\n" + extended_help_message;
}

std::string gasp_module_input::extended_help() const { return ""; }

bool gasp_module_input::input_from_file() const { return _input_from_file; }
std::ifstream &gasp_module_input::input() { return _input; }
std::string gasp_module_input::input_format() const { return _input_format; }
void gasp_module_input::set_input_format(std::string format) { _input_format = format; }

bool gasp_module_input::parse_command_line_hook(int &arg_index, int argc, char *argv[], std::map<std::string, bool> &flags) { return false; }

void gasp_module_input::parse_command_line(int argc, char *argv[])
{
   std::map<std::string, bool> flags;

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
         if (flags.find(p_input_file) != flags.end())
            throw gasp_error("Input file name already provided.");
         arg = argv[++arg_index];
         _input.open(arg);
         if (!_input.good())
            throw gasp_error(sanelli::make_string("Cannot open file '", arg, "'"));
         _input_from_file = true;
         flags[p_input_file] = true;
      }
      else if (p_input_format == arg || p_input_format_short == arg)
      {
         if ((arg_index + 1) >= argc)
            throw gasp_error("Missing format definition.");
         if (flags.find(p_input_format) != flags.end())
            throw gasp_error("Input format already provided.");
         arg = argv[++arg_index];
         _input_format = arg;
         flags[p_input_format] = true;
      }
      else
      {
         if (!parse_command_line_hook(arg_index, argc, argv, flags))
            throw gasp_error(sanelli::make_string("Unknown parameter '", arg, "'."));
      }
   }

   if (flags.find(p_input_file) == flags.end())
   {
      _input.copyfmt(std::cin);
      _input.clear(std::cin.rdstate());
      _input.basic_ios<char>::rdbuf(std::cin.rdbuf());
   }

   post_command_line_parse(flags);
}

void gasp_module_input::post_command_line_parse(const std::map<std::string, bool> &flags)
{
}