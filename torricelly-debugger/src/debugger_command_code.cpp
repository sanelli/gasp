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

torricelly_debugger_command_code::torricelly_debugger_command_code(std::shared_ptr<torricelly_debugger> debugger)
    : torricelly_debugger_command(debugger) {}
torricelly_debugger_command_code::~torricelly_debugger_command_code() {}

std::string torricelly_debugger_command_code::command() const { return "code"; };
std::string torricelly_debugger_command_code::description() const
{
   return "Display instruction pointed by the instruction pointer";
}

void torricelly_debugger_command_code::print_instruction(std::ostream &out, std::string prefix, std::shared_ptr<torricelly_subroutine> subroutine, unsigned int ip)
{
   auto interpreter = debugger()->interpreter();
   auto activation_record = interpreter->activation_record();
   auto current_subroutine = activation_record->subroutine();
   auto is_current_subrouttine = current_subroutine->name() == subroutine->name();
   bool is_breakpoint = debugger()->is_breakpoint(subroutine, ip);
   bool is_next_instruction = is_current_subrouttine 
      && activation_record->subroutine()->name() == subroutine->name()
      && (activation_record->ip()+1) == ip;

   out
       << prefix
       << "(" << ip << ") "
       << (is_breakpoint ? "*" : " ")
       << (is_next_instruction ? ">" : " ")
       << " "
       << to_string(subroutine->get_instruction(ip))
       << std::endl;
}

void torricelly_debugger_command_code::print_subroutine(std::ostream &out, std::string prefix, std::shared_ptr<torricelly_subroutine> subroutine)
{
   out << prefix << subroutine->name() << ":" << std::endl;
   for (auto ip = 1U; ip <= subroutine->get_number_of_instructions(); ++ip)
      print_instruction(out, prefix + " ", subroutine, ip);
}

void torricelly_debugger_command_code::print_module(std::ostream &out, std::string prefix, std::shared_ptr<torricelly_module> module)
{
   out << module->module_name() << std::endl;
   for (auto subroutine_index = 1U; subroutine_index <= module->get_number_of_subroutines(); ++subroutine_index)
   {
      auto subroutine = module->get_subroutine(subroutine_index);
      print_subroutine(out, prefix + " ", subroutine);
      out << std::endl;
   }
}

bool torricelly_debugger_command_code::execute(std::ostream &out, const std::vector<std::string> &parameters)
{
   auto interpreter = debugger()->interpreter();
   auto activation_record = interpreter->activation_record();

   auto success = true;

   switch (parameters.size())
   {
   case 0:
      print_instruction(out, " ", activation_record->subroutine(), activation_record->ip()+1);
      break;
   case 1:
   {
      auto param = parameters.at(0);
      if (param == "help")
      {
         out << description() << std::endl;
         out << "Usage: " << command() << " [option]" << std::endl;
         out << "Options:" << std::endl;
         out << "   help: display this message" << std::endl;
         out << "   show: display the instruction pointed by the instruction pointer (defult option)." << std::endl;
         out << "   sub: display the instructions of the current subroutine being executed." << std::endl;
         out << "   module: display the instructions of the subroutines of the module being executed." << std::endl;
      }
      else if (param == "show")
      {
         print_instruction(out, " ", activation_record->subroutine(), activation_record->ip()+1);
      }
      else if (param == "sub")
      {
         print_subroutine(out, " ", activation_record->subroutine());
      }
      else if (param == "module")
      {
         print_module(out, " ", activation_record->module());
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