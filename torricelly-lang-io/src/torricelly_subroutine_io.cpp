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

torricelly_subroutine_flag torricelly::torricelly_subroutine_flag_from_binary(char byte)
{
   switch (byte)
   {
   case 'm':
      return torricelly_subroutine_flag::MAIN;
   case 's':
      return torricelly_subroutine_flag::STATIC;
   case 'v':
      return torricelly_subroutine_flag::VIRTUAL;
   case 'o':
      return torricelly_subroutine_flag::OVERRIDE;
   case 'f':
      return torricelly_subroutine_flag::FINAL;
   case 'p':
      return torricelly_subroutine_flag::PUBLIC;
   case 'r':
      return torricelly_subroutine_flag::PRIVATE;
   case 't':
      return torricelly_subroutine_flag::PROTECTED;
   case 'n':
      return torricelly_subroutine_flag::NATIVE;
   default:
      throw torricelly_error(sanelli::make_string("Cannot convert binary '", (int)byte, "' code into subroutine flag"));
   }
}

torricelly_binary_input &torricelly::operator>>(torricelly_binary_input &is, torricelly_subroutine_flag &flags)
{
   int32_t size;
   is >> size;

   flags = torricelly_subroutine_flag::NOTHING;
   for (int flag_index = 0; flag_index < size; ++flag_index)
   {
      char byte;
      is >> byte;
      auto flag = torricelly_subroutine_flag_from_binary(byte);
      flags = flags | flag;
   }

   return is;
}

torricelly_binary_input &torricelly::operator>>(torricelly_binary_input &is, std::shared_ptr<const torricelly_subroutine> &subroutine)
{
   std::string name;
   is >> name;
   torricelly_subroutine_flag flags;
   is >> flags;
   std::shared_ptr<torricelly_type> return_type;
   is >> return_type;
   auto result = torricelly::make_torricelly_subroutine(name, return_type);
   result->set_flags(flags);

   int32_t max_stack_size;
   is >> max_stack_size;
   result->max_stack_size(max_stack_size);

   int32_t number_of_locals;
   is >> number_of_locals;
   for (auto local_index = 0; local_index < number_of_locals; ++local_index)
   {
      bool is_parameter;
      is >> is_parameter;
      std::shared_ptr<torricelly_type> local_type;
      is >> local_type;
      torricelly_value local_value = torricelly_value_from_binary(is, local_type);
      result->add_local(local_type, local_value, is_parameter);
   }

   int32_t number_of_instructions;
   for (auto instruction_index = 0; instruction_index < number_of_instructions; ++instruction_index)
   {
      torricelly_instruction instruction = torricelly_instruction::make(torricelly_inst_code::NOOP);
      is >> instruction;
      result->append_instruction(instruction);
   }

   subroutine = result;

   return is;
}
