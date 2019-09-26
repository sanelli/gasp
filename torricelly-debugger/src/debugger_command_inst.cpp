#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/interpreter.hpp>
#include <gasp/torricelly/debugger.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::interpreter;
using namespace gasp::torricelly::debugger;

torricelly_debugger_command_inst::torricelly_debugger_command_inst(std::shared_ptr<torricelly_debugger> debugger)
    : torricelly_debugger_command(debugger) {}
torricelly_debugger_command_inst::~torricelly_debugger_command_inst() {}

std::string torricelly_debugger_command_inst::command() const { return "inst"; };

void torricelly_debugger_command_inst::print_instruction(std::ostream &out, std::shared_ptr<torricelly_subroutine> subroutine, unsigned int ip)
{
   auto interpreter = debugger()->interpreter();
   auto activation_record = interpreter->activation_record();
   bool is_breakpoint = debugger()->is_breakpoint(subroutine, ip);
   bool is_current_instruction = activation_record->subroutine()->name() == subroutine->name() && activation_record->ip() == ip;

   out
       << (is_breakpoint ? "*" : " ")
       << (is_current_instruction ? ">" : " ")
       << " "
       << to_string(subroutine->get_instruction(ip))
       << std::endl;
}

bool torricelly_debugger_command_inst::execute(std::ostream &out, const std::vector<std::string> &parameters)
{
   auto interpreter = debugger()->interpreter();
   auto activation_record = interpreter->activation_record();

   auto success = true;

   switch (parameters.size())
   {
   case 0:
      print_instruction(out, activation_record->subroutine(), activation_record->ip());
      break;
   case 1:
   {
      auto param = parameters.at(0);
      sanelli::rtrim(param);
      sanelli::ltrim(param);
      if (param == "help")
      {
         out << "Display instruction pointed by the instruction pointer." << std::endl;
         out << "Usage:" << command() << " [option]" << std::endl;
         out << "Options:" << std::endl;
         out << "   help: display this message" << std::endl;
         out << "   show: display the instruction pointed by the instruction pointer (defult option)" << std::endl;
      }
      else if (param == "show")
      {
         print_instruction(out, activation_record->subroutine(), activation_record->ip());
      }
      else
      {
         out << "Unknwon option" << std::endl;
         success = false;
      }
   }
   break;
   default:
      out << "Invalid parameters for command " << command() << std::endl;
      success = false;
   }

   return success;
}