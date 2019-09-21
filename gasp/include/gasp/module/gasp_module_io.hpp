#pragma once

#include <string>
#include <fstream>

#include <gasp/module/gasp_module.hpp>

namespace gasp::module
{

class gasp_module_io : public gasp_module
{
   bool _has_help_flag;
   std::ifstream _input;
   std::ofstream _output;
   bool _input_from_file;
   bool _output_from_file;
   std::string _format;

protected:
   void parse_command_line(int argc, char* argv[]);
   virtual bool parse_command_line_hook(int& arg_index, int argc, char* argv[]);

   bool is_help() const;
   std::string help() const;
   virtual std::string extended_help() const;
   bool input_from_file() const;
   std::ifstream& input();
   bool output_from_file() const;
   std::ofstream& output();
   std::string format() const;

public:
   gasp_module_io();
   virtual std::string name() const = 0;
   virtual std::string description() const = 0;
   virtual bool run(int argc, char* argv[]) = 0;
   virtual ~gasp_module_io();
};

} // namespace gasp::module