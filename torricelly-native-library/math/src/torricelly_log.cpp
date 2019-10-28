#include <cmath>
#include <vector>

#include <gasp/torricelly/interpreter.hpp>

using namespace std;
using namespace gasp;
using namespace gasp::torricelly;

namespace gasp::torricelly::native::math
{

extern "C" void log_i(std::vector<interpreter::torricelly_activation_record_local> &stack)
{
   auto value = stack.back();
   stack.pop_back();
   auto numeric_result = (int)std::log((double)value.get_integer());
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   stack.push_back(result);
}

extern "C" void log_d(std::vector<interpreter::torricelly_activation_record_local> &stack)
{
   auto value = stack.back();
   stack.pop_back();
   auto numeric_result = std::log(value.get_double());
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   stack.push_back(result);
}

extern "C" void log_f(std::vector<interpreter::torricelly_activation_record_local> &stack)
{
   auto value = stack.back();
   stack.pop_back();
   auto numeric_result = std::logf(value.get_float());
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   stack.push_back(result);
}

extern "C" void log10_i(std::vector<interpreter::torricelly_activation_record_local> &stack)
{
   auto value = stack.back();
   stack.pop_back();
   auto numeric_result = (int)std::log10((double)value.get_integer());
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   stack.push_back(result);
}

extern "C" void log10_d(std::vector<interpreter::torricelly_activation_record_local> &stack)
{
   auto value = stack.back();
   stack.pop_back();
   auto numeric_result = std::log10(value.get_double());
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   stack.push_back(result);
}

extern "C" void log10_f(std::vector<interpreter::torricelly_activation_record_local> &stack)
{
   auto value = stack.back();
   stack.pop_back();
   auto numeric_result = std::log10f(value.get_float());
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   stack.push_back(result);
}

extern "C" void log2_i(std::vector<interpreter::torricelly_activation_record_local> &stack)
{
   auto value = stack.back();
   stack.pop_back();
   auto numeric_result = (int)std::log2((double)value.get_integer());
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   stack.push_back(result);
}

extern "C" void log2_d(std::vector<interpreter::torricelly_activation_record_local> &stack)
{
   auto value = stack.back();
   stack.pop_back();
   auto numeric_result = std::log2(value.get_double());
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   stack.push_back(result);
}

extern "C" void log2_f(std::vector<interpreter::torricelly_activation_record_local> &stack)
{
   auto value = stack.back();
   stack.pop_back();
   auto numeric_result = std::log2f(value.get_float());
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   stack.push_back(result);
}

} // namespace gasp::torricelly::native::math