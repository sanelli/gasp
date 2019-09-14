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
   unsigned int _ip; // instruction pointer
   torricelly_activation_record(const std::shared_ptr<torricelly_subroutine> subroutine);

   public:

   // Stack management
   void push(const torricelly_activation_record_variable value);
   torricelly_activation_record_variable pop();
   torricelly_activation_record_variable peek() const;

   // Instructions management
   unsigned int IP() const;
   torricelly_instruction next();

   // Variables management
   torricelly_activation_record_variable load(unsigned int index);
   void store(unsigned int index, torricelly_activation_record_variable variable);

   friend sanelli::memory;
};

std::shared_ptr<torricelly_activation_record> make_torricelly_activation_record(const std::shared_ptr<torricelly_subroutine> subroutine);

} // namespace gasp::torricelly::interpreter