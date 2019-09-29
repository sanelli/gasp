#include <memory>

#include <functional>

#include <sanelli/sanelli.hpp>
#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/interpreter.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::interpreter;

void inline torricelly_instruction_interpreter::__execute_binary_integer(const torricelly::torricelly_instruction &instruction, std::function<int(int, int)> op)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto right_param_value = pop_and_validate(activation_record, torricelly_activation_record_variable_type::INTEGER);
   auto left_param_value = pop_and_validate(activation_record, torricelly_activation_record_variable_type::INTEGER);
   auto integer_result = op(left_param_value.get_integer(), right_param_value.get_integer());
   auto result = torricelly_activation_record_variable::make(integer_result);
   activation_record->push(result);
}

void inline torricelly_instruction_interpreter::__execute_binary_float(const torricelly::torricelly_instruction &instruction, std::function<float(float, float)> op)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto right_param_value = pop_and_validate(activation_record, torricelly_activation_record_variable_type::FLOAT);
   auto left_param_value = pop_and_validate(activation_record, torricelly_activation_record_variable_type::FLOAT);
   auto float_result = op(left_param_value.get_float(), right_param_value.get_float());
   auto result = torricelly_activation_record_variable::make(float_result);
   activation_record->push(result);
}

void inline torricelly_instruction_interpreter::__execute_binary_double(const torricelly::torricelly_instruction &instruction, std::function<double(double, double)> op)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto right_param_value = pop_and_validate(activation_record, torricelly_activation_record_variable_type::DOUBLE);
   auto left_param_value = pop_and_validate(activation_record, torricelly_activation_record_variable_type::DOUBLE);
   auto double_result = op(left_param_value.get_double(), right_param_value.get_double());
   auto result = torricelly_activation_record_variable::make(double_result);
   activation_record->push(result);
}

void torricelly_instruction_interpreter::execute_add_integer(const torricelly::torricelly_instruction &instruction)
{
   __execute_binary_integer(instruction, [](int a, int b) { return a + b; });
}

void torricelly_instruction_interpreter::execute_sub_integer(const torricelly::torricelly_instruction &instruction)
{
   __execute_binary_integer(instruction, [](int a, int b) { return a - b; });
}

void torricelly_instruction_interpreter::execute_mul_integer(const torricelly::torricelly_instruction &instruction)
{
   __execute_binary_integer(instruction, [](int a, int b) { return a * b; });
}

void torricelly_instruction_interpreter::execute_div_integer(const torricelly::torricelly_instruction &instruction)
{
   __execute_binary_integer(instruction, [](int a, int b) { return a / b; });
}

void torricelly_instruction_interpreter::execute_rem_integer(const torricelly::torricelly_instruction &instruction)
{
   __execute_binary_integer(instruction, [](int a, int b) { return a % b; });
}

void torricelly_instruction_interpreter::execute_negate_integer(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_variable_type::INTEGER);
   auto integer_result = -value.get_integer();
   auto result = torricelly_activation_record_variable::make(integer_result);
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
   auto value = pop_and_validate(activation_record, torricelly_activation_record_variable_type::FLOAT);
   auto float_result = -value.get_float();
   auto result = torricelly_activation_record_variable::make(float_result);
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
   auto value = pop_and_validate(activation_record, torricelly_activation_record_variable_type::DOUBLE);
   auto double_result = -value.get_float();
   auto result = torricelly_activation_record_variable::make(double_result);
   activation_record->push(result);
}