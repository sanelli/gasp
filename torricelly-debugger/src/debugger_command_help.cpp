
#include <memory>
#include <string>
#include <vector>
#include <iostream>

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
   return "Display available commands.";
}

bool torricelly_debugger_command_help::execute(std::ostream &out, const std::vector<std::string> &parameters) { 

   switch(parameters.size()) { 
      case 0:
      {
         out << "Available commands:" << std::endl;
         for(auto it = debugger()->begin_commands(); it != debugger()->end_commands(); ++it)
            out << "   " << it->second->command() << ": " << it->second->description() << "." << std::endl;
         out << "Type \"<command_name> help\" for more details about the command." << std::endl; 
      }
      break;
      default:
         out << "Too many parameters." << std::endl;
         return false;
   }

   return true;
}