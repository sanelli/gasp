#include <iostream>
#include <vector>

#include <gasp/torricelly/interpreter.hpp>

using namespace std;
using namespace gasp;
using namespace gasp::torricelly;

namespace gasp::torricelly::native::io
{

extern "C" void print_y(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = context->pop();
   std::cout << (int32_t)value.get_byte(); // Need to convert into integer or it is outputted as character
}

extern "C" void print_s(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = context->pop();
   std::cout << value.get_short();
}

extern "C" void print_i(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = context->pop();
   std::cout << value.get_integer();
}

extern "C" void print_l(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = context->pop();
   std::cout << value.get_long();
}

extern "C" void print_f(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = context->pop();
   std::cout << value.get_float();
}

extern "C" void print_b(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = context->pop();
   std::cout << value.get_boolean();
}

extern "C" void print_c(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = context->pop();
   std::cout << value.get_char();
}

extern "C" void print_d(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto value = context->pop();
   std::cout << value.get_double();
}

} // namespace gasp::torricelly::native::system
