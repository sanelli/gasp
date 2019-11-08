#include <memory>

#include <sanelli/sanelli.hpp>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/interpreter.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::interpreter;

void torricelly_instruction_interpreter::execute_cast_y2s(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::BYTE);
   auto converted_result = (int16_t)value.get_byte();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cast_y2i(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::BYTE);
   auto converted_result = (int32_t)value.get_byte();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cast_y2l(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::BYTE);
   auto converted_result = (int64_t)value.get_byte();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cast_y2b(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::BYTE);
   auto converted_result = (bool)value.get_byte();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cast_y2c(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::BYTE);
   auto converted_result = (unsigned char)value.get_byte();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cast_y2f(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::BYTE);
   auto converted_result = (float)value.get_byte();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cast_y2d(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::BYTE);
   auto converted_result = (double)value.get_byte();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cast_s2y(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::SHORT);
   auto converted_result = (int8_t)value.get_short();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cast_s2i(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::SHORT);
   auto converted_result = (int32_t)value.get_short();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cast_s2l(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::SHORT);
   auto converted_result = (int64_t)value.get_short();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cast_s2b(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::SHORT);
   auto converted_result = (bool)value.get_short();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cast_s2c(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::SHORT);
   auto converted_result = (unsigned char)value.get_short();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cast_s2f(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::SHORT);
   auto converted_result = (float)value.get_short();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cast_s2d(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::SHORT);
   auto converted_result = (double)value.get_short();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cast_i2y(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::INTEGER);
   auto converted_result = (int8_t)value.get_integer();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cast_i2s(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::INTEGER);
   auto converted_result = (int16_t)value.get_integer();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cast_i2l(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::INTEGER);
   auto converted_result = (int64_t)value.get_integer();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

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
   auto converted_result = (unsigned char)value.get_integer();
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








void torricelly_instruction_interpreter::execute_cast_l2y(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::LONG);
   auto converted_result = (int8_t)value.get_long();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cast_l2s(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::LONG);
   auto converted_result = (int16_t)value.get_long();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cast_l2i(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::LONG);
   auto converted_result = (int32_t)value.get_long();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cast_l2f(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::LONG);
   auto converted_result = (float)value.get_long();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cast_l2d(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::LONG);
   auto converted_result = (double)value.get_long();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cast_b2y(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::BOOLEAN);
   auto converted_result = (int8_t)value.get_boolean();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cast_b2s(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::BOOLEAN);
   auto converted_result = (int16_t)value.get_boolean();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}
void torricelly_instruction_interpreter::execute_cast_b2i(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::BOOLEAN);
   auto converted_result = (int32_t)value.get_boolean();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cast_c2y(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::CHAR);
   auto converted_result = (int8_t)value.get_char();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cast_c2s(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::CHAR);
   auto converted_result = (int16_t)value.get_char();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cast_c2i(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::CHAR);
   auto converted_result = (int32_t)value.get_char();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cast_f2y(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::FLOAT);
   auto converted_result = (int8_t)value.get_float();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cast_f2s(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::FLOAT);
   auto converted_result = (int16_t)value.get_float();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}
void torricelly_instruction_interpreter::execute_cast_f2i(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::FLOAT);
   auto converted_result = (int32_t)value.get_float();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_cast_f2l(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::FLOAT);
   auto converted_result = (int64_t)value.get_float();
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

void torricelly_instruction_interpreter::execute_cast_d2y(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::DOUBLE);
   auto converted_result = (int8_t)value.get_double();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}
void torricelly_instruction_interpreter::execute_cast_d2s(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::DOUBLE);
   auto converted_result = (int16_t)value.get_double();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}
void torricelly_instruction_interpreter::execute_cast_d2i(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::DOUBLE);
   auto converted_result = (int32_t)value.get_double();
   auto result = torricelly_activation_record_local::make(converted_result);
   activation_record->push(result);
}
void torricelly_instruction_interpreter::execute_cast_d2l(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::DOUBLE);
   auto converted_result = (int64_t)value.get_double();
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