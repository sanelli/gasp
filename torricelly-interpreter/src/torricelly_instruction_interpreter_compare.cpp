#include <memory>

#include <sanelli/sanelli.hpp>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/interpreter.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::interpreter;

void torricelly_instruction_interpreter::execute_cmp_byte(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto right_param_value = pop_and_validate(activation_record, torricelly_activation_record_local_type::BYTE);
   auto left_param_value = pop_and_validate(activation_record, torricelly_activation_record_local_type::BYTE);
   auto integer_result = sanelli::sign(left_param_value.get_byte() - right_param_value.get_byte());
   auto result = torricelly_activation_record_local::make(integer_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cmp_short(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto right_param_value = pop_and_validate(activation_record, torricelly_activation_record_local_type::SHORT);
   auto left_param_value = pop_and_validate(activation_record, torricelly_activation_record_local_type::SHORT);
   auto integer_result = sanelli::sign(left_param_value.get_short() - right_param_value.get_short());
   auto result = torricelly_activation_record_local::make(integer_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cmp_integer(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto right_param_value = pop_and_validate(activation_record, torricelly_activation_record_local_type::INTEGER);
   auto left_param_value = pop_and_validate(activation_record, torricelly_activation_record_local_type::INTEGER);
   auto integer_result = sanelli::sign(left_param_value.get_integer() - right_param_value.get_integer());
   auto result = torricelly_activation_record_local::make(integer_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cmp_long(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto right_param_value = pop_and_validate(activation_record, torricelly_activation_record_local_type::LONG);
   auto left_param_value = pop_and_validate(activation_record, torricelly_activation_record_local_type::LONG);
   auto integer_result = sanelli::sign(left_param_value.get_long() - right_param_value.get_long());
   auto result = torricelly_activation_record_local::make(integer_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cmp_float(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto right_param_value = pop_and_validate(activation_record, torricelly_activation_record_local_type::FLOAT);
   auto left_param_value = pop_and_validate(activation_record, torricelly_activation_record_local_type::FLOAT);
   auto integer_result = sanelli::sign(left_param_value.get_float() - right_param_value.get_float());
   auto result = torricelly_activation_record_local::make(integer_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cmp_double(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto right_param_value = pop_and_validate(activation_record, torricelly_activation_record_local_type::DOUBLE);
   auto left_param_value = pop_and_validate(activation_record, torricelly_activation_record_local_type::DOUBLE);
   auto integer_result = sanelli::sign(left_param_value.get_double() - right_param_value.get_double());
   auto result = torricelly_activation_record_local::make(integer_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cmp_boolean(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto right_param_value = pop_and_validate(activation_record, torricelly_activation_record_local_type::BOOLEAN);
   auto left_param_value = pop_and_validate(activation_record, torricelly_activation_record_local_type::BOOLEAN);
   auto integer_result = sanelli::sign(left_param_value.get_boolean() - right_param_value.get_boolean());
   auto result = torricelly_activation_record_local::make(integer_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cmp_char(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto right_param_value = pop_and_validate(activation_record, torricelly_activation_record_local_type::CHAR);
   auto left_param_value = pop_and_validate(activation_record, torricelly_activation_record_local_type::CHAR);
   auto integer_result = sanelli::sign(left_param_value.get_char() - right_param_value.get_char());
   auto result = torricelly_activation_record_local::make(integer_result);
   activation_record->push(result);
}