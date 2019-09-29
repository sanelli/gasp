#include <string>
#include <stdexcept>
#include <vector>

#include <sanelli/sanelli.hpp>

#include <gasp/common/gasp_error.hpp>
#include <gasp/torricelly/debugger.hpp>

#include <gasp/module/gasp_module_debug.hpp>

using namespace gasp;
using namespace gasp::common;
using namespace gasp::module;
using namespace gasp::torricelly;
using namespace gasp::torricelly::debugger;

const std::string p_help = "--help";
const std::string p_help_short = "-h";

gasp_module_debug::gasp_module_debug() {}
gasp_module_debug::~gasp_module_debug() {}

std::string gasp_module_debug::name() const { return "debug"; }
std::string gasp_module_debug::description() const { return "Debug the torricelly code."; }

bool gasp_module_debug::run(int argc, char *argv[])
{

   std::map<std::string, bool> flags;
   bool has_help_flag = false;

   for (auto arg_index = 2; arg_index < argc; ++arg_index)
   {
      auto arg = argv[arg_index];
      if (p_help == arg || p_help_short == arg)
      {
         if(has_help_flag)
           throw gasp_error("Duplicated --help/-h provided.");
         has_help_flag = true;
         break;
      }
   }
   try
   {
      auto debugger = debugger::make_torricelly_debugger();
      return debugger->run(std::cin, std::cout);
   }
   catch (const std::exception &error)
   {
      std::cerr << "Error: " << error.what() << std::endl;
      return false;
   }
   catch (...)
   {
      std::cerr << "UNKNOWN ERROR -- No further information available." << std::endl;
      return false;
   }
}
