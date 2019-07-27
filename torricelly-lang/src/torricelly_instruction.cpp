#include <memory>

#include <gasp/common/memory.hpp>
#include <gasp/torricelly/torricelly.hpp>

using namespace gasp;
using namespace gasp::common;
using namespace gasp::torricelly;

torricelly_instruction::torricelly_instruction(torricelly_inst_code code, unsigned int parameter_reference)
   : _code(code), _parameter_reference(parameter_reference), _label(0) { };
torricelly_instruction::torricelly_instruction(torricelly_inst_code code)
   : _code(code), _parameter_reference(0), _label(0) { };
torricelly_inst_code torricelly_instruction::code() const { return _code; }
unsigned int torricelly_instruction::parameter_reference() const { return _parameter_reference; }
void torricelly_instruction::set_parameter_reference(unsigned int parameter_reference) { _parameter_reference = parameter_reference; }
unsigned int torricelly_instruction::label() const { return _label; }
void torricelly_instruction::set_label(unsigned int label) { _label = label; }

std::shared_ptr<torricelly_instruction> torricelly::make_torricelly_instruction(torricelly_inst_code code) {
   return memory::gasp_make_shared<torricelly_instruction>(code);
}

std::shared_ptr<torricelly_instruction> torricelly::make_torricelly_instruction(torricelly_inst_code code, unsigned int parameter_reference){
   return memory::gasp_make_shared<torricelly_instruction>(code, parameter_reference);
}
