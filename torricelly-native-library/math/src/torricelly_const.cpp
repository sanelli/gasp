#include <cmath>
#include <vector>

#include <gasp/torricelly/interpreter.hpp>

using namespace std;
using namespace gasp;
using namespace gasp::torricelly;

const double PI = 3.141592653589793;

namespace gasp::torricelly::native::math
{

extern "C" void pi_(std::vector<interpreter::torricelly_activation_record_local> &stack)
{
   auto result = interpreter::torricelly_activation_record_local::make(PI);
   stack.push_back(result);
}

extern "C" void e_(std::vector<interpreter::torricelly_activation_record_local> &stack)
{
   auto numeric_result = std::exp(1.0);
   auto result = interpreter::torricelly_activation_record_local::make(numeric_result);
   stack.push_back(result);
}

} // namespace gasp::torricelly::native::math