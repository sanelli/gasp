#pragma once

#include <string>
#include <fstream>
#include <map>

#include <gasp/module/gasp_module_input.hpp>

namespace gasp::module
{

class gasp_module_io : public gasp_module_input
{
   std::ofstream _output;
   bool _output_from_file;
   std::string _output_format;

protected:
   bool parse_command_line_hook(int &arg_index, int argc, char *argv[], std::map<std::string, bool> &map) override;
   void post_command_line_parse(const std::map<std::string, bool> &flags) override;

   std::string extended_help() const override;

   bool output_from_file() const;
   std::ofstream &output();
   std::string output_format() const;
   void set_output_format(std::string format);

public:
   gasp_module_io();
   virtual ~gasp_module_io();
};

} // namespace gasp::module
