#include <random>
#include <vector>
#include <limits>
#include <memory>

#include <gasp/torricelly/interpreter.hpp>

using namespace std;
using namespace gasp;
using namespace gasp::torricelly;

namespace gasp::torricelly::native::math
{

std::default_random_engine __random_generator;
std::uniform_int_distribution<int> __random_int_distribution(0, std::numeric_limits<int>::max());
std::uniform_int_distribution<int> __random_bool_distribution(0, 1);
std::uniform_int_distribution<char> __random_char_distribution((char)0, (char)255);
std::uniform_real_distribution<double> __uniform_double_distribution(0, 1.00);
std::uniform_real_distribution<float> __uniform_float_distribution(0, 1.00);

extern "C" void random_integer_(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = __random_int_distribution(__random_generator);
   auto result = interpreter::torricelly_activation_record_local::make(value);
   context->push(result);
}

extern "C" void random_char_(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = __random_char_distribution(__random_generator);
   auto result = interpreter::torricelly_activation_record_local::make(value);
   context->push(result);
}

extern "C" void random_boolean_(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = (bool)__random_bool_distribution(__random_generator);
   auto result = interpreter::torricelly_activation_record_local::make(value);
   context->push(result);
}

extern "C" void random_float_(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = __uniform_float_distribution(__random_generator);
   auto result = interpreter::torricelly_activation_record_local::make(value);
   context->push(result);
}

extern "C" void random_double_(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = __uniform_double_distribution(__random_generator);
   auto result = interpreter::torricelly_activation_record_local::make(value);
   context->push(result);
}

} // namespace gasp::torricelly::native::math