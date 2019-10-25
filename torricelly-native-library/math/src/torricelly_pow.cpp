#include <cmath>
#include <vector>

#include <gasp/torricelly/interpreter.hpp>

using namespace std;
using namespace gasp;
using namespace gasp::torricelly;

namespace gasp::torricelly::native::math
{

extern "C" void pow_i(std::vector<interpreter::torricelly_activation_record_local> &stack)
{
   auto exponent_value = stack.back();
   auto base_value = stack.back();
   stack.pop_back();
   auto numeric_result = (int)std::pow(base_value.get_integer(), exponent_value.get_integer());
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   stack.push_back(result);
}

extern "C" void pow_f(std::vector<interpreter::torricelly_activation_record_local> &stack)
{
   auto exponent_value = stack.back();
   auto base_value = stack.back();
   stack.pop_back();
   auto numeric_result = std::powf(base_value.get_float(), exponent_value.get_float());
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   stack.push_back(result);
}

extern "C" void pow_d(std::vector<interpreter::torricelly_activation_record_local> &stack)
{
   auto exponent_value = stack.back();
   auto base_value = stack.back();
   stack.pop_back();
   auto numeric_result = std::pow(base_value.get_double(), exponent_value.get_double());
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   stack.push_back(result);
}

} // namespace gasp::torricelly::native::math