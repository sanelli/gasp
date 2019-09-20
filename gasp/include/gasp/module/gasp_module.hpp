#pragma once

#include <string>

namespace gasp::module
{

class gasp_module
{
public:
   virtual std::string name() const = 0;
   virtual std::string description() const = 0;
   virtual bool run(int argc, char* argv[]) = 0;
   virtual ~gasp_module() = 0;
};

} // namespace gasp::module