#pragma once

#include <string>
#include <gasp/module/gasp_module_io.hpp>

namespace gasp::module
{

class gasp_module_tokenize : public gasp_module_io
{
public:
   gasp_module_tokenize();
   std::string name() const override;
   std::string description() const override;
   bool run(int argc, char* argv[]) override;
   virtual ~gasp_module_tokenize();
};

} // namespace gasp::module