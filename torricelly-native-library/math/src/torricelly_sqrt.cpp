#include <cmath>
#include <vector>

#include <gasp/torricelly/interpreter.hpp>

using namespace std;
using namespace gasp;
using namespace gasp::torricelly;

namespace gasp::torricelly::native::math
{

extern "C" void sqrt_y(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = context->pop();
   auto numeric_result = (int8_t)std::sqrt((double)value.get_byte());
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   context->push(result);
}

extern "C" void sqrt_s(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = context->pop();
   auto numeric_result = (int16_t)std::sqrt((double)value.get_short());
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   context->push(result);
}

extern "C" void sqrt_i(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = context->pop();
   auto numeric_result = (int32_t)std::sqrt((double)value.get_integer());
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   context->push(result);
}

extern "C" void sqrt_l(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = context->pop();
   auto numeric_result = (int64_t)std::sqrt((double)value.get_long());
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

extern "C" void sqrt_af1xuaf1xui(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto size = context->pop().get_integer();
   auto output_array = context->pop().get_array_pointer();
   auto input_array = context->pop().get_array_pointer();

   for(auto index = 0; index < size; ++index)
   {
      gasp::torricelly::interpreter::torricelly_activation_record_local_union u;
      u._float = (float)std::sqrt(input_array->get(index)._float);
      output_array->set(index, u);
   }
}

extern "C" void sqrt_ad1xuad1xui(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto size = context->pop().get_integer();
   auto output_array = context->pop().get_array_pointer();
   auto input_array = context->pop().get_array_pointer();

   for(auto index = 0; index < size; ++index)
   {
      gasp::torricelly::interpreter::torricelly_activation_record_local_union u;
      u._double = (double)std::sqrt(input_array->get(index)._double);
      output_array->set(index, u);
   }
}

} // namespace gasp::torricelly::native::math