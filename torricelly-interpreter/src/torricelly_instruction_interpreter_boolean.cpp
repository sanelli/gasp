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
   auto activation_record = _interpreter.lock()->activation_record();
   auto right_param_value = pop_and_validate(activation_record, torricelly_activation_record_local_type::BOOLEAN);
   auto left_param_value = pop_and_validate(activation_record, torricelly_activation_record_local_type::BOOLEAN);
   auto boolean_result = op(left_param_value.get_boolean(), right_param_value.get_boolean());
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
   auto activation_record = _interpreter.lock()->activation_record();
   auto param_value = pop_and_validate(activation_record, torricelly_activation_record_local_type::BOOLEAN);
   auto boolean_result = !param_value.get_boolean();
   auto result = torricelly_activation_record_variable::make(boolean_result);
   activation_record->push(result);
}