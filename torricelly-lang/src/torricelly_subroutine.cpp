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
   : _name(name), _return_type(return_type) { }
std::string torricelly_subroutine::name() const { return _name; };
std::shared_ptr<torricelly::torricelly_type> torricelly_subroutine::return_type() const { return _return_type; }

void torricelly_subroutine::set_flags(torricelly_subroutine_flag flags) { 
   _flags = flags;
}
bool torricelly_subroutine::is(torricelly_subroutine_flag flag) const { 
   return (_flags & flag) == flag;
}
torricelly_subroutine_flag torricelly_subroutine::get_flags() const {
   return _flags;
}
unsigned int torricelly_subroutine::add_variable(std::shared_ptr<torricelly::torricelly_type> type, torricelly_value initial_value, bool is_parameter) {
   _variable_types.push_back(type);
   _variable_initial_values.push_back(initial_value);
   if(is_parameter)
      _parameters.insert(_variable_types.size());
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
   return _parameters.size();
}
bool torricelly_subroutine::is_parameter(unsigned int variable_index) const { 
   return _parameters.count(variable_index) > 0; 
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

unsigned int torricelly_subroutine::get_number_of_labels() const { return _labels_counter; }

void torricelly_subroutine::validate(unsigned int number_of_module_fields) const {

   // Check flags
   if(is(torricelly_subroutine_flag::STATIC) && is(torricelly_subroutine_flag::VIRTUAL))
      throw torricelly_error(make_string("Subroutine cannot be marked as both '", torricelly_subroutine_flag::STATIC,"' and '", torricelly_subroutine_flag::VIRTUAL,"'"));
   if(is(torricelly_subroutine_flag::STATIC) && is(torricelly_subroutine_flag::OVERRIDE))
      throw torricelly_error(make_string("Subroutine cannot be marked as both '", torricelly_subroutine_flag::STATIC,"' and '", torricelly_subroutine_flag::OVERRIDE,"'"));
   if(is(torricelly_subroutine_flag::STATIC) && is(torricelly_subroutine_flag::FINAL))
      throw torricelly_error(make_string("Subroutine cannot be marked as both '", torricelly_subroutine_flag::STATIC,"' and '", torricelly_subroutine_flag::FINAL,"'"));
   if(is(torricelly_subroutine_flag::VIRTUAL) && is(torricelly_subroutine_flag::OVERRIDE))
      throw torricelly_error(make_string("Subroutine cannot be marked as both '", torricelly_subroutine_flag::VIRTUAL,"' and '", torricelly_subroutine_flag::OVERRIDE,"'"));
   if(is(torricelly_subroutine_flag::VIRTUAL) && is(torricelly_subroutine_flag::FINAL))
      throw torricelly_error(make_string("Subroutine cannot be marked as both '", torricelly_subroutine_flag::VIRTUAL,"' and '", torricelly_subroutine_flag::FINAL,"'"));
   if(is(torricelly_subroutine_flag::PUBLIC) && is(torricelly_subroutine_flag::PRIVATE))
      throw torricelly_error(make_string("Subroutine cannot be marked as both '", torricelly_subroutine_flag::PUBLIC,"' and '", torricelly_subroutine_flag::PRIVATE,"'"));
   if(is(torricelly_subroutine_flag::PUBLIC) && is(torricelly_subroutine_flag::PROTECTED))
      throw torricelly_error(make_string("Subroutine cannot be marked as both '", torricelly_subroutine_flag::PUBLIC,"' and '", torricelly_subroutine_flag::PROTECTED,"'"));
   if(is(torricelly_subroutine_flag::PRIVATE) && is(torricelly_subroutine_flag::PROTECTED))
      throw torricelly_error(make_string("Subroutine cannot be marked as both '", torricelly_subroutine_flag::PRIVATE,"' and '", torricelly_subroutine_flag::PROTECTED,"'"));

   // Check variables count
   const unsigned int num_of_par = get_number_of_parameters();
   const unsigned int num_of_var = get_number_of_variables();
   const unsigned int num_of_labels = get_number_of_labels();
   if(num_of_par > num_of_var)
      throw torricelly_error(make_string("The number of parameters (", num_of_par,") is greater than the number of variables (",num_of_var,")"));
   
   // Check all variables matches their initial values
   for(auto variable_index = 0; variable_index < num_of_var; ++variable_index) {
      auto initial_value = _variable_initial_values.at(variable_index);
      auto variable_type = _variable_types.at(variable_index);
      if(!initial_value.match(variable_type))
         throw torricelly_error(make_string("Variable at index ", variable_index + 1," has type ", variable_type, " but initial value has type <type = ", initial_value.type(),", system_type = ", initial_value.system_type() ,">."));
   }
   
   // Check instructions
   for(auto instruction_index = 0; instruction_index < _instructions.size(); ++instruction_index)
   {  
      auto instruction = _instructions.at(instruction_index);
      try{
         instruction->validate();
      }catch(const torricelly_error& inst_error) { 
         throw torricelly_error(make_string("Instruction at ", instruction_index, " is not valid: ", inst_error.what()));
      }
      if(instruction->has_label() && instruction->label() > num_of_labels)
         throw torricelly_error(make_string("Instruction at ", instruction_index, " has an invalid label."));
      if(instruction->has_parameter_reference()) { 
         auto ref = instruction->parameter_reference();
         switch(instruction->ref_type()) { 
            case torricelly_inst_ref_type::MODULE:
               if(ref > number_of_module_fields)
                  throw torricelly_error(make_string("Instruction at ", instruction_index, " refers to an invalid module variable."));
               // TODO: Check if instruction match type of module variable
               break;
            case torricelly_inst_ref_type::SUBROUTINE:
               if(ref > num_of_var)
                  throw torricelly_error(make_string("Instruction at ", instruction_index, " refers to an invalid subroutine variable."));
                // TODO: Check if instruction match type of module variable
               break;
            case torricelly_inst_ref_type::LABEL:
               if(ref > num_of_labels)
                  throw torricelly_error(make_string("Instruction at ", instruction_index, " refers to an invalid label."));
               break;
            default:
               throw torricelly_error(make_string("Instruction at ", instruction_index, " refers to an unknown parameter reference type."));
         }
      }
    }
}

std::shared_ptr<torricelly::torricelly_subroutine> torricelly::make_torricelly_subroutine(const std::string& name, std::shared_ptr<torricelly::torricelly_type> return_type){
   return memory::gasp_make_shared<torricelly_subroutine>(name, return_type);
}

torricelly_subroutine_flag torricelly::operator|(torricelly_subroutine_flag value1, torricelly_subroutine_flag value2) {
   return static_cast<torricelly_subroutine_flag>(
      static_cast<std::underlying_type<torricelly_subroutine_flag>::type>(value1)
      |
      static_cast<std::underlying_type<torricelly_subroutine_flag>::type>(value2)
   );
}
torricelly_subroutine_flag torricelly::operator&(torricelly_subroutine_flag value1, torricelly_subroutine_flag value2) {
   return static_cast<torricelly_subroutine_flag>(
      static_cast<std::underlying_type<torricelly_subroutine_flag>::type>(value1)
      &
      static_cast<std::underlying_type<torricelly_subroutine_flag>::type>(value2)
   );
}

std::ostream& torricelly::operator<<(std::ostream& os, torricelly_subroutine_flag flag){
   return os << to_string(flag);
}

std::string torricelly::to_string(torricelly_subroutine_flag flag){
   switch(flag){
      case torricelly_subroutine_flag::NOTHING: return "";
      case torricelly_subroutine_flag::STATIC: return "static";
      case torricelly_subroutine_flag::VIRTUAL: return "virtual";
      case torricelly_subroutine_flag::OVERRIDE: return "override";
      case torricelly_subroutine_flag::FINAL: return "final";
      case torricelly_subroutine_flag::PUBLIC: return "public";
      case torricelly_subroutine_flag::PRIVATE: return "private";
      case torricelly_subroutine_flag::PROTECTED: return "protected";
      default:
         throw torricelly_error("Cannot write flag into output stream: unknown flag");
   }
}