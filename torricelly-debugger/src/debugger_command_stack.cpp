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

torricelly_debugger_command_stack::torricelly_debugger_command_stack(std::shared_ptr<torricelly_debugger> debugger)
    : torricelly_debugger_command(debugger) {}
torricelly_debugger_command_stack::~torricelly_debugger_command_stack() {}

std::string torricelly_debugger_command_stack::command() const { return "stack"; }

void torricelly_debugger_command_stack::show(std::ostream &out)
{
   auto interpreter = debugger()->interpreter();
   auto activation_record = interpreter->activation_record();
   bool first = true;
   for (auto it = activation_record->begin_stack(); it != activation_record->end_stack(); ++it, first = false)
      out << (first ? "> " : "  ") << to_string(*it) << to_string(it->type()) << std::endl;
}

std::string torricelly_debugger_command_stack::description() const
{
   return "Display the content of the data stack for the activation record on top of the activation records stack";
}

bool torricelly_debugger_command_stack::execute(std::ostream &out, const std::vector<std::string> &parameters)
{
   auto success = true;

   switch (parameters.size())
   {
   case 0:
      show(out);
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
         out << "   show: display context of the stack (defult option)" << std::endl;
         out << "   peek: display the top of the stack for the current procedure" << std::endl;
         out << "   peek-type: display the top of the stack for the current procedure" << std::endl;
      }
      else if (param == "show")
      {
         show(out);
      }
      else if (param == "peek")
      {
         auto interpreter = debugger()->interpreter();
         auto activation_record = interpreter->activation_record();
         out << to_string(activation_record->peek()) << std::endl;
      }
      else if (param == "peek-type")
      {
         auto interpreter = debugger()->interpreter();
         auto activation_record = interpreter->activation_record();
         out << to_string(activation_record->peek().type()) << std::endl;
      }
   }
   break;
   default:
      out << "Invalid parameters for command " << command() << std::endl;
      success = false;
   }

   return success;
}