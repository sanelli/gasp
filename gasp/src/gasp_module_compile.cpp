#include <string>
#include <stdexcept>
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
#include <gasp/bf/bf-to-torricelly.hpp>
#include <gasp/torricelly/torricelly_io.hpp>

#include <gasp/module/gasp_module_compile.hpp>

using namespace gasp;
using namespace gasp::module;
using namespace gasp::common;
using namespace gasp::blaise;
using namespace gasp::blaise::ast;
using namespace gasp::torricelly;
using namespace gasp::blaise_to_torricelly;
using namespace gasp::bf;

gasp_module_compile::gasp_module_compile() {}
gasp_module_compile::~gasp_module_compile() {}

std::string gasp_module_compile::name() const { return "compile"; }
std::string gasp_module_compile::description() const { return "Compile the input source file into the desired format."; }

std::string __get_target_path(std::string original_path, std::string module_name, std::string format)
{
   if (original_path == "")
      return "";
   std::filesystem::path path{original_path};
   auto target_path = path.parent_path() / (module_name + (format == "torricelly-binary" ? ".tb" : ".t"));

   if (!std::filesystem::exists(target_path))
      return target_path;

   auto original_last_modified = std::filesystem::last_write_time(original_path);
   auto target_last_modified = std::filesystem::last_write_time(target_path);

   if (target_last_modified < original_last_modified)
      return target_path;

   return "";
}

void __write_to_file(std::ostream &stream, std::string output_format, std::shared_ptr<torricelly_module> module)
{
   if (output_format == "torricelly-text")
   {
      torricelly_text_output torricelly_output(stream);
      torricelly_output << module;
   }
   else if (output_format == "torricelly-binary")
   {
      torricelly_binary_output torricelly_output(stream);
      torricelly_output << module;
   }
}

void __translate_dependencies(std::ostream &original_output_stream,
                              std::string output_format,
                              std::shared_ptr<ast::blaise_ast_module> blaise_module,
                              std::vector<std::shared_ptr<torricelly_module>> &torricelly_modules)
{

   auto initial_size = torricelly_modules.size();
   translator translator(blaise_module);
   translator.execute(torricelly_modules);
   auto final_size = torricelly_modules.size();

   if (initial_size != final_size)
   {
      std::ostream &output_stream = original_output_stream;

      if (blaise_module->get_path() != "")
      {
         auto target_path = __get_target_path(blaise_module->get_path(), blaise_module->name(), output_format);
         if (target_path != "")
         {
            std::ofstream file_output_stream;
            file_output_stream.open(target_path);
            __write_to_file(file_output_stream, output_format, torricelly_modules.back());
            file_output_stream.close();
         }
      }
      else
         __write_to_file(original_output_stream, output_format, torricelly_modules.back());
   }

   for (auto dependecy_index = 0; dependecy_index < blaise_module->count_dependencies(); ++dependecy_index)
      __translate_dependencies(original_output_stream, output_format, blaise_module->get_dependency(dependecy_index), torricelly_modules);
}

bool gasp_module_compile::run(int argc, char *argv[])
{
   set_input_format("blaise");
   set_output_format("torricelly-binary");
   parse_command_line(argc, argv);

   if (is_help())
   {
      std::cerr << "Command line: " << argv[0] << " " << name() << " [options]" << std::endl;
      std::cerr << "Options:" << std::endl;
      std::cerr << help() << std::endl;
      return true;
   }

   if (input_format() != "blaise" && input_format() != "bf")
   {
      std::cerr << "Input format '" << input_format() << "' is not supported (Supported formats are blaise and bf)." << std::endl;
      return false;
   }

   if (output_format() != "torricelly-text" && output_format() != "torricelly-binary")
   {
      std::cerr << "Output format '" << output_format() << "' is not supported (Valid values are 'torricelly-text' and 'torricelly-binary')." << std::endl;
      return false;
   }

   try
   {
      std::shared_ptr<torricelly_module> module;

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
         __translate_dependencies(output(), output_format(), context.module(), modules);
         module = modules.at(0);
      }
      else if (input_format() == "bf")
      {
         bf_to_torricelly_translator translator;
         module = translator.translate("brainfuck", input());
      }

      if (output_format() == "torricelly-text")
      {
         torricelly_text_output torricelly_output(output());
         torricelly_output << module;
      }
      else if (output_format() == "torricelly-binary")
      {
         torricelly_binary_output torricelly_output(output());
         torricelly_output << module;
      }
      return true;
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
   catch (gasp::blaise_to_torricelly::blaise_to_torricelly_error &error)
   {
      std::cerr << "BLAISE_TO_TORRICELLI_ERROR(" << error.line() << "," << error.column() << "): " << error.what() << std::endl;
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
