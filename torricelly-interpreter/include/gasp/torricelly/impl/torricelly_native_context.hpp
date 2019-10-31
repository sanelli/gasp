#pragma once

#include <vector>
#include <memory>

#include <gasp/torricelly/impl/torricelly_activation_record_local.hpp>
#include <sanelli/sanelli.hpp>

namespace gasp::torricelly::interpreter
{

class torricelly_native_context
{
   std::vector<torricelly_activation_record_local> _stack;
   torricelly_native_context();

public:
   torricelly_activation_record_local pop();
   void push(torricelly_activation_record_local value);

   friend sanelli::memory;
};

std::shared_ptr<torricelly_native_context> make_torricelly_native_context();

} // namespace gasp::torricelly