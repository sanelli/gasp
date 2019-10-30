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
   os << (instruction.has_label()
                     ? (int32_t)instruction.label()
                     : (int32_t)0)
             << instruction.code();
   if (instruction.has_parameter_reference())
   {
      os << instruction.ref_type();
      os << (int32_t)instruction.parameter_reference();
   }
   else
   {
      os << (char)0;
      os << (int32_t)0;
   }
   return os;
}

torricelly_binary_input &torricelly::operator>>(torricelly_binary_input &is, torricelly_inst_ref_type &inst_ref_type)
{
   char code;
   is >> code;
   switch (code)
   {
   case 0:
      inst_ref_type = torricelly_inst_ref_type::UNDEFINED;
      break;
   case 'm':
      inst_ref_type = torricelly_inst_ref_type::MODULE;
      break;
   case 's':
      inst_ref_type = torricelly_inst_ref_type::SUBROUTINE;
      break;
   case 'l':
      inst_ref_type = torricelly_inst_ref_type::LABEL;
      break;
   default:
      throw torricelly_error(
          sanelli::make_string("Error while parsing file. Cannot parse instruction reference type. Unknown value (",
                               (int)code, ")"));
   }
   return is;
}

torricelly_binary_input &torricelly::operator>>(torricelly_binary_input &is, torricelly_instruction &instruction)
{
   int32_t label;
   torricelly_inst_code instruction_code;
   torricelly_inst_ref_type ref_type;
   int32_t reference;
   is >> label;
   is >> instruction_code;
   is >> ref_type;
   is >> reference;
   
   instruction = torricelly_instruction::make(instruction_code);
   if (label > 0)
      instruction.set_label(label);
   if (ref_type != torricelly_inst_ref_type::UNDEFINED)
      instruction.set_parameter_reference(reference, ref_type);
   return is;
}