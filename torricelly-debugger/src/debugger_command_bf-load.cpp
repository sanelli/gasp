
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <iterator>
#include <filesystem>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/interpreter.hpp>
#include <gasp/torricelly/debugger.hpp>
#include <gasp/bf/bf-to-torricelly.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::interpreter;
using namespace gasp::torricelly::debugger;
using namespace gasp::bf;

torricelly_debugger_command_bf_load::torricelly_debugger_command_bf_load(std::shared_ptr<torricelly_debugger> debugger)
    : torricelly_debugger_command(debugger) {}
torricelly_debugger_command_bf_load::~torricelly_debugger_command_bf_load() {}

bool torricelly_debugger_command_bf_load::load(std::ostream &out, const std::vector<std::string> parameters)
{
   try
   {
      std::string filename = parameters.at(0);
      std::ifstream stream;
      stream.open(filename);

      if (!stream)
      {
         out << "Cannot open file '" << filename << "'." << std::endl;
         return false;
      }

      bf_to_torricelly_translator translator;
      auto module = translator.translate("brainfuck", stream);
      debugger()->add_module(module);

      return true;
   }
   catch (const std::exception &e)
   {
      out << "An error occurred while loading '" << parameters.at(0) << "':" << e.what() << std::endl;
      return false;
   }
}

std::string torricelly_debugger_command_bf_load::command() const { return "bf-load"; }
std::string torricelly_debugger_command_bf_load::description() const
{
   return "Load a brainf*ck program and convert it into torricelly code to be debugged";
}
bool torricelly_debugger_command_bf_load::execute(std::ostream &out, const std::vector<std::string> &parameters)
{
   auto success = true;
   switch (parameters.size())
   {
   case 0:
      out << "Not enough parameters" << std::endl;
      success = false;
      break;
   case 1:
   {
      auto p1 = parameters.at(0);
      sanelli::trim(p1);
      if (p1 == "help")
      {
         out << description() << std::endl;
         out << "Usage: " << command() << " [option]" << std::endl;
         out << "Options:" << std::endl;
         out << "   help: display this message" << std::endl;
         out << "   <filename>: Load filename (defult option)" << std::endl;
      }
      else
      {
         success = load(out, parameters);
      }
   }
   break;
   default:
      out << "Too many parameetrs. A brainfuck program does not accept input parameters." << std::endl;
      success = false;
      break;
   }

   return success;
}
