#include <memory>

#include <gasp/common/memory.hpp>
#include <gasp/torricelly/torricelly.hpp>

using namespace gasp;
using namespace gasp::common;
using namespace gasp::torricelly;

torricelly_instruction::torricelly_instruction(torricelly_inst_code code, unsigned int parameter_reference, torricelly_inst_ref_type ref_type)
   : _code(code), _parameter_reference(parameter_reference), _ref_type(ref_type), _label(0) { };
torricelly_instruction::torricelly_instruction(torricelly_inst_code code)
   : _code(code), _parameter_reference(0), _label(0) { };
torricelly_inst_code torricelly_instruction::code() const { return _code; }
bool torricelly_instruction::has_parameter_reference() const { return _parameter_reference > 0; }
unsigned int torricelly_instruction::parameter_reference() const { return _parameter_reference; }
torricelly_inst_ref_type torricelly_instruction::ref_type() const { return _ref_type; }
void torricelly_instruction::set_parameter_reference(unsigned int parameter_reference, torricelly_inst_ref_type ref_type) { 
   _parameter_reference = parameter_reference;
   _ref_type = ref_type;
}
unsigned int torricelly_instruction::label() const { return _label; }
bool torricelly_instruction::has_label() const { return _label > 0; }
void torricelly_instruction::set_label(unsigned int label) { _label = label; }

std::shared_ptr<torricelly_instruction> torricelly::make_torricelly_instruction(torricelly_inst_code code) {
   return memory::gasp_make_shared<torricelly_instruction>(code);
}

std::shared_ptr<torricelly_instruction> torricelly::make_torricelly_instruction(torricelly_inst_code code, unsigned int parameter_reference, torricelly_inst_ref_type ref_type){
   return memory::gasp_make_shared<torricelly_instruction>(code, parameter_reference, ref_type);
}
