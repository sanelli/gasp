#include <string>
#include <iostream>
#include <memory>
#include <stdexcept>

#include <gasp/module/gasp_module_dump.hpp>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/torricelly_io.hpp>

using namespace gasp;
using namespace gasp::module;

gasp_module_dump::gasp_module_dump() {}
gasp_module_dump::~gasp_module_dump() {}
std::string gasp_module_dump::name() const { return "dump"; };
std::string gasp_module_dump::description() const { return "Convert a binary representation into a textual representation˝"; };
bool gasp_module_dump::run(int argc, char *argv[])
{
   set_input_format("torricelly-binary");
   set_output_format("torricelly-text");

   parse_command_line(argc, argv);

   if (is_help())
   {
      std::cerr << "Command line: " << argv[0] << " " << name() << " [options]" << std::endl;
      std::cerr << "Options:" << std::endl;
      std::cerr << help() << std::endl;
      return true;
   }

   if (input_format() != "torricelly-binary")
   {
      std::cerr << "Input format '" << input_format() << "' is not supported. Only 'torricelly-binary'" << std::endl;
      return false;
   }

   if (output_format() != "torricelly-text")
   {
      std::cerr << "Output format '" << output_format() << "' is not supported. Only 'torricelly-text' is supported." << std::endl;
      return false;
   }

   try
   {
      torricelly::torricelly_binary_input binary_input(input());
      torricelly::torricelly_text_output text_output(output());

      std::shared_ptr<torricelly::torricelly_module> torricelly_module;
      binary_input >> torricelly_module;
      text_output << torricelly_module;
   }
   catch (torricelly::torricelly_error &e)
   {
      std::cerr << "TORRICELLY_ERROR: " << e.what() << std::endl;
      return false;
   }
   catch (std::exception &e)
   {
      std::cerr << "TORRICELLY_ERROR: " << e.what() << std::endl;
      return false;
   }
   catch (...)
   {
      std::cerr << "Unexpected error. No other details are available." << std::endl;
      return false;
   }

   return true;
};
