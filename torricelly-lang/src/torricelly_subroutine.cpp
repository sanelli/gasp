#include <memory>
#include <string>
#include <vector>

#include <gasp/common/string.hpp>
#include <gasp/common/memory.hpp>
#include <gasp/torricelly/torricelly.hpp>

using namespace gasp;
using namespace gasp::common;
using namespace gasp::torricelly;

torricelly_subroutine::torricelly_subroutine(const std::string& name, 
   std::shared_ptr<torricelly::torricelly_type> return_type)
   : _name(name), _return_type(return_type), _number_of_parameters(0) { }
std::string torricelly_subroutine::name() const { return _name; };
std::shared_ptr<torricelly::torricelly_type> torricelly_subroutine::return_type() const { return _return_type; }

unsigned int torricelly_subroutine::add_variable(std::shared_ptr<torricelly::torricelly_type> type, torricelly_value initial_value, bool is_parameter) {
   _variable_types.push_back(type);
   _variable_initial_values.push_back(initial_value);
   if(is_parameter)
      _number_of_parameters++;
   return _variable_types.size();
}
std::shared_ptr<torricelly::torricelly_type> torricelly_subroutine::get_variable_type(unsigned int index) const {
   if(index > _variable_types.size())
      throw torricelly_error(make_string("Cannot access variable type at index ", index,"."));
   return _variable_types.at(index - 1);
}
torricelly_value torricelly_subroutine::get_initial_value(unsigned int index) const {
   if(index > _variable_initial_values.size())
      throw torricelly_error(make_string("Cannot access variable initial value at index ", index,"."));
   return _variable_initial_values.at(index -1);
}
unsigned int torricelly_subroutine::get_number_of_variables() const {
   return _variable_types.size();
}
unsigned int torricelly_subroutine::get_number_of_parameters() const {
   return _number_of_parameters;
}

unsigned int torricelly_subroutine::append_instruction(std::shared_ptr<torricelly::torricelly_instruction> instruction) {
   _instructions.push_back(instruction);
   return _instructions.size();
}
void torricelly_subroutine::insert_instruction(unsigned int index, std::shared_ptr<torricelly::torricelly_instruction> instruction) {
   _instructions.insert(_instructions.begin() + index - 1, instruction);
}
void torricelly_subroutine::replace_instruction(unsigned int index, std::shared_ptr<torricelly::torricelly_instruction> instruction) {
   _instructions.at(index - 1) = instruction;
}
void torricelly_subroutine::remove_instruction(unsigned int index) { 
   _instructions.erase(_instructions.begin() + index - 1); 
}
std::shared_ptr<torricelly::torricelly_instruction> torricelly_subroutine::get_instruction(unsigned int index) const { 
   return _instructions.at(index-1); 
}
unsigned int torricelly_subroutine::get_number_of_instructions() const { 
   return _instructions.size(); 
}

unsigned int torricelly_subroutine::next_label(){
   return ++_labels_counter;
}

void torricelly_subroutine::validate() const { 
   const unsigned int num_of_par = get_number_of_parameters();
   const unsigned int num_of_var = get_number_of_variables();
   if(num_of_par > num_of_var)
      throw torricelly_error(make_string("The number of parameters (", num_of_par,") is greather than the number of variables (",num_of_var,")"));
   for(auto instruction : _instructions)
      instruction->validate();
}

std::shared_ptr<torricelly::torricelly_subroutine> torricelly::make_torricelly_subroutine(const std::string& name, std::shared_ptr<torricelly::torricelly_type> return_type){
   return memory::gasp_make_shared<torricelly_subroutine>(name, return_type);
}

