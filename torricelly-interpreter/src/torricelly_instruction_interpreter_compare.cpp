#include <memory>

#include <sanelli/sanelli.hpp>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/interpreter.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::interpreter;

void torricelly_instruction_interpreter::execute_cmp_integer(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto right_param_value = pop_and_validate(activation_record, torricelly_activation_record_variable_type::INTEGER);
   auto left_param_value = pop_and_validate(activation_record, torricelly_activation_record_variable_type::INTEGER);
   auto integer_result = sanelli::sign(left_param_value.get_integer() - left_param_value.get_integer());
   auto result = torricelly_activation_record_variable::make(integer_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cmp_float(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto right_param_value = pop_and_validate(activation_record, torricelly_activation_record_variable_type::FLOAT);
   auto left_param_value = pop_and_validate(activation_record, torricelly_activation_record_variable_type::FLOAT);
   auto integer_result = sanelli::sign(left_param_value.get_float() - left_param_value.get_float());
   auto result = torricelly_activation_record_variable::make(integer_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cmp_double(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto right_param_value = pop_and_validate(activation_record, torricelly_activation_record_variable_type::DOUBLE);
   auto left_param_value = pop_and_validate(activation_record, torricelly_activation_record_variable_type::DOUBLE);
   auto integer_result = sanelli::sign(left_param_value.get_double() - left_param_value.get_double());
   auto result = torricelly_activation_record_variable::make(integer_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cmp_boolean(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto right_param_value = pop_and_validate(activation_record, torricelly_activation_record_variable_type::BOOLEAN);
   auto left_param_value = pop_and_validate(activation_record, torricelly_activation_record_variable_type::BOOLEAN);
   auto integer_result = sanelli::sign(left_param_value.get_boolean() - left_param_value.get_boolean());
   auto result = torricelly_activation_record_variable::make(integer_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cmp_char(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto right_param_value = pop_and_validate(activation_record, torricelly_activation_record_variable_type::CHAR);
   auto left_param_value = pop_and_validate(activation_record, torricelly_activation_record_variable_type::CHAR);
   auto integer_result = sanelli::sign(left_param_value.get_char() - left_param_value.get_char());
   auto result = torricelly_activation_record_variable::make(integer_result);
   activation_record->push(result);
}