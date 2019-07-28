#pragma once

#include <string>
#include <memory>
#include <vector>
#include <ostream>

#include <gasp/common/memory.hpp>
#include <gasp/torricelly/impl/torricelly_type.hpp>
#include <gasp/torricelly/impl/torricelly_instruction.hpp>
#include <gasp/torricelly/impl/torricelly_variable_value.hpp>

namespace gasp::torricelly {

enum class torricelly_subroutine_flag : unsigned short {
   NOTHING = 0,
   STATIC = 1 << 1,
   VIRTUAL = 1 << 2,
   OVERRIDE = 1 << 3,
   FINAL = 1 << 4,
   PUBLIC = 1 << 5,
   PRIVATE = 1 << 6,
   PROTECTED = 1 << 7
};

torricelly_subroutine_flag operator|(torricelly_subroutine_flag value1, torricelly_subroutine_flag value2);
torricelly_subroutine_flag operator&(torricelly_subroutine_flag value1, torricelly_subroutine_flag value2);
std::ostream& operator<<(std::ostream& os, torricelly_subroutine_flag flag);

class torricelly_subroutine {
   std::string _name;
   std::shared_ptr<torricelly::torricelly_type> _return_type;
   std::vector<std::shared_ptr<torricelly::torricelly_type>> _variable_types;
   std::vector<torricelly_value> _variable_initial_values;
   std::vector<std::shared_ptr<torricelly::torricelly_instruction>> _instructions;
   unsigned int _labels_counter;
   unsigned int _number_of_parameters;
   torricelly_subroutine_flag _flags;
   torricelly_subroutine(const std::string& name, std::shared_ptr<torricelly::torricelly_type> return_type);
public:
   std::string name() const;
   std::shared_ptr<torricelly::torricelly_type> return_type() const;
   void set_flags(torricelly_subroutine_flag flags);
   bool is(torricelly_subroutine_flag flag) const;

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
   unsigned int get_number_of_labels() const;

   void validate(unsigned int number_of_module_fields) const;

   friend gasp::common::memory;
};

std::shared_ptr<torricelly_subroutine> make_torricelly_subroutine(const std::string& name, std::shared_ptr<torricelly::torricelly_type> return_type);

}