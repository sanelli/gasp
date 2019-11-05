#include <iostream>
#include <vector>

#include <gasp/torricelly/interpreter.hpp>

using namespace std;
using namespace gasp;
using namespace gasp::torricelly;

namespace gasp::torricelly::native::io
{

template <typename TValue>
inline void read_template(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   TValue value;
   std::cin >> value;
   auto result = interpreter::torricelly_activation_record_local::make(value);
   context->push(result);
}

extern "C" void read_integer_(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   read_template<int>(context);
}

extern "C" void read_double_(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   read_template<double>(context);
}

extern "C" void read_float_(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   read_template<float>(context);
}

extern "C" void read_char_(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   char value;
   std::cin.get(value);
   auto result = interpreter::torricelly_activation_record_local::make(value);
   context->push(result);
}

} // namespace gasp::torricelly::native::io
