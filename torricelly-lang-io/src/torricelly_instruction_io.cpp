#include <memory>
#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/torricelly_io.hpp>

using namespace gasp;
using namespace gasp::torricelly;

torricelly_text_output &torricelly::operator<<(torricelly_text_output &os, torricelly_inst_ref_type inst_ref_type)
{
   return os << to_string(inst_ref_type);
}

torricelly_text_output &torricelly::operator<<(torricelly_text_output &os, const torricelly_instruction &instruction)
{
   return os << to_string(instruction);
}

torricelly_binary_output &torricelly::operator<<(torricelly_binary_output &os, torricelly_inst_ref_type inst_ref_type)
{
   switch (inst_ref_type)
   {
   case torricelly_inst_ref_type::SUBROUTINE:
      return os << 's';
   case torricelly_inst_ref_type::MODULE:
      return os << 'm';
   case torricelly_inst_ref_type::LABEL:
      return os << 'l';
   default:
      throw torricelly_error("Unknown instruction reference type while converting into binary");
   }
}

torricelly_binary_output &torricelly::operator<<(torricelly_binary_output &os, const torricelly_instruction &instruction)
{
   return os << (instruction.has_label()
                     ? (int32_t)instruction.label()
                     : (int32_t)0)
             << instruction.code();
   if (instruction.has_parameter_reference())
      os << instruction.ref_type() << (int32_t)instruction.parameter_reference();
   else
      os << (char)0 << (int32_t)0;
   return os;
}