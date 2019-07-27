#include <memory>
#include <string>
#include <vector>

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
unsigned int torricelly_subroutine::add_parameter(std::shared_ptr<torricelly::torricelly_type> type){
   _parameters.push_back(type);
   return _parameters.size();
}
unsigned int torricelly_subroutine::add_variable(std::shared_ptr<torricelly::torricelly_type> type) {
   _variables.push_back(type);
   return _variables.size();
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
unsigned int torricelly_subroutine::count_instructions() const { 
   return _instructions.size(); 
}

std::shared_ptr<torricelly::torricelly_subroutine> torricelly::make_torricelly_subroutine(const std::string& name, std::shared_ptr<torricelly::torricelly_type> return_type){
   return memory::gasp_make_shared<torricelly_subroutine>(name, return_type);
}