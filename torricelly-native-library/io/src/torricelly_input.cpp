#include <iostream>
#include <vector>

#include <gasp/torricelly/interpreter.hpp>

using namespace std;
using namespace gasp;
using namespace gasp::torricelly;

namespace gasp::torricelly::native::io
{

template <typename TValue>
inline void read_template(std::vector<interpreter::torricelly_activation_record_local> &stack)
{
   TValue value;
   std::cin >> value;
   auto result = interpreter::torricelly_activation_record_local::make(value);
   stack.push_back(result);
}

extern "C" void read_integer(std::vector<interpreter::torricelly_activation_record_local> &stack)
{
   read_template<int>(stack);
}

extern "C" void read_double(std::vector<interpreter::torricelly_activation_record_local> &stack)
{
   read_template<double>(stack);
}

extern "C" void read_float(std::vector<interpreter::torricelly_activation_record_local> &stack)
{
   read_template<float>(stack);
}

extern "C" void read_char(std::vector<interpreter::torricelly_activation_record_local> &stack)
{
   read_template<char>(stack);
}

} // namespace gasp::torricelly::native::io
