#pragma once

#include <string>

#include <gasp/module/gasp_module_io.hpp>

namespace gasp::module
{

class gasp_module_compile : public gasp_module_io
{
   std::string _out_format;
   std::string out_format() const;
protected:
   std::string extended_help() const override;
   bool parse_command_line_hook(int& arg_index, int argc, char* argv[]) override;
public:
   gasp_module_compile();
   std::string name() const override;
   std::string description() const override;
   bool run(int argc, char* argv[]) override;
   virtual ~gasp_module_compile();
};

} // namespace gasp::module