#include <iostream>
#include <gasp/torricelly/interpreter.hpp>

using namespace std;
using namespace gasp;
using namespace gasp::torricelly;

namespace gasp::torricelly::native::io
{

extern "C" void print_ai1uic(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto separator = context->pop().get_char();
   auto size = context->pop().get_integer();
   auto array = context->pop().get_array_pointer();

   for (int index = 0; index < size; ++index)
   {
      auto value = array->get(index);
      if (index > 0)
         std::cout << separator << ' ';
      std::cout << value._integer;
   }
}

extern "C" void print_ai1ubc(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto separator = context->pop().get_char();
   auto size = context->pop().get_integer();
   auto array = context->pop().get_array_pointer();

   for (int index = 0; index < size; ++index)
   {
      auto value = array->get(index);
      if (index > 0)
         std::cout << separator << ' ';
      std::cout << value._boolean;
   }
}

extern "C" void print_ai1ucc(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto separator = context->pop().get_char();
   auto size = context->pop().get_integer();
   auto array = context->pop().get_array_pointer();

   for (int index = 0; index < size; ++index)
   {
      auto value = array->get(index);
      if (index > 0)
         std::cout << separator << ' ';
      std::cout << value._char;
   }
}

extern "C" void print_ai1ufc(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto separator = context->pop().get_char();
   auto size = context->pop().get_integer();
   auto array = context->pop().get_array_pointer();

   for (int index = 0; index < size; ++index)
   {
      auto value = array->get(index);
      if (index > 0)
         std::cout << separator << ' ';
      std::cout << value._float;
   }
}

extern "C" void print_ai1udc(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   auto separator = context->pop().get_char();
   auto size = context->pop().get_integer();
   auto array = context->pop().get_array_pointer();

   for (int index = 0; index < size; ++index)
   {
      auto value = array->get(index);
      if (index > 0)
         std::cout << separator << ' ';
      std::cout << value._double;
   }
}

} // namespace gasp::torricelly::native::io