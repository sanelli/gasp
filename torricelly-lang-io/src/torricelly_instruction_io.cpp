#include <memory>
#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/torricelly_io.hpp>

using namespace gasp;
using namespace gasp::torricelly;

torricelly_text_output& torricelly::operator<<(torricelly_text_output& os, torricelly_inst_ref_type inst_ref_type){
   switch(inst_ref_type){
      case torricelly_inst_ref_type::MODULE: return os << "$";
      case torricelly_inst_ref_type::SUBROUTINE: return os << "#";
      case torricelly_inst_ref_type::LABEL: return os << "@";
      default:
         throw torricelly_error("Unknown instruction parameter refernce type");
   }
}

torricelly_text_output& torricelly::operator<<(torricelly_text_output& os, const std::shared_ptr<const torricelly_instruction> instruction){
   if(instruction->has_label())
      os << torricelly_inst_ref_type::LABEL << instruction->label() << ": ";
   os << instruction->code();
   if(instruction->has_parameter_reference())
      os <<  " " << instruction->ref_type() << instruction->parameter_reference();
   return os;
}