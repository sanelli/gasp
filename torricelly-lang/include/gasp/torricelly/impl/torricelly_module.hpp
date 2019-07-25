#pragma once

#include <string>
#include <memory>
#include <vector>

#include <gasp/common/memory.hpp>
#include <gasp/torricelly/impl/torricelly_subroutine.hpp>

namespace gasp::torricelly
{

class torricelly_module
{
   std::string _module_name;
   std::vector<std::shared_ptr<torricelly_subroutine>> _subroutines;
   torricelly_module(const std::string &module_name);

public:
   std::string module_name() const;
   int add_subroutine(std::shared_ptr<torricelly_subroutine> subroutine);

   friend gasp::common::memory;
};

std::shared_ptr<torricelly_module> make_torricelly_module(const std::string &module_name);

} // namespace gasp::torricelly