#include <ostream>
#include <memory>
#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/torricelly_io.hpp>

using namespace gasp;
using namespace gasp::torricelly;

torricelly_text_output &torricelly::operator<<(torricelly_text_output &os, const std::shared_ptr<const torricelly_subroutine> subroutine)
{
   os << ".subroutine " << subroutine->name() << std::endl;
   os << "   .flags" << subroutine->get_flags() << std::endl;
   os << "   .returns " << subroutine->return_type() << std::endl;
   os << "   .stacksize " << subroutine->max_stack_size() << std::endl;
   os << "   .locals " << subroutine->count_locals() << std::endl;
   for (auto local_index = 1u; local_index <= subroutine->count_locals(); ++local_index)
   {
      os << "      .local "
         << (subroutine->is_parameter(local_index) ? ".parameter " : "")
         << torricelly_inst_ref_type::SUBROUTINE << local_index << " "
         << subroutine->get_local_type(local_index) << " "
         << subroutine->get_local_initial_value(local_index)
         << std::endl;
   }

   for (auto instruction_index = 1u; instruction_index <= subroutine->get_number_of_instructions(); ++instruction_index)
      os << subroutine->get_instruction(instruction_index) << std::endl;

   os << ".end subroutine" << std::endl;

   return os;
}

torricelly_text_output &torricelly::operator<<(torricelly_text_output &os, torricelly_subroutine_flag flags)
{
   if ((flags | torricelly_subroutine_flag::NOTHING) == torricelly_subroutine_flag::NOTHING)
      return os;

   std::vector<torricelly_subroutine_flag> vectorized;
   if ((flags & torricelly_subroutine_flag::MAIN) == torricelly_subroutine_flag::MAIN)
      vectorized.push_back(torricelly_subroutine_flag::MAIN);
   if ((flags & torricelly_subroutine_flag::STATIC) == torricelly_subroutine_flag::STATIC)
      vectorized.push_back(torricelly_subroutine_flag::STATIC);
   if ((flags & torricelly_subroutine_flag::VIRTUAL) == torricelly_subroutine_flag::VIRTUAL)
      vectorized.push_back(torricelly_subroutine_flag::VIRTUAL);
   if ((flags & torricelly_subroutine_flag::OVERRIDE) == torricelly_subroutine_flag::OVERRIDE)
      vectorized.push_back(torricelly_subroutine_flag::OVERRIDE);
   if ((flags & torricelly_subroutine_flag::FINAL) == torricelly_subroutine_flag::FINAL)
      vectorized.push_back(torricelly_subroutine_flag::FINAL);
   if ((flags & torricelly_subroutine_flag::PUBLIC) == torricelly_subroutine_flag::PUBLIC)
      vectorized.push_back(torricelly_subroutine_flag::PUBLIC);
   if ((flags & torricelly_subroutine_flag::PRIVATE) == torricelly_subroutine_flag::PRIVATE)
      vectorized.push_back(torricelly_subroutine_flag::PRIVATE);
   if ((flags & torricelly_subroutine_flag::PROTECTED) == torricelly_subroutine_flag::PROTECTED)
      vectorized.push_back(torricelly_subroutine_flag::PROTECTED);
   if ((flags & torricelly_subroutine_flag::NATIVE) == torricelly_subroutine_flag::NATIVE)
      vectorized.push_back(torricelly_subroutine_flag::NATIVE);

   for (int index = 0; index < vectorized.size(); ++index)
      os << " [" << to_string(vectorized.at(index)) << "]";

   return os;
}

torricelly_binary_output &torricelly::operator<<(torricelly_binary_output &os, const std::shared_ptr<const torricelly_subroutine> subroutine)
{
   os << subroutine->name()
      << subroutine->get_flags()
      << subroutine->return_type()
      << (int32_t)subroutine->max_stack_size()
      << (int32_t)subroutine->count_locals();
   for (auto local_index = 1u; local_index <= subroutine->count_locals(); ++local_index)
   {
      os << subroutine->is_parameter(local_index)
         << subroutine->get_local_type(local_index)
         << subroutine->get_local_initial_value(local_index);
   }
   os << (int32_t)subroutine->get_number_of_instructions();
   for (auto instruction_index = 1u; instruction_index <= subroutine->get_number_of_instructions(); ++instruction_index)
      os << subroutine->get_instruction(instruction_index);
   return os;
}

char torricelly::to_binary(torricelly_subroutine_flag flag)
{
   switch (flag)
   {
   case torricelly_subroutine_flag::MAIN:
      return 'm';
   case torricelly_subroutine_flag::STATIC:
      return 's';
   case torricelly_subroutine_flag::VIRTUAL:
      return 'v';
   case torricelly_subroutine_flag::OVERRIDE:
      return 'o';
   case torricelly_subroutine_flag::FINAL:
      return 'f';
   case torricelly_subroutine_flag::PUBLIC:
      return 'p';
   case torricelly_subroutine_flag::PRIVATE:
      return 'r';
   case torricelly_subroutine_flag::PROTECTED:
      return 't';
   case torricelly_subroutine_flag::NATIVE:
      return 'n';
   default:
      throw torricelly_error("Cannot convert subroutine flag into binary code");
   }
}

torricelly_binary_output &torricelly::operator<<(torricelly_binary_output &os, torricelly_subroutine_flag flags)
{
   std::vector<char> vectorized;
   if ((flags & torricelly_subroutine_flag::MAIN) == torricelly_subroutine_flag::MAIN)
      vectorized.push_back(to_binary(torricelly_subroutine_flag::MAIN));
   if ((flags & torricelly_subroutine_flag::STATIC) == torricelly_subroutine_flag::STATIC)
      vectorized.push_back(to_binary(torricelly_subroutine_flag::STATIC));
   if ((flags & torricelly_subroutine_flag::VIRTUAL) == torricelly_subroutine_flag::VIRTUAL)
      vectorized.push_back(to_binary(torricelly_subroutine_flag::VIRTUAL));
   if ((flags & torricelly_subroutine_flag::OVERRIDE) == torricelly_subroutine_flag::OVERRIDE)
      vectorized.push_back(to_binary(torricelly_subroutine_flag::OVERRIDE));
   if ((flags & torricelly_subroutine_flag::FINAL) == torricelly_subroutine_flag::FINAL)
      vectorized.push_back(to_binary(torricelly_subroutine_flag::FINAL));
   if ((flags & torricelly_subroutine_flag::PUBLIC) == torricelly_subroutine_flag::PUBLIC)
      vectorized.push_back(to_binary(torricelly_subroutine_flag::PUBLIC));
   if ((flags & torricelly_subroutine_flag::PRIVATE) == torricelly_subroutine_flag::PRIVATE)
      vectorized.push_back(to_binary(torricelly_subroutine_flag::PRIVATE));
   if ((flags & torricelly_subroutine_flag::PROTECTED) == torricelly_subroutine_flag::PROTECTED)
      vectorized.push_back(to_binary(torricelly_subroutine_flag::PROTECTED));
   if ((flags & torricelly_subroutine_flag::NATIVE) == torricelly_subroutine_flag::NATIVE)
      vectorized.push_back(to_binary(torricelly_subroutine_flag::NATIVE));

   os << (int32_t)vectorized.size();
   for (int index = 0; index < vectorized.size(); ++index)
      os << vectorized.at(index);

   return os;
}
