#include <cmath>
#include <vector>

#include <gasp/torricelly/interpreter.hpp>

using namespace std;
using namespace gasp;
using namespace gasp::torricelly;

namespace gasp::torricelly::native::math
{

extern "C" void sqrt_i(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = context->pop();
   auto numeric_result = (int)std::sqrt((double)value.get_integer());
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   context->push(result);
}

extern "C" void sqrt_d(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = context->pop();
   auto numeric_result = std::sqrt(value.get_double());
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   context->push(result);
}

extern "C" void sqrt_f(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = context->pop();
   auto numeric_result = std::sqrtf(value.get_float());
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   context->push(result);
}

} // namespace gasp::torricelly::native::math