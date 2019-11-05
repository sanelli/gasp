
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <iterator>
#include <filesystem>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/torricelly_io.hpp>
#include <gasp/torricelly/interpreter.hpp>
#include <gasp/torricelly/debugger.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::interpreter;
using namespace gasp::torricelly::debugger;

torricelly_debugger_command_load::torricelly_debugger_command_load(std::shared_ptr<torricelly_debugger> debugger)
    : torricelly_debugger_command(debugger) {}
torricelly_debugger_command_load::~torricelly_debugger_command_load() {}

bool torricelly_debugger_command_load::load(std::ostream &out, const std::vector<std::string> parameters)
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

      std::shared_ptr<torricelly::torricelly_module> module{nullptr};
      torricelly_binary_input input(stream);
      input >> module;

      debugger()->add_module(module);

      auto it = parameters.begin();
      std::advance(it, 1);
      auto end = parameters.end();
      debugger()->load(it, end);

      return true;
   }
   catch (const std::exception &e)
   {
      out << "An error occurred while loading '" << parameters.at(0) << "':" << e.what() << std::endl;
      return false;
   }
}

std::string torricelly_debugger_command_load::command() const { return "load"; }
std::string torricelly_debugger_command_load::description() const
{
   return "Load a binary torricelly module to be debugged.";
}
bool torricelly_debugger_command_load::execute(std::ostream &out, const std::vector<std::string> &parameters)
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
         out << "   <filename> [parameters]: Load filename with the given parameters (default option)" << std::endl;
      }
      else
      {
         success = load(out, parameters);
      }
   }
   break;
   default:
      success = load(out, parameters);
      break;
   }

   return success;
}
