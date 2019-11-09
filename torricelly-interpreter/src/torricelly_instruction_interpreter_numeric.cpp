#include <memory>

#include <functional>

#include <sanelli/sanelli.hpp>
#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/interpreter.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::interpreter;

void inline torricelly_instruction_interpreter::__execute_binary_byte(const torricelly::torricelly_instruction &instruction, std::function<int8_t(int8_t, int8_t)> op)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto right_param_value = pop_and_validate(activation_record, torricelly_activation_record_local_type::BYTE);
   auto left_param_value = pop_and_validate(activation_record, torricelly_activation_record_local_type::BYTE);
   auto integer_result = op(left_param_value.get_byte(), right_param_value.get_byte());
   auto result = torricelly_activation_record_local::make(integer_result);
   activation_record->push(result);
}
void inline torricelly_instruction_interpreter::__execute_binary_short(const torricelly::torricelly_instruction &instruction, std::function<int16_t(int16_t, int16_t)> op)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto right_param_value = pop_and_validate(activation_record, torricelly_activation_record_local_type::SHORT);
   auto left_param_value = pop_and_validate(activation_record, torricelly_activation_record_local_type::SHORT);
   auto integer_result = op(left_param_value.get_short(), right_param_value.get_short());
   auto result = torricelly_activation_record_local::make(integer_result);
   activation_record->push(result);
}
void inline torricelly_instruction_interpreter::__execute_binary_integer(const torricelly::torricelly_instruction &instruction, std::function<int32_t(int32_t, int32_t)> op)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto right_param_value = pop_and_validate(activation_record, torricelly_activation_record_local_type::INTEGER);
   auto left_param_value = pop_and_validate(activation_record, torricelly_activation_record_local_type::INTEGER);
   auto integer_result = op(left_param_value.get_integer(), right_param_value.get_integer());
   auto result = torricelly_activation_record_local::make(integer_result);
   activation_record->push(result);
}
void inline torricelly_instruction_interpreter::__execute_binary_long(const torricelly::torricelly_instruction &instruction, std::function<int64_t(int64_t, int64_t)> op)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto right_param_value = pop_and_validate(activation_record, torricelly_activation_record_local_type::LONG);
   auto left_param_value = pop_and_validate(activation_record, torricelly_activation_record_local_type::LONG);
   auto integer_result = op(left_param_value.get_long(), right_param_value.get_long());
   auto result = torricelly_activation_record_local::make(integer_result);
   activation_record->push(result);
}
void inline torricelly_instruction_interpreter::__execute_binary_float(const torricelly::torricelly_instruction &instruction, std::function<float(float, float)> op)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto right_param_value = pop_and_validate(activation_record, torricelly_activation_record_local_type::FLOAT);
   auto left_param_value = pop_and_validate(activation_record, torricelly_activation_record_local_type::FLOAT);
   auto float_result = op(left_param_value.get_float(), right_param_value.get_float());
   auto result = torricelly_activation_record_local::make(float_result);
   activation_record->push(result);
}

