#include <memory>

#include <functional>

#include <sanelli/sanelli.hpp>
#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/interpreter.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::interpreter;

void inline torricelly_instruction_interpreter::__execute_binary_boolean(const torricelly::torricelly_instruction &instruction,
                                                                         std::function<bool(bool, bool)> op)
{
   auto activation_record = _interpreter->_activation_records.top();
   auto right_param_value = activation_record->pop();
   if (_validate_during_executions)
   {
      if (right_param_value.type() != torricelly_activation_record_variable_type::BOOLEAN)
         throw torricelly_interpreter_execution_error(activation_record->subroutine()->name(), activation_record->ip(),
                                                      sanelli::make_string("Cannot pop a '", to_string(torricelly_activation_record_variable_type::BOOLEAN), "'. Top of stack is '", to_string(right_param_value.type()), "'."));
   }

   auto left_param_value = activation_record->pop();
   if (_validate_during_executions)
   {
      if (left_param_value.type() != torricelly_activation_record_variable_type::BOOLEAN)
         throw torricelly_interpreter_execution_error(activation_record->subroutine()->name(), activation_record->ip(),
                                                      sanelli::make_string("Cannot pop a '", to_string(torricelly_activation_record_variable_type::BOOLEAN), "'. Top of stack is '", to_string(left_param_value.type()), "'."));
   }

   auto boolean_result = op(left_param_value.get_boolean(), left_param_value.get_boolean());
   auto result = torricelly_activation_record_variable::make(boolean_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_and(const torricelly::torricelly_instruction &instruction)
{
   __execute_binary_boolean(instruction, [](bool a, bool b) { return a && b; });
}

void torricelly_instruction_interpreter::execute_or(const torricelly::torricelly_instruction &instruction)
{
   __execute_binary_boolean(instruction, [](bool a, bool b) { return a || b; });
}

void torricelly_instruction_interpreter::execute_not(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter->_activation_records.top();
   auto param_value = activation_record->pop();
   if (_validate_during_executions)
   {
      if (param_value.type() != torricelly_activation_record_variable_type::BOOLEAN)
         throw torricelly_interpreter_execution_error(activation_record->subroutine()->name(), activation_record->ip(),
                                                      sanelli::make_string("Cannot pop a '", to_string(torricelly_activation_record_variable_type::BOOLEAN), "'. Top of stack is '", to_string(param_value.type()), "'."));
   }

   auto boolean_result = !param_value.get_boolean();
   auto result = torricelly_activation_record_variable::make(boolean_result);
   activation_record->push(result);
}