#include <cmath>
#include <vector>

#include <gasp/torricelly/interpreter.hpp>

using namespace std;
using namespace gasp;
using namespace gasp::torricelly;

namespace gasp::torricelly::native::math
{

extern "C" void log_y(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = context->pop();
   auto numeric_result = (int8_t)std::log((double)value.get_byte());
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   context->push(result);
}

extern "C" void log_s(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = context->pop();
   auto numeric_result = (int16_t)std::log((double)value.get_short());
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   context->push(result);
}

extern "C" void log_i(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = context->pop();
   auto numeric_result = (int32_t)std::log((double)value.get_integer());
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   context->push(result);
}

extern "C" void log_l(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = context->pop();
   auto numeric_result = (int64_t)std::log((double)value.get_long());
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   context->push(result);
}

extern "C" void log_d(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = context->pop();
   auto numeric_result = std::log(value.get_double());
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   context->push(result);
}

extern "C" void log_f(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = context->pop();
   auto numeric_result = std::logf(value.get_float());
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   context->push(result);
}

extern "C" void log10_y(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = context->pop();
   auto numeric_result = (int8_t)std::log10((double)value.get_byte());
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   context->push(result);
}

extern "C" void log10_s(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = context->pop();
   auto numeric_result = (int16_t)std::log10((double)value.get_short());
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   context->push(result);
}

extern "C" void log10_i(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = context->pop();
   auto numeric_result = (int32_t)std::log10((double)value.get_integer());
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   context->push(result);
}

extern "C" void log10_l(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = context->pop();
   auto numeric_result = (int64_t)std::log10((double)value.get_long());
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   context->push(result);
}

extern "C" void log10_d(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = context->pop();
   auto numeric_result = std::log10(value.get_double());
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   context->push(result);
}

extern "C" void log10_f(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = context->pop();
   auto numeric_result = std::log10f(value.get_float());
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   context->push(result);
}

extern "C" void log2_y(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = context->pop();
   auto numeric_result = (int8_t)std::log2((double)value.get_byte());
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   context->push(result);
}

extern "C" void log2_s(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = context->pop();
   auto numeric_result = (int16_t)std::log2((double)value.get_short());
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   context->push(result);
}

extern "C" void log2_i(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = context->pop();
   auto numeric_result = (int32_t)std::log2((double)value.get_integer());
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   context->push(result);
}

extern "C" void log2_l(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = context->pop();
   auto numeric_result = (int64_t)std::log2((double)value.get_long());
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   context->push(result);
}

extern "C" void log2_d(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = context->pop();
   auto numeric_result = std::log2(value.get_double());
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   context->push(result);
}

extern "C" void log2_f(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = context->pop();
   auto numeric_result = std::log2f(value.get_float());
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   context->push(result);
}

} // namespace gasp::torricelly::native::math