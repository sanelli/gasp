#include <memory>
#include <string>

#include <iostream>

#include <sanelli/sanelli.hpp>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/interpreter.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::interpreter;

torricelly_activation_record::torricelly_activation_record(std::shared_ptr<torricelly_module> module, std::shared_ptr<torricelly_subroutine> subroutine)
    : _ip(0), _subroutine(subroutine), _module(module) {}

void torricelly_activation_record::push(const torricelly_activation_record_local value)
{
   return _stack.push_back(value);
}
torricelly_activation_record_local torricelly_activation_record::pop()
{
   auto value = _stack.back();
   _stack.pop_back();
   return value;
}
torricelly_activation_record_local torricelly_activation_record::peek() const
{
   return _stack.back();
}

typename std::vector<torricelly_activation_record_local>::const_reverse_iterator torricelly_activation_record::begin_stack() const
{
   return _stack.rbegin();
}

typename std::vector<torricelly_activation_record_local>::const_reverse_iterator torricelly_activation_record::end_stack() const
{
   return _stack.rend();
}

std::shared_ptr<torricelly_module> torricelly_activation_record::module() const { return _module; }
std::shared_ptr<torricelly_subroutine> torricelly_activation_record::subroutine() const { return _subroutine; }
unsigned int torricelly_activation_record::ip() const { return _ip; }
void torricelly_activation_record::jump(unsigned int ip) { _ip = ip; }
bool torricelly_activation_record::move_next()
{
   ++_ip;
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

torricelly_activation_record_local torricelly_activation_record::load(unsigned int index)
{
   auto it = _locals.find(index);
   if (it == _locals.end())
      throw torricelly_interpreter_error(sanelli::make_string("Cannot get local at ", index, "."));
   return it->second;
}
void torricelly_activation_record::store(unsigned int index, torricelly_activation_record_local local)
{
   _locals[index] = local;
}
torricelly_activation_record_local_type torricelly_activation_record::load_type(unsigned int index)
{
   auto it = _locals.find(index);
   if (it == _locals.end())
      throw torricelly_interpreter_error(sanelli::make_string("Cannot get type for local at ", index, "."));
   return it->second.type();
}

void torricelly_activation_record::reference_module_local(unsigned int index, torricelly_activation_record_local *local)
{
   _module_locals[index] = local;
}
torricelly_activation_record_local *torricelly_activation_record::get_module_local(unsigned int index)
{
   auto it = _module_locals.find(index);
   if (it == _module_locals.end())
      throw torricelly_interpreter_error(sanelli::make_string("Cannot get module local at ", index, "."));
   return it->second;
}

unsigned int torricelly_activation_record::count_module_locals() const { 
   return _module_locals.size();
}

std::shared_ptr<torricelly_activation_record> gasp::torricelly::interpreter::make_torricelly_activation_record(std::shared_ptr<torricelly_module> module, std::shared_ptr<torricelly_subroutine> subroutine)
{
   return sanelli::memory::make_shared<torricelly_activation_record>(module, subroutine);
}
