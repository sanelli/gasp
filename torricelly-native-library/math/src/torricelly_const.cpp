#include <cmath>
#include <vector>
#include <memory>

#include <gasp/torricelly/interpreter.hpp>

using namespace std;
using namespace gasp;
using namespace gasp::torricelly;

const double PI = 3.141592653589793;

namespace gasp::torricelly::native::math
{

extern "C" void pi_(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto result = interpreter::torricelly_activation_record_local::make(PI);
   context->push(result);
}

extern "C" void e_(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto numeric_result = std::exp(1.0);
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   context->push(result);
}

} // namespace gasp::torricelly::native::math