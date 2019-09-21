#pragma once

#include <string>
#include <fstream>
#include <map>

#include <gasp/module/gasp_module.hpp>

namespace gasp::module
{

class gasp_module_input : public gasp_module
{
   bool _has_help_flag;
   std::ifstream _input;
   bool _input_from_file;
   std::string _input_format;

protected:
   void parse_command_line(int argc, char* argv[]);
   virtual bool parse_command_line_hook(int& arg_index, int argc, char* argv[], std::map<std::string, bool>& flags);
   virtual void post_command_line_parse(const std::map<std::string, bool>& flags);

   bool is_help() const;
   std::string help() const;
   virtual std::string extended_help() const;

   bool input_from_file() const;
   std::ifstream& input();
   void set_input_format(std::string format);
   std::string input_format() const;

public:
   gasp_module_input();
   virtual std::string name() const = 0;
   virtual std::string description() const = 0;
   virtual bool run(int argc, char* argv[]) = 0;
   virtual ~gasp_module_input();
};

} // namespace gasp::module