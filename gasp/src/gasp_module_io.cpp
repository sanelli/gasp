#include <string>
#include <fstream>

#include <sanelli/sanelli.hpp>

#include <gasp/common/gasp_error.hpp>
#include <gasp/module/gasp_module_io.hpp>

using namespace gasp;
using namespace gasp::common;
using namespace gasp::module;

const std::string p_output_file = "--output";
const std::string p_output_file_short = "-o";
const std::string p_output_format = "--output-format";
const std::string p_output_format_short = "-of";

gasp_module_io::gasp_module_io()
    : _output_from_file(false), _output_format("text") {}

gasp_module_io::~gasp_module_io()
{
   if (output_from_file())
      output().close();
}

std::string gasp_module_io::extended_help() const
{
   return "   " + p_output_file + "/" + p_output_file_short + ": Output file (optional).\n" +
          "   " + p_output_format + "/" + p_output_format_short + ": Output file format (optional).";
}

bool gasp_module_io::output_from_file() const { return _output_from_file; }
std::ofstream &gasp_module_io::output() { return _output; }
std::string gasp_module_io::output_format() const { return _output_format; }
void gasp_module_io::set_output_format(std::string format) { _output_format = format; }

void gasp_module_io::post_command_line_parse(const std::map<std::string, bool> &flags)
{
   if (flags.find(p_output_file) == flags.end())
   {
      _output.copyfmt(std::cout);
      _output.clear(std::cout.rdstate());
      _output.basic_ios<char>::rdbuf(std::cout.rdbuf());
   }
}

bool gasp_module_io::parse_command_line_hook(int &arg_index, int argc, char *argv[], std::map<std::string, bool> &flags)
{
   auto arg = argv[arg_index];

   if (p_output_file == arg || p_output_file_short == arg)
   {
      if ((arg_index + 1) >= argc)
         throw gasp_error("Missing output file name.");
      if (flags.find(p_output_file) != flags.end())
         throw gasp_error("Output file name already provided.");
      arg = argv[++arg_index];
      _output.open(arg);
      if (!_output.good())
         throw gasp_error(sanelli::make_string("Cannot open file '", arg, "'."));
      _output_from_file = true;
      return true;
   }
   else if (p_output_format == arg || p_output_format_short == arg)
   {
      if ((arg_index + 1) >= argc)
         throw gasp_error("Missing format definition.");
      if (flags.find(p_output_format) != flags.end())
         throw gasp_error("Output file format already provided.");
      arg = argv[++arg_index];
      _output_format = arg;
      return true;
   }

   if (flags.find(p_output_file) == flags.end())
   {
      _output.copyfmt(std::cout);
      _output.clear(std::cout.rdstate());
      _output.basic_ios<char>::rdbuf(std::cout.rdbuf());
   }

   return false;
}
