#pragma once

#include <memory>
#include <stack>
#include <string>

#include <sanelli/sanelli.hpp>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/impl/activation_record.hpp>
#include <gasp/torricelly/impl/activation_record_local.hpp>
#include <gasp/torricelly/impl/torricelly_instruction_interpreter.hpp>
#include <gasp/torricelly/impl/torricelly_native_library_loader.hpp>

namespace gasp::torricelly::interpreter
{

enum class torricelly_interpreter_status : unsigned char {
   ZERO,
   INITIALIZED,
   RUNNING,
   HALTED,
   FINISHED,
   MALFORMED_SUBROUTINE_ENDING
};

std::string to_string(const torricelly_interpreter_status status);

class torricelly_interpreter : public std::enable_shared_from_this<torricelly_interpreter>
{
   std::shared_ptr<gasp::torricelly::torricelly_module> _main_module;
   std::vector<std::shared_ptr<torricelly_activation_record>> _activation_records;
   torricelly_interpreter_status _status;
   std::stack<torricelly_activation_record_local> _parameters_passing;
   std::map<std::string, std::shared_ptr<std::map<unsigned int, torricelly_activation_record_local>>> _module_locals_mapping;
   std::function<std::string(unsigned int)> _get_parameter;
   std::shared_ptr<torricelly_instruction_interpreter> _instruction_interpreter;
   torricelly_activation_record_local _return_value;
   std::shared_ptr<sanelli::shared_library_loader<torricelly_native_library>> _native_module_loader;

   // Need to force the creation of shared pointer in order to be able to use share_from_this feature
   torricelly_interpreter(std::shared_ptr<gasp::torricelly::torricelly_module> main_module, std::function<std::string(unsigned int)> get_parameter);

   void store_module_locals(std::shared_ptr<gasp::torricelly::torricelly_module> module);
   void push_activation_record(std::shared_ptr<gasp::torricelly::torricelly_module> module, std::shared_ptr<gasp::torricelly::torricelly_subroutine> subroutine);
   void pop_activation_record();

public:
   std::shared_ptr<gasp::torricelly::torricelly_module> main_module() const;

   torricelly_interpreter_status status() const;

   void initialize();
   void run();
   void step();
   torricelly_activation_record_local return_value() const;
   std::shared_ptr<torricelly_activation_record> activation_record() const;
   typename std::vector<std::shared_ptr<torricelly_activation_record>>::const_reverse_iterator begin_stack_trace() const;
   typename std::vector<std::shared_ptr<torricelly_activation_record>>::const_reverse_iterator end_stack_trace() const;

   friend sanelli::memory;
   friend torricelly_instruction_interpreter;
};

std::shared_ptr<torricelly_interpreter> make_torricelly_interpreter(std::shared_ptr<gasp::torricelly::torricelly_module> main_module, std::function<std::string(unsigned int)> get_parameter);

} // namespace gasp::torricelly::interpreter