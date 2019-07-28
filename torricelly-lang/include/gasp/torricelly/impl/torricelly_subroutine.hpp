#pragma once

#include <string>
#include <memory>
#include <vector>

#include <gasp/common/memory.hpp>
#include <gasp/torricelly/impl/torricelly_type.hpp>
#include <gasp/torricelly/impl/torricelly_instruction.hpp>
#include <gasp/torricelly/impl/torricelly_variable_value.hpp>

namespace gasp::torricelly {

class torricelly_subroutine {
   std::string _name;
   std::shared_ptr<torricelly::torricelly_type> _return_type;
   std::vector<std::shared_ptr<torricelly::torricelly_type>> _variable_types;
   std::vector<torricelly_value> _variable_initial_values;
   std::vector<std::shared_ptr<torricelly::torricelly_instruction>> _instructions;
   unsigned int _labels_counter;
   unsigned int _number_of_parameters;
   torricelly_subroutine(const std::string& name, std::shared_ptr<torricelly::torricelly_type> return_type);
public:
   std::string name() const;
   std::shared_ptr<torricelly::torricelly_type> return_type() const;
   unsigned int add_variable(std::shared_ptr<torricelly::torricelly_type> type, torricelly_value initial_value, bool is_parameter = false);
   std::shared_ptr<torricelly::torricelly_type> get_variable_type(unsigned int index) const;
   torricelly_value get_initial_value(unsigned int index) const;
   unsigned int get_number_of_variables() const;
   unsigned int get_number_of_parameters() const;

   unsigned int append_instruction(std::shared_ptr<torricelly::torricelly_instruction> instruction);
   void insert_instruction(unsigned int index, std::shared_ptr<torricelly::torricelly_instruction> instruction);
   void replace_instruction(unsigned int index, std::shared_ptr<torricelly::torricelly_instruction> instruction);
   void remove_instruction(unsigned int index);
   std::shared_ptr<torricelly::torricelly_instruction> get_instruction(unsigned int index) const;
   unsigned int get_number_of_instructions() const;

   unsigned int next_label();

   void validate() const;

   friend gasp::common::memory;
};

std::shared_ptr<torricelly_subroutine> make_torricelly_subroutine(const std::string& name, std::shared_ptr<torricelly::torricelly_type> return_type);

}