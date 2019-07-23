#pragma once

#include <string>
#include <memory>
#include <gasp/common/memory.hpp>

namespace gasp::torricelly
{

class torricelly_module
{
   std::string _module_name;

   torricelly_module(const std::string &module_name);

   public:
   std::string module_name() const;

   friend gasp::common::memory;
};

std::shared_ptr<torricelly_module> make_torricelly_module(const std::string &module_name);

} // namespace gasp::torricelly