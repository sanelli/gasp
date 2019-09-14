#pragma once

#include <memory>
#include <gasp/torricelly/torricelly.hpp>

namespace gasp::torricelly::interpreter
{

class torricelly_interpreter
{
   public:
   torricelly_interpreter();
   void execute(std::shared_ptr<gasp::torricelly::torricelly_module> main_module);
};

} // namespace gasp::torricelly::interpreter