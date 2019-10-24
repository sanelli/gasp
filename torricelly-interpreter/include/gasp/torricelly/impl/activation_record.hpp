#pragma once

#include <memory>
#include <stack>
#include <map>

#include <sanelli/sanelli.hpp>
#include <gasp/torricelly/torricelly.hpp>

#include <gasp/torricelly/impl/activation_record_local.hpp>

namespace gasp::torricelly::interpreter
{

class torricelly_activation_record
{
   std::shared_ptr<torricelly_module> _module;
   std::shared_ptr<torricelly_subroutine> _subroutine;
   std::vector<torricelly_activation_record_local> _stack;
   std::map<unsigned int, torricelly_activation_record_local> _locals;
   std::map<unsigned int, torricelly_activation_record_local *> _module_locals;

   unsigned int _ip; // instruction pointer
   torricelly_activation_record(std::shared_ptr<torricelly_module> module, std::shared_ptr<torricelly_subroutine> subroutine);

public:
   std::shared_ptr<torricelly_module> module() const;
   std::shared_ptr<torricelly_subroutine> subroutine() const;

   // Stack management
   void push(const torricelly_activation_record_local value);
   torricelly_activation_record_local pop();
   torricelly_activation_record_local peek() const;
   typename std::vector<torricelly_activation_record_local>::const_reverse_iterator begin_stack() const;
   typename std::vector<torricelly_activation_record_local>::const_reverse_iterator end_stack() const;

   // Instructions management
   unsigned int ip() const;
   void jump(unsigned int ip);
   bool move_next();
   torricelly_instruction instruction() const;
   unsigned int get_instruction_address(unsigned int label);

   // locals management
   torricelly_activation_record_local load(unsigned int index);
   void store(unsigned int index, torricelly_activation_record_local local);
   torricelly_activation_record_local_type load_type(unsigned int index);

   // Module locals management
   void reference_module_local(unsigned int index, torricelly_activation_record_local *local);
   torricelly_activation_record_local *get_module_local(unsigned int index);
   unsigned int count_module_locals() const;

   friend sanelli::memory;
};

std::shared_ptr<torricelly_activation_record> make_torricelly_activation_record(std::shared_ptr<torricelly_module> module, std::shared_ptr<torricelly_subroutine> subroutine);

} // namespace gasp::torricelly::interpreter