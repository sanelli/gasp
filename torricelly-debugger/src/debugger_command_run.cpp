#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

#include <gasp/torricelly/interpreter.hpp>
#include <gasp/torricelly/debugger.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::interpreter;
using namespace gasp::torricelly::debugger;

torricelly_debugger_command_run::torricelly_debugger_command_run(std::shared_ptr<torricelly_debugger> debugger)
    : torricelly_debugger_command(debugger) {}
torricelly_debugger_command_run::~torricelly_debugger_command_run() {}

std::string torricelly_debugger_command_run::command() const { return "run"; }

void torricelly_debugger_command_run::run(std::ostream &out)
{
   auto interpreter = debugger()->interpreter();
   while (true)
   {
      if (interpreter->status() != torricelly_interpreter_status::RUNNING && interpreter->status() != torricelly_interpreter_status::INITIALIZED)
         break;

      interpreter->step();

      // If not running something went wrong or reached the end program execution
      if (interpreter->status() == torricelly_interpreter_status::RUNNING)
      {
         auto activation_record = interpreter->activation_record();
         auto subroutine = activation_record->subroutine();

         if (debugger()->is_breakpoint(subroutine, activation_record->ip()))
            break;
      }
      else
         break;
   }
}

std::string torricelly_debugger_command_run::description() const
{
   return "Execute the program until a breakpoint or the end of execution is reached.";
}

bool torricelly_debugger_command_run::execute(std::ostream &out, const std::vector<std::string> &parameters)
{

   auto success = true;

   switch (parameters.size())
   {
   case 0:
      run(out);
      break;
   case 1:
   {
      auto param = parameters.at(0);
      if (param == "help")
      {
         out << description() << std::endl;
         out << "Usage: " << command() << std::endl;
         out << "Options:" << std::endl;
         out << "   help: display this message" << std::endl;
      }
      else
      {
         out << "Invalid parameters for command " << command() << std::endl;
      }
   }
   break;
   default:
      out << "Invalid parameters for command " << command() << std::endl;
      success = false;
   }

   return success;
}
