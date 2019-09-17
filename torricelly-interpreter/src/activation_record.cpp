#include <memory>
#include <string>

#include <sanelli/sanelli.hpp>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/interpreter.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::interpreter;

torricelly_activation_record::torricelly_activation_record(const std::shared_ptr<torricelly_subroutine> subroutine) : _ip(0), _subroutine(subroutine) {}

void torricelly_activation_record::push(const torricelly_activation_record_variable value)
{
   return _stack.push(value);
}
torricelly_activation_record_variable torricelly_activation_record::pop()
{
   auto value = _stack.top();
   _stack.pop();
   return value;
}
torricelly_activation_record_variable torricelly_activation_record::peek() const
{
   return _stack.top();
}
std::shared_ptr<torricelly_subroutine> torricelly_activation_record::subroutine() const { return _subroutine; }
unsigned int torricelly_activation_record::ip() const { return _ip; }
void torricelly_activation_record::jump(unsigned int ip){ _ip = ip; }
bool torricelly_activation_record::move_next()
{
   _ip++;
   return _ip <= _subroutine->get_number_of_instructions();
}
torricelly_instruction torricelly_activation_record::instruction() const
{
   if (_ip <= 0 || _ip > _subroutine->get_number_of_instructions())
      throw torricelly_interpreter_error(sanelli::make_string("Cannot get instruction at ", _ip, " (maximum numer of instrutions is ", _subroutine->get_number_of_instructions(), ")."));
   return _subroutine->get_instruction(_ip);
}
unsigned int torricelly_activation_record::get_instruction_address(unsigned int label)
{
   return _subroutine->get_instruction_address(label);
}

torricelly_activation_record_variable torricelly_activation_record::load(unsigned int index)
{
   auto it = _variables.find(index);
   if (it == _variables.end())
      throw torricelly_interpreter_error(sanelli::make_string("Cannot get variable at ", index, "."));
   return it->second;
}
void torricelly_activation_record::store(unsigned int index, torricelly_activation_record_variable variable)
{
   _variables[index] = variable;
}
torricelly_activation_record_variable_type torricelly_activation_record::load_type(unsigned int index) { 
      auto it = _variables.find(index);
   if (it == _variables.end())
      throw torricelly_interpreter_error(sanelli::make_string("Cannot get variable at ", index, "."));
   return it->second.type();
}

void torricelly_activation_record::register_module_variable(unsigned int index, torricelly_activation_record_variable *variable)
{
   _module_variables[index] = variable;
}
torricelly_activation_record_variable *torricelly_activation_record::load_module_variable(unsigned int index)
{
   auto it = _module_variables.find(index);
   if (it == _module_variables.end())
      throw torricelly_interpreter_error(sanelli::make_string("Cannot get module variable at ", index, "."));
   return it->second;
}

std::shared_ptr<torricelly_activation_record> gasp::torricelly::interpreter::make_torricelly_activation_record(std::shared_ptr<torricelly_subroutine> subroutine)
{
   return sanelli::memory::make_shared<torricelly_activation_record>(subroutine);
}
