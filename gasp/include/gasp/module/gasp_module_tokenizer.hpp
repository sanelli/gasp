#pragma once

#include <string>
#include <fstream>

#include <gasp/module/gasp_module.hpp>

namespace gasp::module
{

class gasp_module_tokenizer : public gasp_module
{
   bool _has_help_flag;
   std::ifstream _input;
   std::ofstream _output;
   bool _input_from_file;
   bool _output_from_file;
   std::string _format;

   void parse_command_line(int argc, char* argv[]);

public:
   gasp_module_tokenizer();
   std::string name() const override;
   std::string description() const override;
   bool run(int argc, char* argv[]) override;
   virtual ~gasp_module_tokenizer();
};

} // namespace gasp::module