void inline torricelly_instruction_interpreter::__execute_binary_double(const torricelly::torricelly_instruction &instruction, std::function<double(double, double)> op)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto right_param_value = pop_and_validate(activation_record, torricelly_activation_record_local_type::DOUBLE);
   auto left_param_value = pop_and_validate(activation_record, torricelly_activation_record_local_type::DOUBLE);
   auto double_result = op(left_param_value.get_double(), right_param_value.get_double());
   auto result = torricelly_activation_record_local::make(double_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_add_byte(const torricelly::torricelly_instruction &instruction)
{
   __execute_binary_byte(instruction, [](int8_t a, int8_t b) { return a + b; });
}

void torricelly_instruction_interpreter::execute_sub_byte(const torricelly::torricelly_instruction &instruction)
{
   __execute_binary_byte(instruction, [](int8_t a, int8_t b) { return a - b; });
}

void torricelly_instruction_interpreter::execute_mul_byte(const torricelly::torricelly_instruction &instruction)
{
   __execute_binary_byte(instruction, [](int8_t a, int8_t b) { return a * b; });
}

void torricelly_instruction_interpreter::execute_div_byte(const torricelly::torricelly_instruction &instruction)
{
   __execute_binary_byte(instruction, [](int8_t a, int8_t b) { return a / b; });
}

void torricelly_instruction_interpreter::execute_rem_byte(const torricelly::torricelly_instruction &instruction)
{
   __execute_binary_byte(instruction, [](int8_t a, int8_t b) { return a % b; });
}

void torricelly_instruction_interpreter::execute_add_short(const torricelly::torricelly_instruction &instruction)
{
   __execute_binary_short(instruction, [](int16_t a, int16_t b) { return a + b; });
}

void torricelly_instruction_interpreter::execute_sub_short(const torricelly::torricelly_instruction &instruction)
{
   __execute_binary_short(instruction, [](int16_t a, int16_t b) { return a - b; });
}

void torricelly_instruction_interpreter::execute_mul_short(const torricelly::torricelly_instruction &instruction)
{
   __execute_binary_short(instruction, [](int16_t a, int16_t b) { return a * b; });
}

void torricelly_instruction_interpreter::execute_div_short(const torricelly::torricelly_instruction &instruction)
{
   __execute_binary_short(instruction, [](int16_t a, int16_t b) { return a / b; });
}

void torricelly_instruction_interpreter::execute_rem_short(const torricelly::torricelly_instruction &instruction)
{
   __execute_binary_short(instruction, [](int16_t a, int16_t b) { return a % b; });
}

void torricelly_instruction_interpreter::execute_add_integer(const torricelly::torricelly_instruction &instruction)
{
   __execute_binary_integer(instruction, [](int32_t a, int32_t b) { return a + b; });
}

void torricelly_instruction_interpreter::execute_sub_integer(const torricelly::torricelly_instruction &instruction)
{
   __execute_binary_integer(instruction, [](int32_t a, int32_t b) { return a - b; });
}

void torricelly_instruction_interpreter::execute_mul_integer(const torricelly::torricelly_instruction &instruction)
{
   __execute_binary_integer(instruction, [](int32_t a, int32_t b) { return a * b; });
}

void torricelly_instruction_interpreter::execute_div_integer(const torricelly::torricelly_instruction &instruction)
{
   __execute_binary_integer(instruction, [](int32_t a, int32_t b) { return a / b; });
}

void torricelly_instruction_interpreter::execute_rem_integer(const torricelly::torricelly_instruction &instruction)
{
   __execute_binary_integer(instruction, [](int64_t a, int64_t b) { return a % b; });
}

void torricelly_instruction_interpreter::execute_add_long(const torricelly::torricelly_instruction &instruction)
{
   __execute_binary_long(instruction, [](int64_t a, int64_t b) { return a + b; });
}

void torricelly_instruction_interpreter::execute_sub_long(const torricelly::torricelly_instruction &instruction)
{
   __execute_binary_long(instruction, [](int64_t a, int64_t b) { return a - b; });
}

void torricelly_instruction_interpreter::execute_mul_long(const torricelly::torricelly_instruction &instruction)
{
   __execute_binary_long(instruction, [](int64_t a, int64_t b) { return a * b; });
}

void torricelly_instruction_interpreter::execute_div_long(const torricelly::torricelly_instruction &instruction)
{
   __execute_binary_long(instruction, [](int64_t a, int64_t b) { return a / b; });
}

void torricelly_instruction_interpreter::execute_rem_long(const torricelly::torricelly_instruction &instruction)
{
   __execute_binary_long(instruction, [](int64_t a, int64_t b) { return a % b; });
}

void torricelly_instruction_interpreter::execute_negate_byte(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::BYTE);
   auto integer_result = (int8_t) -value.get_integer();
   auto result = torricelly_activation_record_local::make(integer_result);
   activation_record->push(result);
}
void torricelly_instruction_interpreter::execute_negate_short(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::SHORT);
   auto integer_result = (int16_t) -value.get_short();
   auto result = torricelly_activation_record_local::make(integer_result);
   activation_record->push(result);
}
void torricelly_instruction_interpreter::execute_negate_integer(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::INTEGER);
   auto integer_result = (int32_t) -value.get_integer();
   auto result = torricelly_activation_record_local::make(integer_result);
   activation_record->push(result);
}
void torricelly_instruction_interpreter::execute_negate_long(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::LONG);
   auto integer_result = (int64_t) -value.get_long();
   auto result = torricelly_activation_record_local::make(integer_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_add_float(const torricelly::torricelly_instruction &instruction)
{
   __execute_binary_float(instruction, [](float a, float b) { return a + b; });
}

void torricelly_instruction_interpreter::execute_sub_float(const torricelly::torricelly_instruction &instruction)
{
   __execute_binary_float(instruction, [](float a, float b) { return a - b; });
}

void torricelly_instruction_interpreter::execute_mul_float(const torricelly::torricelly_instruction &instruction)
{
   __execute_binary_float(instruction, [](float a, float b) { return a * b; });
}

void torricelly_instruction_interpreter::execute_div_float(const torricelly::torricelly_instruction &instruction)
{
   __execute_binary_float(instruction, [](float a, float b) { return a / b; });
}

void torricelly_instruction_interpreter::execute_negate_float(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::FLOAT);
   auto float_result = -value.get_float();
   auto result = torricelly_activation_record_local::make(float_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_add_double(const torricelly::torricelly_instruction &instruction)
{
   __execute_binary_double(instruction, [](double a, double b) { return a + b; });
}

void torricelly_instruction_interpreter::execute_sub_double(const torricelly::torricelly_instruction &instruction)
{
   __execute_binary_double(instruction, [](double a, double b) { return a - b; });
}

void torricelly_instruction_interpreter::execute_mul_double(const torricelly::torricelly_instruction &instruction)
{
   __execute_binary_double(instruction, [](double a, double b) { return a * b; });
}

void torricelly_instruction_interpreter::execute_div_double(const torricelly::torricelly_instruction &instruction)
{
   __execute_binary_double(instruction, [](double a, double b) { return a / b; });
}

void torricelly_instruction_interpreter::execute_negate_double(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_local_type::DOUBLE);
   auto double_result = -value.get_double();
   auto result = torricelly_activation_record_local::make(double_result);
   activation_record->push(result);
}