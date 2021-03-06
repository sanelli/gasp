#include <string>
#include <stdexcept>
#include <vector>
#include <filesystem>

#include <sanelli/sanelli.hpp>

#include <gasp/common/gasp_error.hpp>
#include <gasp/common/internal_error.hpp>
#include <gasp/blaise/tokenizer/tokenizer.hpp>
#include <gasp/blaise/parser/parser.hpp>
#include <gasp/blaise/ast.hpp>
#include <gasp/blaise/blaise_module_loader.hpp>
#include <gasp/torricelly/torricelly.hpp>
#include <gasp/blaise-to-torricelly/blaise-to-torricelly.hpp>
#include <gasp/torricelly/interpreter.hpp>
#include <gasp/torricelly/torricelly_io.hpp>

#include <gasp/module/gasp_module_execute.hpp>

using namespace gasp;
using namespace gasp::module;
using namespace gasp::common;
using namespace gasp::blaise;
using namespace gasp::blaise::ast;
using namespace gasp::torricelly;
using namespace gasp::blaise_to_torricelly;
using namespace gasp::torricelly::interpreter;

gasp_module_execute::gasp_module_execute() {}
gasp_module_execute::~gasp_module_execute() {}

std::string gasp_module_execute::name() const { return "execute"; }
std::string gasp_module_execute::description() const { return "Execute the input source file using an interpreter."; }

bool gasp_module_execute::parse_command_line_hook(int &arg_index, int argc, char *argv[], std::map<std::string, bool> &flags)
{
   _parameters.push_back(argv[arg_index]);
   return true;
}

bool gasp_module_execute::run(int argc, char *argv[])
{
   set_input_format("torricelly-binary");
   parse_command_line(argc, argv);

   if (is_help())
   {
      std::cerr << "Command line: " << argv[0] << " " << name() << " [options]" << std::endl;
      std::cerr << "Options:" << std::endl;
      std::cerr << help() << std::endl;
      return true;
   }

   if (input_format() != "blaise" && input_format() != "torricelly-binary")
   {
      std::cerr << "Input format '" << input_format() << "' is not supported (Supported format are 'blaise' and 'torricelly-binary')." << std::endl;
      return false;
   }

   try
   {
      std::shared_ptr<torricelly_module> main_module;

      if (input_format() == "blaise")
      {
         blaise_tokenizer tokenizer;
         blaise_parser parser;
         std::vector<std::string> module_loader_path{"library"};
         auto loader = sanelli::memory::make_shared<blaise_module_loader>(std::filesystem::current_path().string(), module_loader_path);
         blaise_parser_context context([loader](std::string dependency) { return loader->get_module(dependency); });
         std::vector<std::shared_ptr<torricelly_module>> modules;
         tokenizer.tokenize(input(), context);
         parser.parse(context);
         translator translator(context.module());
         translator.execute(modules);
         main_module = modules.at(0);
      }
      else if (input_format() == "torricelly-binary")
      {
         torricelly_binary_input binary_reader(input());
         binary_reader >> main_module;
      }
      else
      {
         std::cerr << "Unknown unput format" << std::endl;
         return false;
      }

      auto interpreter = make_torricelly_interpreter(
          main_module,
          [this](unsigned int index) { return index < _parameters.size() ? _parameters.at(index) : ""; });
      interpreter->initialize();
      interpreter->run();

      if (interpreter->status() == torricelly_interpreter_status::FINISHED)
      {
         if (!torricelly_type_utility::is_void(interpreter->main_module()->get_main()->return_type()))
         {
            auto return_value = interpreter->return_value();
            std::cout << gasp::torricelly::interpreter::to_string(return_value) << std::endl;
         }
      }
      else
      {
         std::cerr << "Execution failed." << std::endl;
      }

      return interpreter->status() == torricelly_interpreter_status::FINISHED;
   }
   catch (const sanelli::tokenizer_error &error)
   {
      std::cerr << "TOKENIZER_ERROR(" << error.line() << "," << error.column() << "): " << error.what() << std::endl;
      return false;
   }
   catch (const sanelli::parser_error &error)
   {
      std::cerr << "PARSER_ERROR(" << error.line() << "," << error.column() << "): " << error.what() << std::endl;
      return false;
   }
   catch (const gasp::blaise::ast::blaise_ast_error &error)
   {
      std::cerr << "BLAISE_AST_ERROR(" << error.line() << "," << error.column() << "): " << error.what() << std::endl;
      return false;
   }
   catch (const gasp::common::gasp_internal_error &error)
   {
      std::cerr << "INTERNAL_ERROR: " << error.what() << std::endl;
      return false;
   }
   catch (const gasp::blaise_to_torricelly::blaise_to_torricelly_internal_error &error)
   {
      std::cerr << "BLAISE_TO_TORRICELLI_INTERNAL_ERROR: " << error.what() << std::endl;
      return false;
   }
   catch (const gasp::blaise_to_torricelly::blaise_to_torricelly_error &error)
   {
      std::cerr << "BLAISE_TO_TORRICELLI_ERROR(" << error.line() << "," << error.column() << "): " << error.what() << std::endl;
      return false;
   }
   catch (const gasp::torricelly::interpreter::torricelly_interpreter_error &error)
   {
      std::cerr << "TORRICELLY_INTERPRETER_ERROR:" << error.what() << std::endl;
      return false;
   }
   catch (const gasp::torricelly::interpreter::torricelly_interpreter_execution_error &error)
   {
      std::cerr << "TORRICELLY_INTERPRETER_EXECUTION_ERROR(" << error.subroutine() << ":" << error.ip() << ")" << error.what() << std::endl;
      return false;
   }
   catch (const std::exception &error)
   {
      std::cerr << "GENERIC_ERROR: " << error.what() << std::endl;
      return false;
   }
   catch (...)
   {
      std::cerr << "UNKNOWN ERROR -- No further information available." << std::endl;
      return false;
   }
}
