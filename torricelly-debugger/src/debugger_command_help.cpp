
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include <gasp/torricelly/interpreter.hpp>
#include <gasp/torricelly/debugger.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::interpreter;
using namespace gasp::torricelly::debugger;

torricelly_debugger_command_help::torricelly_debugger_command_help(std::shared_ptr<torricelly_debugger> debugger)
   : torricelly_debugger_command(debugger) { }
torricelly_debugger_command_help::~torricelly_debugger_command_help() { }

std::string torricelly_debugger_command_help::command() const { return "help"; }

std::string torricelly_debugger_command_help::description() const
{
   return "Display available commands";
}

bool torricelly_debugger_command_help::execute(std::ostream &out, const std::vector<std::string> &parameters) { 

   switch(parameters.size()) { 
      case 0:
      {
        std::ios_base::fmtflags original_flags(out.flags());
         const std::string spaces(1, ' ');
         out << "Available commands:" << std::endl;
         size_t max_command_name_size = 1;
         for(auto it = debugger()->begin_commands(); it != debugger()->end_commands(); ++it)
            max_command_name_size = std::max(max_command_name_size, it->second->command().size() + spaces.size() + 2);
         for(auto it = debugger()->begin_commands(); it != debugger()->end_commands(); ++it)
            out << std::left << std::setw(max_command_name_size) << (spaces + it->second->command() + ": ")
                << std::setw(0) << it->second->description() << "." << std::endl;
         out << "Type \"<command_name> help\" for more details about the command." << std::endl; 
         out.flags(original_flags);
      }
      break;
      default:
         out << "Too many parameters." << std::endl;
         return false;
   }

   return true;
}