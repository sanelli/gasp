#pragma once

#include <memory>
#include <gasp/common/memory.hpp>
#include <gasp/torricelly/impl/torricelly_instruction_code.hpp>

namespace gasp::torricelly {

class torricelly_instruction {
   torricelly_inst_code _code;
   unsigned int _parameter_reference;
   unsigned int _label;
   torricelly_instruction(torricelly_inst_code code, unsigned int parameter_reference);
   torricelly_instruction(torricelly_inst_code code);
public:
   torricelly_inst_code code() const;
   unsigned int parameter_reference() const;
   void set_parameter_reference(unsigned int parameter_reference);
   unsigned int label() const;
   void set_label(unsigned int label);

   friend gasp::common::memory;
};

std::shared_ptr<torricelly_instruction> make_torricelly_instruction(torricelly_inst_code code);
std::shared_ptr<torricelly_instruction> make_torricelly_instruction(torricelly_inst_code code, unsigned int parameter_reference);

}