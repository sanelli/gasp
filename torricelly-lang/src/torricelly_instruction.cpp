#include <memory>

#include <sanelli/sanelli.hpp>
#include <gasp/torricelly/torricelly.hpp>

using namespace sanelli;
using namespace gasp;
using namespace gasp::torricelly;

torricelly_instruction::torricelly_instruction(const torricelly_inst_code code, const unsigned int parameter_reference, const torricelly_inst_ref_type ref_type)
    : _code(code), _parameter_reference(parameter_reference), _ref_type(ref_type), _label(0){};
torricelly_instruction::torricelly_instruction(const torricelly_inst_code code)
    : _code(code), _parameter_reference(0), _label(0){};
torricelly_instruction::torricelly_instruction(const torricelly_instruction &other)
    : _code(other._code), _parameter_reference(other._parameter_reference), _ref_type(other._ref_type), _label(other._label) {}
torricelly_instruction::torricelly_instruction(torricelly_instruction &&other)
    : _code(std::move(other._code)), _parameter_reference(std::move(other._parameter_reference)),
      _ref_type(std::move(other._ref_type)), _label(std::move(other._label)) {}

torricelly_instruction::~torricelly_instruction() {}

torricelly_instruction &torricelly_instruction::operator=(torricelly_instruction &&other)
{
   if (&other == this)
      return *this;
   _code = other._code;
   _parameter_reference = other._parameter_reference;
   _ref_type = other._ref_type;
   _label = other._label;
   return *this;
}
torricelly_instruction &torricelly_instruction::operator=(const torricelly_instruction &other)
{
   _code = std::move(other._code);
   _parameter_reference = std::move(other._parameter_reference);
   _ref_type = std::move(other._ref_type);
   _label = std::move(other._label);
   return *this;
}

torricelly_inst_code torricelly_instruction::code() const { return _code; }
bool torricelly_instruction::has_parameter_reference() const { return _parameter_reference > 0; }
unsigned int torricelly_instruction::parameter_reference() const { return _parameter_reference; }
torricelly_inst_ref_type torricelly_instruction::ref_type() const { return _ref_type; }
void torricelly_instruction::set_parameter_reference(unsigned int parameter_reference, torricelly_inst_ref_type ref_type)
{
   _parameter_reference = parameter_reference;
   _ref_type = ref_type;
}
unsigned int torricelly_instruction::label() const { return _label; }
bool torricelly_instruction::has_label() const { return _label > 0; }
void torricelly_instruction::set_label(unsigned int label) { _label = label; }

void torricelly_instruction::validate() const
{
   const torricelly_inst_code cd = code();
   if (has_parameter_reference() && !torricelly_inst_code_helper::accept_parameter(cd))
      throw torricelly_error(sanelli::make_string("Instruction code '", cd, "' does not accept any parameter"));
   if (!has_parameter_reference() && torricelly_inst_code_helper::accept_parameter(cd))
      throw torricelly_error(sanelli::make_string("Instruction code '", cd, "' requires a parameter"));
   switch (ref_type())
   {
   case torricelly_inst_ref_type::LABEL:
      if (!torricelly_inst_code_helper::accept_label(cd))
         throw torricelly_error(sanelli::make_string("Instruction code '", cd, "' does not accept a label parameter"));
      break;
   case torricelly_inst_ref_type::MODULE:
      if (!torricelly_inst_code_helper::accept_reference(cd))
         throw torricelly_error(sanelli::make_string("Instruction code '", cd, "' does not accept a module variable reference"));
      break;
   case torricelly_inst_ref_type::SUBROUTINE:
      if (!torricelly_inst_code_helper::accept_reference(cd))
         throw torricelly_error(sanelli::make_string("Instruction code '", cd, "' does not accept a subroutine variable reference"));
      break;
   default:
      throw torricelly_error("Unknwon instruction reference type detected during validation");
   }
}

torricelly_instruction torricelly_instruction::make(const torricelly_inst_code code, const unsigned int parameter_reference, const torricelly_inst_ref_type ref_type) { 
   return torricelly_instruction(code, parameter_reference, ref_type);
}

torricelly_instruction torricelly_instruction::make(const torricelly_inst_code code) { 
   return torricelly_instruction(code);
}
