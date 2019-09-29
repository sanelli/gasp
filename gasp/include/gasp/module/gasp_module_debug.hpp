#pragma once

#include <string>
#include <vector>

#include <gasp/module/gasp_module.hpp>

namespace gasp::module
{

class gasp_module_debug : public gasp_module
{
public:
   gasp_module_debug();
   std::string name() const override;
   std::string description() const override;
   bool run(int argc, char *argv[]) override;
   virtual ~gasp_module_debug();
};

} // namespace gasp::module