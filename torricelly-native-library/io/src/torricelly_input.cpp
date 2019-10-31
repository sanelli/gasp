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

extern "C" void read_integer(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   read_template<int>(context);
}

extern "C" void read_double(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   read_template<double>(context);
}

extern "C" void read_float(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   read_template<float>(context);
}

extern "C" void read_char(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   read_template<char>(context);
}

} // namespace gasp::torricelly::native::io
