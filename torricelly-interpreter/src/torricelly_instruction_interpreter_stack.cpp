#include <memory>
#include <sanelli/sanelli.hpp>
#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/interpreter.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::interpreter;

void torricelly_instruction_interpreter::execute_dup(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->_activation_records.top();
   auto value = activation_record->peek();
   activation_record->push(activation_record->peek());
}

void inline torricelly_instruction_interpreter::__execute_pop(const torricelly::torricelly_instruction &instruction, torricelly_activation_record_variable_type extpected_type)
{
   auto activation_record = _interpreter.lock()->_activation_records.top();
   if (_validate_during_executions)
   {
      auto top_value = activation_record->peek();
      if (top_value.type() != extpected_type)
         throw torricelly_interpreter_execution_error(activation_record->subroutine()->name(), activation_record->ip(),
                                                      sanelli::make_string("Cannot pop a '", to_string(extpected_type),"'. Top of stack is '", to_string(top_value.type()), "'."));
   }
   activation_record->pop();
}

void torricelly_instruction_interpreter::execute_pop_boolean(const torricelly::torricelly_instruction &instruction)
{
   __execute_pop(instruction, torricelly_activation_record_variable_type::BOOLEAN);
}

void torricelly_instruction_interpreter::execute_pop_integer(const torricelly::torricelly_instruction &instruction)
{
   __execute_pop(instruction, torricelly_activation_record_variable_type::INTEGER);
}

void torricelly_instruction_interpreter::execute_pop_float(const torricelly::torricelly_instruction &instruction)
{
   __execute_pop(instruction, torricelly_activation_record_variable_type::FLOAT);
}

void torricelly_instruction_interpreter::execute_pop_double(const torricelly::torricelly_instruction &instruction)
{
   __execute_pop(instruction, torricelly_activation_record_variable_type::DOUBLE);
}

void torricelly_instruction_interpreter::execute_pop_char(const torricelly::torricelly_instruction &instruction)
{
   __execute_pop(instruction, torricelly_activation_record_variable_type::CHAR);
}
