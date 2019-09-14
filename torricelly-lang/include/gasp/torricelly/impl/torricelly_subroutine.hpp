#pragma once

#include <string>
#include <memory>
#include <vector>
#include <ostream>
#include <set>
#include <map>

#include <gasp/torricelly/impl/torricelly_type.hpp>
#include <gasp/torricelly/impl/torricelly_instruction.hpp>
#include <gasp/torricelly/impl/torricelly_value.hpp>

namespace gasp::torricelly {

enum class torricelly_subroutine_flag : unsigned short {
   NOTHING = 0,
   MAIN = 1 << 1,
   STATIC = 1 << 2,
   VIRTUAL = 1 << 3,
   OVERRIDE = 1 << 4,
   FINAL = 1 << 5,
   PUBLIC = 1 << 6,
   PRIVATE = 1 << 7,
   PROTECTED = 1 << 8,
   NATIVE = 1 << 9
};

torricelly_subroutine_flag operator|(torricelly_subroutine_flag value1, torricelly_subroutine_flag value2);
torricelly_subroutine_flag operator&(torricelly_subroutine_flag value1, torricelly_subroutine_flag value2);
std::ostream& operator<<(std::ostream& os, torricelly_subroutine_flag flag);
std::string to_string(torricelly_subroutine_flag flag);

class torricelly_subroutine {
   std::string _name;
   std::shared_ptr<torricelly::torricelly_type> _return_type;
   std::vector<std::shared_ptr<torricelly::torricelly_type>> _variable_types;
   std::vector<torricelly_value> _variable_initial_values;
   std::vector<torricelly::torricelly_instruction> _instructions;
   unsigned int _labels_counter;
   std::set<unsigned int> _parameters;
   torricelly_subroutine_flag _flags;
   torricelly_subroutine(const std::string& name, std::shared_ptr<torricelly::torricelly_type> return_type);
public:
   std::string name() const;
   std::shared_ptr<torricelly::torricelly_type> return_type() const;
   void set_flags(torricelly_subroutine_flag flags);
   torricelly_subroutine_flag get_flags() const;
   bool is(torricelly_subroutine_flag flag) const;

   unsigned int add_variable(std::shared_ptr<torricelly::torricelly_type> type, torricelly_value initial_value, bool is_parameter = false);
   std::shared_ptr<torricelly::torricelly_type> get_variable_type(unsigned int index) const;
   torricelly_value get_initial_value(unsigned int index) const;
   unsigned int get_number_of_variables() const;
   unsigned int get_number_of_parameters() const;
   bool is_parameter(unsigned int variable_index) const;

   unsigned int append_instruction(torricelly::torricelly_instruction instruction);
   void insert_instruction(unsigned int index, torricelly::torricelly_instruction instruction);
   void replace_instruction(unsigned int index, torricelly::torricelly_instruction instruction);
   void remove_instruction(unsigned int index);
   torricelly::torricelly_instruction get_instruction(unsigned int index) const;
   unsigned int get_number_of_instructions() const;

   unsigned int next_label();
   unsigned int get_number_of_labels() const;

   void validate(unsigned int number_of_module_fields) const;

   friend sanelli::memory;
};

std::shared_ptr<torricelly_subroutine> make_torricelly_subroutine(const std::string& name, std::shared_ptr<torricelly::torricelly_type> return_type);

}