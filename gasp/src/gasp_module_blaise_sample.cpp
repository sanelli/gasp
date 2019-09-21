#include <string>
#include <map>
#include <iostream>

#include <gasp/common/gasp_error.hpp>
#include <gasp/module/gasp_module_blaise_sample.hpp>

using namespace gasp;
using namespace gasp::common;
using namespace gasp::module;

std::map<std::string, std::string> _samples;

auto sample_empty = R"__(program empty;
begin
end.)__";

auto sample_empty_with_parameters = R"__(program empty_with_parameters(first: integer, second: boolean);
begin
end.)__";

gasp_module_blaise_sample::gasp_module_blaise_sample()
{
   _samples["empty"] = sample_empty;
   _samples["empty-with-parameters"] = sample_empty_with_parameters;
}

std::string gasp_module_blaise_sample::name() const
{
   return "blaise-sample";
}

std::string gasp_module_blaise_sample::description() const
{
   return "Output the desired blaise sample.";
}

bool gasp_module_blaise_sample::run(int argc, char *argv[])
{
   if (argc < 3)
   {
      std::cerr << "To few parameters passed. Use the option '--help' for more details." << std::endl;
      return false;
   }

   if (argc > 3)
   {
      std::cerr << "To many parameters passed. Use the option '--help' for more details." << std::endl;
      return false;
   }

   std::string arg(argv[2]);
   if (arg == "--help" || arg == "-h")
   {
      std::cout << "Command line: " << argv[0] << " " << name() << " [options]|<sample_name>" << std::endl;
      std::cout << "Options:" << std::endl;
      std::cout << "   --help/-h: display this help." << std::endl;
      std::cout << "   --list/-l: list all the available samples." << std::endl;
      return true;
   }

   if (arg == "--list" || arg == "-l")
   {
      for (auto it = _samples.begin(); it != _samples.end(); ++it)
         std::cout << it->first << std::endl;
      return true;
   }

   auto it = _samples.find(arg);
   if (it != _samples.end())
   {
      std::cout << it->second << std::endl;
      return true;
   }
   else
   {
      std::cerr << "Unknown sample '" << arg << "'." << std::endl;
      return false;
   }
}

gasp_module_blaise_sample::~gasp_module_blaise_sample()
{
   _samples.clear();
}
