
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <iterator>
#include <filesystem>

#include <gasp/blaise/tokenizer/tokenizer.hpp>
#include <gasp/blaise/parser/parser.hpp>
#include <gasp/blaise/ast.hpp>
#include <gasp/blaise/blaise_module_loader.hpp>
#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/interpreter.hpp>
#include <gasp/torricelly/debugger.hpp>
#include <gasp/blaise-to-torricelly/blaise-to-torricelly.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::interpreter;
using namespace gasp::torricelly::debugger;
using namespace gasp::blaise;
using namespace gasp::blaise::ast;
using namespace gasp::blaise_to_torricelly;

torricelly_debugger_command_blaise_load::torricelly_debugger_command_blaise_load(std::shared_ptr<torricelly_debugger> debugger)
    : torricelly_debugger_command(debugger) {}
torricelly_debugger_command_blaise_load::~torricelly_debugger_command_blaise_load() {}

bool torricelly_debugger_command_blaise_load::load(std::ostream &out, const std::vector<std::string> parameters)
{
   try
   {
      blaise_tokenizer tokenizer;
      blaise_parser parser;
      std::vector<std::string> module_loader_path{"library"};
      auto loader = sanelli::memory::make_shared<blaise_module_loader>(std::filesystem::current_path().string(), module_loader_path);
      blaise_parser_context context([loader](std::string dependency) { return loader->get_module(dependency); });
      std::vector<std::shared_ptr<torricelly_module>> modules;

      std::string filename = parameters.at(0);
      std::ifstream stream;
      stream.open(filename);

      if (!stream)
      {
         out << "Cannot open file '" << filename << "'." << std::endl;
         return false;
      }

      tokenizer.tokenize(stream, context);
      parser.parse(context);
      translator translator(context.module());
      translator.execute(modules);

      for (auto module : modules)
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

std::string torricelly_debugger_command_blaise_load::command() const { return "blaise-load"; }
std::string torricelly_debugger_command_blaise_load::description() const
{
   return "Load a blaise program and covnert into torricelly code to be debugged";
}
bool torricelly_debugger_command_blaise_load::execute(std::ostream &out, const std::vector<std::string> &parameters)
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
         out << "   <filename> [parameters]: Load filename with the given parameters (defult option)" << std::endl;
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
