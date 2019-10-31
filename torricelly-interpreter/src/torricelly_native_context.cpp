#include <vector>
#include <memory>

#include <gasp/torricelly/interpreter.hpp>
#include <sanelli/sanelli.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::interpreter;

torricelly_native_context::torricelly_native_context() {}

torricelly_activation_record_local torricelly_native_context::pop()
{
   auto value = _stack.back();
   _stack.pop_back();
   return value;
}
void torricelly_native_context::push(torricelly_activation_record_local value)
{
   _stack.push_back(value);
}

std::shared_ptr<torricelly_native_context> torricelly::interpreter::make_torricelly_native_context()
{
   return sanelli::memory::make_shared<torricelly_native_context>();
}