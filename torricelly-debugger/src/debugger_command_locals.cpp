#include <memory>
#include <string>
#include <vector>
#include <iostream>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/interpreter.hpp>
#include <gasp/torricelly/debugger.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::interpreter;
using namespace gasp::torricelly::debugger;

torricelly_debugger_command_locals::torricelly_debugger_command_locals(std::shared_ptr<torricelly_debugger> debugger)
    : torricelly_debugger_command(debugger) {}
torricelly_debugger_command_locals::~torricelly_debugger_command_locals() {}

std::string torricelly_debugger_command_locals::command() const { return "locals"; }

bool torricelly_debugger_command_locals::show(std::ostream &out, std::shared_ptr<gasp::torricelly::interpreter::torricelly_activation_record> activation_record, unsigned int index)
{
   auto subroutine = activation_record->subroutine();
   if (index < 1 || index > subroutine->count_locals())
   {
      out << "Error. Cannot display local at index " << index << "." << std::endl;
      return false;
   }
   else
   {
      auto value = activation_record->load(index);
      out << "(" << index << ") " << to_string(value) << " [" << to_string(value.type()) << "]" << std::endl;
      return true;
   }
}

void torricelly_debugger_command_locals::show(std::ostream &out, std::shared_ptr<gasp::torricelly::interpreter::torricelly_activation_record> activation_record)
{
   auto subroutine = activation_record->subroutine();
   for (auto index = 1U; index <= subroutine->count_locals(); ++index)
      show(out, activation_record, index);
}

std::string torricelly_debugger_command_locals::description() const
{
   return "Display the content of the locals for the activation record on top of the activation records stack";
}

bool torricelly_debugger_command_locals::execute(std::ostream &out, const std::vector<std::string> &parameters)
{
   auto success = true;

   switch (parameters.size())
   {
   case 0:
   {
      auto interpreter = debugger()->interpreter();
      auto activation_record = interpreter->activation_record();
      show(out, activation_record);
   }
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
         out << "   all: display all the locals (defult option)" << std::endl;
         out << "   count: display the number of locals" << std::endl;
         out << "   <number>: display the local at the specified index" << std::endl;
      }
      else if (param == "all")
      {
         auto interpreter = debugger()->interpreter();
         auto activation_record = interpreter->activation_record();
         show(out, activation_record);
      }
      else if (param == "count")
      {
         auto interpreter = debugger()->interpreter();
         auto activation_record = interpreter->activation_record();
         auto subroutine = activation_record->subroutine();
         out << subroutine->count_locals() << std::endl;
      }
      else
      {
         auto interpreter = debugger()->interpreter();
         auto activation_record = interpreter->activation_record();
         auto index = 0;
         try
         {
            index = stoi(parameters.at(0));
         }
         catch (std::exception &e)
         {
            index = 0;
            out << "Error. Invalid number." << std::endl;
            success = false;
         }
         if (success)
            success = show(out, activation_record, index);
      }
   }
   break;
   default:
   {
      out << "Invalid parameters for command " << command() << std::endl;
      success = false;
   }
   }

   return success;
}