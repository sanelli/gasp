#pragma once

#include <memory>
#include <stack>
#include <map>

#include <sanelli/sanelli.hpp>
#include <gasp/torricelly/torricelly.hpp>

#include <gasp/torricelly/impl/activation_record_variable.hpp>

namespace gasp::torricelly::interpreter
{

class torricelly_activation_record
{
   std::shared_ptr<torricelly_subroutine> _subroutine;
   std::stack<torricelly_activation_record_variable> _stack;
   std::map<unsigned int, torricelly_activation_record_variable> _variables;
   std::map<unsigned int, torricelly_activation_record_variable *> _module_variables;

   unsigned int _ip; // instruction pointer
   torricelly_activation_record(std::shared_ptr<torricelly_subroutine> subroutine);

public:
   std::shared_ptr<torricelly_subroutine> subroutine() const;

   // Stack management
   void push(const torricelly_activation_record_variable value);
   torricelly_activation_record_variable pop();
   torricelly_activation_record_variable peek() const;

   // Instructions management
   unsigned int ip() const;
   void jump(unsigned int ip);
   bool move_next();
   torricelly_instruction instruction() const;

   // Variables management
   torricelly_activation_record_variable load(unsigned int index);
   void store(unsigned int index, torricelly_activation_record_variable variable);
   torricelly_activation_record_variable_type load_type(unsigned int index);

   // Module variables management
   void register_module_variable(unsigned int index, torricelly_activation_record_variable *variable);
   torricelly_activation_record_variable *load_module_variable(unsigned int index);

   friend sanelli::memory;
};

std::shared_ptr<torricelly_activation_record> make_torricelly_activation_record(const std::shared_ptr<torricelly_subroutine> subroutine);

} // namespace gasp::torricelly::interpreter