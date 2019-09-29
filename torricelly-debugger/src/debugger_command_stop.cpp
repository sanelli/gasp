
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

torricelly_debugger_command_stop::torricelly_debugger_command_stop(std::shared_ptr<torricelly_debugger> debugger)
    : torricelly_debugger_command(debugger) {}
torricelly_debugger_command_stop::~torricelly_debugger_command_stop() {}

std::string torricelly_debugger_command_stop::command() const { return "stop"; }

std::string torricelly_debugger_command_blaise_load::description() const
{
   return "Stop the debugger and exit.";
}

bool torricelly_debugger_command_stop::execute(std::ostream &out, const std::vector<std::string> &parameters)
{

   switch (parameters.size())
   {
   case 0:
      debugger()->stop();
      break;
   default:
      out << "Too many parameters." << std::endl;
      return false;
   }

   return true;
}