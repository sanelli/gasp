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

torricelly_debugger_command_bp::torricelly_debugger_command_bp(std::shared_ptr<torricelly_debugger> debugger)
    : torricelly_debugger_command(debugger) {}
torricelly_debugger_command_bp::~torricelly_debugger_command_bp() {}

std::string torricelly_debugger_command_bp::command() const { return "bp"; };

void torricelly_debugger_command_bp::list(std::ostream &out)
{
   auto interpreter = debugger()->interpreter();
   auto activation_record = interpreter->activation_record();
   auto module = activation_record->module();
   for (auto index = 1; index <= module->get_number_of_subroutines(); ++index)
   {
      auto subroutine = module->get_subroutine(index);
      list(out, subroutine);
   }
}
void torricelly_debugger_command_bp::list(std::ostream &out, std::shared_ptr<torricelly::torricelly_subroutine> subroutine)
{
   auto breakpoints = debugger()->breakpoints(subroutine);
   for (auto bp = breakpoints.first; bp != breakpoints.second; ++bp)
      out << subroutine->name() << " :: " << bp->second << std::endl;
}

void torricelly_debugger_command_bp::add(std::ostream &out, std::shared_ptr<torricelly::torricelly_subroutine> subroutine, unsigned int ip)
{
   debugger()->add_breakpoint(subroutine, ip);
}

void torricelly_debugger_command_bp::remove(std::ostream &out, std::shared_ptr<torricelly::torricelly_subroutine> subroutine, unsigned int ip)
{
   debugger()->remove_breakpoint(subroutine, ip);
}

std::string torricelly_debugger_command_bp::description() const
{
   return "List, add and remove breakpoints";
}

bool torricelly_debugger_command_bp::execute(std::ostream &out, const std::vector<std::string> &parameters)
{
   auto success = true;

   switch (parameters.size())
   {
   case 0:
   {
      auto interpreter = debugger()->interpreter();
      auto activation_record = interpreter->activation_record();
      list(out, activation_record->subroutine());
   }
   break;
   case 1:
   {
      auto param = parameters.at(0);
      sanelli::trim(param);
      if (param == "help")
      {
         out << description() << std::endl;
         out << "Usage:" << command() << " [option]" << std::endl;
         out << "Options:" << std::endl;
         out << "   help: display this message" << std::endl;
         out << "   list: display the list of breakpoints for the currenct subroutine (defult option)" << std::endl;
         out << "   all: display all the breakpoints" << std::endl;
         out << "   add <ip>: add a breakpoint to the current subroutine at the given instruction number" << std::endl;
         out << "   add <subroutine> <ip>: add a breakpoint to the given subroutine at the given instruction number" << std::endl;
         out << "   del <ip>: add a breakpoint to the current subroutine at the given instruction number" << std::endl;
         out << "   del <subroutine> <ip>: add a breakpoint to the given subroutine at the given instruction number" << std::endl;
      }
      else if (param == "list")
      {
         auto interpreter = debugger()->interpreter();
         auto activation_record = interpreter->activation_record();
         list(out, activation_record->subroutine());
      }
      else if (param == "all")
      {
         list(out);
      }
      else
      {
         out << "Invalid option" << std::endl;
         success = false;
      }
   }
   break;
   case 2:
   {
      auto p1 = parameters.at(0);
      auto p2 = parameters.at(1);
      unsigned int ip = 0;
      try
      {
         ip = std::stoi(p2);
      }
      catch (const std::exception &e)
      {
         out << "Instruction pointer is not a number" << std::endl;
         return false;
      }

      auto interpreter = debugger()->interpreter();
      auto activation_record = interpreter->activation_record();
      auto subroutine = activation_record->subroutine();

      if (ip <= 0 || ip > subroutine->get_number_of_instructions())
      {
         out << "Instruction pointer is not a valid number" << std::endl;
         return false;
      }

      if (p1 == "add")
      {
         add(out, subroutine, ip);
      }
      else if (p1 == "del")
      {
         remove(out, subroutine, ip);
      }
      else
      {
         out << "Invalid option" << std::endl;
         success = false;
      }
   }
   break;
   case 3:
   {
      auto p1 = parameters.at(0);
      auto p2 = parameters.at(1);
      auto p3 = parameters.at(2);
      unsigned int ip = 0;
      try
      {
         ip = std::stoi(p3);
      }
      catch (const std::exception &e)
      {
         out << "Instruction pointer is not a number" << std::endl;
         return false;
      }

      auto interpreter = debugger()->interpreter();
      auto activation_record = interpreter->activation_record();
      auto module = activation_record->module();
      auto subroutine = module->get_subroutine(p2);

      if (subroutine == nullptr)
      {
         out << "Subroutine " << p2 << " does not exixts" << std::endl;
         return false;
      }

      if (ip <= 0 || ip > subroutine->get_number_of_instructions())
      {
         out << "Instruction pointer is not a valid number" << std::endl;
         return false;
      }

      if (p1 == "add")
      {
         add(out, subroutine, ip);
      }
      else if (p1 == "del")
      {
         remove(out, subroutine, ip);
      }
      else
      {
         out << "Invalid option" << std::endl;
      }
   }
   break;
   default:
      out << "Invalid parameters for command " << command() << std::endl;
      success = false;
   }

   return success;
}