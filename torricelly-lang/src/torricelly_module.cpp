#include <string>
#include <memory>
#include <vector>

#include <gasp/common/memory.hpp>
#include <gasp/torricelly/torricelly.hpp>

using namespace gasp;
using namespace gasp::common;
using namespace gasp::torricelly;

torricelly_module::torricelly_module(const std::string& module_name)
   : _module_name(module_name) { }

std::string torricelly_module::module_name() const { return _module_name; }

unsigned int torricelly_module::add_subroutine(std::shared_ptr<torricelly_subroutine> subroutine) {
   _subroutines.push_back(subroutine);
   return _subroutines.size();
}
unsigned int torricelly_module::get_numbers_of_subroutines() const { 
   return _subroutines.size(); 
}
std::shared_ptr<torricelly_subroutine> torricelly_module::get_subroutine(unsigned int index) const {
   return _subroutines.at(index);
}

unsigned int torricelly_module::add_variable(std::shared_ptr<torricelly::torricelly_type> type, torricelly_value initial_value) {
   _variable_types.push_back(type);
   _variable_initial_values.push_back(initial_value);
   return _variable_types.size();
}
std::shared_ptr<torricelly::torricelly_type> torricelly_module::get_variable_type(unsigned int index) const {
   return _variable_types.at(index);
}
torricelly_value torricelly_module::get_initial_value(unsigned int index) const {
   return _variable_initial_values.at(index);
}
unsigned int torricelly_module::get_number_of_variables() const {
   return _variable_types.size();
}

std::shared_ptr<torricelly_module> torricelly::make_torricelly_module(const std::string& module_name){
 return memory::gasp_make_shared<torricelly_module>(module_name);
}