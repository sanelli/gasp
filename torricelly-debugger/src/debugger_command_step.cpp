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

torricelly_debugger_command_step::torricelly_debugger_command_step(std::shared_ptr<torricelly_debugger> debugger)
    : torricelly_debugger_command(debugger) {}
torricelly_debugger_command_step::~torricelly_debugger_command_step() {}

std::string torricelly_debugger_command_step::command() const { return "step"; }

void torricelly_debugger_command_step::step(std::ostream &out, unsigned int count)
{
   auto interpreter = debugger()->interpreter();
   for (int st = 0; st < count; ++st)
   {  
      if(interpreter->status() != torricelly_interpreter_status::RUNNING)
         break;

      interpreter->step();

      auto activation_record = interpreter->activation_record();
      auto subroutine = activation_record->subroutine();

      if(debugger()->is_breakpoint(subroutine, activation_record->ip()))
         break;
   }
}

std::string torricelly_debugger_command_step::description() const
{
   return "Execute the next instruction(s)";
}

bool torricelly_debugger_command_step::execute(std::ostream &out, const std::vector<std::string> &parameters)
{

   auto success = true;

   switch (parameters.size())
   {
   case 0:
      step(out, 1);
      break;
   case 1:
   {
      auto param = parameters.at(0);
      if (param == "help")
      {
         out << description() << std::endl;
         out << "Usage: " << command() << " [<number>]" << std::endl;
         out << "Options:" << std::endl;
         out << "   help: display this message" << std::endl;
         out << "   <number>: number of steps (defult 1)" << std::endl;
      }
      else
      {
         try
         {
            auto steps = std::stoi(parameters.at(0));
            step(out, steps);
         }
         catch (const std::exception &e)
         {
            out << "Parameter " << parameters.at(0) << "is not a valid number." << std::endl;
            ;
         }
      }
   }
   break;
   default:
      out << "Invalid parameters for command " << command() << std::endl;
      success = false;
   }

   return success;
}