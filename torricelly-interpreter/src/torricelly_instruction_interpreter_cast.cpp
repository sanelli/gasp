#include <memory>

#include <sanelli/sanelli.hpp>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/interpreter.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::interpreter;

void torricelly_instruction_interpreter::execute_cast_i2b(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::INTEGER);
   auto converted_result = (bool)value.get_integer();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cast_i2c(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::INTEGER);
   auto converted_result = (char)value.get_integer();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cast_i2f(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::INTEGER);
   auto converted_result = (float)value.get_integer();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cast_i2d(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::INTEGER);
   auto converted_result = (double)value.get_integer();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cast_b2i(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::BOOLEAN);
   auto converted_result = (int)value.get_boolean();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cast_c2i(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::CHAR);
   auto converted_result = (int)value.get_char();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cast_f2i(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::FLOAT);
   auto converted_result = (int)value.get_float();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cast_f2d(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::FLOAT);
   auto converted_result = (double)value.get_float();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cast_d2i(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::DOUBLE);
   auto converted_result = (int)value.get_double();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}
void torricelly_instruction_interpreter::execute_cast_d2f(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::DOUBLE);
   auto converted_result = (float)value.get_double();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}