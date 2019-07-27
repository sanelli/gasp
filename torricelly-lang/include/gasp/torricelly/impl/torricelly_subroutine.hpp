#pragma once

#include <string>
#include <memory>
#include <vector>

#include <gasp/common/memory.hpp>
#include <gasp/torricelly/impl/torricelly_type.hpp>
#include <gasp/torricelly/impl/torricelly_instruction.hpp>

namespace gasp::torricelly {

class torricelly_subroutine {
   std::string _name;
   std::shared_ptr<torricelly::torricelly_type> _return_type;
   std::vector<std::shared_ptr<torricelly::torricelly_type>> _parameters;
   std::vector<std::shared_ptr<torricelly::torricelly_type>> _variables;
   std::vector<std::shared_ptr<torricelly::torricelly_instruction>> _instructions;

   torricelly_subroutine(const std::string& name, std::shared_ptr<torricelly::torricelly_type> return_type);
public:
   std::string name() const;
   std::shared_ptr<torricelly::torricelly_type> return_type() const;
   unsigned int add_parameter(std::shared_ptr<torricelly::torricelly_type> type);
   unsigned int add_variable(std::shared_ptr<torricelly::torricelly_type> type);

   unsigned int append_instruction(std::shared_ptr<torricelly::torricelly_instruction> instruction);
   void insert_instruction(unsigned int index, std::shared_ptr<torricelly::torricelly_instruction> instruction);
   void replace_instruction(unsigned int index, std::shared_ptr<torricelly::torricelly_instruction> instruction);
   void remove_instruction(unsigned int index);
   std::shared_ptr<torricelly::torricelly_instruction> get_instruction(unsigned int index) const;
   unsigned int count_instructions() const;

   friend gasp::common::memory;
};

std::shared_ptr<torricelly_subroutine> make_torricelly_subroutine(const std::string& name, std::shared_ptr<torricelly::torricelly_type> return_type);

}