#pragma once

#include <string>
#include <vector>

#include <gasp/module/gasp_module_input.hpp>

namespace gasp::module
{

class gasp_module_execute : public gasp_module_input
{
   std::vector<std::string> _parameters;
public:
   bool parse_command_line_hook(int& arg_index, int argc, char* argv[], std::map<std::string, bool>& flags) override;

   gasp_module_execute();
   std::string name() const override;
   std::string description() const override;
   bool run(int argc, char *argv[]) override;
   virtual ~gasp_module_execute();
};

} // namespace gasp::module