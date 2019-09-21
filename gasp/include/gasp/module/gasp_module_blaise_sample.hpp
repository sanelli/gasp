#pragma once

#include <string>
#include <gasp/module/gasp_module.hpp>

namespace gasp::module
{

class gasp_module_blaise_sample : public gasp_module
{
public:
   gasp_module_blaise_sample();
   std::string name() const override;
   std::string description() const override;
   bool run(int argc, char* argv[]) override;
   virtual ~gasp_module_blaise_sample();
};

} // namespace gasp::module