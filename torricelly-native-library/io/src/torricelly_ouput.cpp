#include <iostream>
#include <vector>

#include <gasp/torricelly/interpreter.hpp>

using namespace std;
using namespace gasp;
using namespace gasp::torricelly;

namespace gasp::torricelly::native::io
{

extern "C" void print_i(std::vector<interpreter::torricelly_activation_record_local> &stack)
{
   auto value = stack.back();
   stack.pop_back();
   std::cout << value.get_integer();
}

extern "C" void print_f(std::vector<interpreter::torricelly_activation_record_local> &stack)
{
   auto value = stack.back();
   stack.pop_back();
   std::cout << value.get_float();
}

extern "C" void print_b(std::vector<interpreter::torricelly_activation_record_local> &stack)
{
   auto value = stack.back();
   stack.pop_back();
   std::cout << value.get_boolean();
}

extern "C" void print_c(std::vector<interpreter::torricelly_activation_record_local> &stack)
{
   auto value = stack.back();
   stack.pop_back();
   std::cout << value.get_char();
}

extern "C" void print_d(std::vector<interpreter::torricelly_activation_record_local> &stack)
{
   auto value = stack.back();
   stack.pop_back();
   std::cout << value.get_double();
}

} // namespace gasp::torricelly::native::system
