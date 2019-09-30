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

torricelly_debugger_command_ip::torricelly_debugger_command_ip(std::shared_ptr<torricelly_debugger> debugger)
    : torricelly_debugger_command(debugger) {}
torricelly_debugger_command_ip::~torricelly_debugger_command_ip() {}

std::string torricelly_debugger_command_ip::command() const { return "ip"; }

bool torricelly_debugger_command_ip::ip(std::ostream &out, bool print_instruction)
{
   auto interpreter = debugger()->interpreter();
   if (interpreter->status() != torricelly_interpreter_status::RUNNING && interpreter->status() != torricelly_interpreter_status::INITIALIZED)
   {
      out << "Cannot display Instruction Pointer. Program is not running or not program loaded or reached the end of the program." << std::endl;
      return false;
   }

   auto activation_record = interpreter->activation_record();

   auto ip = activation_record->ip() + 1;
   if (ip > 0)
   {

      if (print_instruction)
      {
         auto subroutine = activation_record->subroutine();
         auto instruction = subroutine->get_instruction(ip);
         out << "(" << ip << ") " << to_string(instruction);
      }
      else
      {
         out << ip;
      }
      out << std::endl;
   }
   else
   {
      out << "Instruction Pointer not set" << std::endl;
      return false;
   }

   return true;
}

std::string torricelly_debugger_command_ip::description() const
{
   return "Display the instruction pointer value of the current activation record";
}

bool torricelly_debugger_command_ip::execute(std::ostream &out, const std::vector<std::string> &parameters)
{
   auto success = true;

   switch (parameters.size())
   {
   case 0:
      ip(out, false);
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
         out << "   show: display the instruction pointer (defult option)" << std::endl;
         out << "   instruction: display the instruction pointer and the instruction associated" << std::endl;
      }
      else if (param == "show")
      {
         ip(out, false);
      }
      else if (param == "instruction")
      {
         ip(out, true);
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