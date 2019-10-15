#include <memory>

#include <functional>

#include <sanelli/sanelli.hpp>
#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/interpreter.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::interpreter;

bool torricelly_instruction_interpreter::__execute_jump(const torricelly::torricelly_instruction &instruction, unsigned int &next_address, std::function<bool(int)> condition)
{
   next_address = 0U;
   auto activation_record = _interpreter.lock()->activation_record();
   auto value = pop_and_validate(activation_record, torricelly_activation_record_variable_type::INTEGER);
   if (condition(value.get_integer()))
   {
      auto label = get_paramter_and_validate(activation_record, instruction, torricelly_inst_ref_type::LABEL);
      next_address = activation_record->get_instruction_address(label);
      return true;
   }
   return false;
}
bool torricelly_instruction_interpreter::execute_jump(const torricelly::torricelly_instruction &instruction, unsigned int &next_address)
{
   next_address = 0U;
   auto activation_record = _interpreter.lock()->activation_record();
   auto label = get_paramter_and_validate(activation_record, instruction, torricelly_inst_ref_type::LABEL);
   next_address = activation_record->get_instruction_address(label);
   return true;
}
bool torricelly_instruction_interpreter::execute_jump_lt(const torricelly::torricelly_instruction &instruction, unsigned int &next_address)
{
   return __execute_jump(instruction, next_address, [](int value) { return value < 0; });
}
bool torricelly_instruction_interpreter::execute_jump_lte(const torricelly::torricelly_instruction &instruction, unsigned int &next_address)
{
   return __execute_jump(instruction, next_address, [](int value) { return value <= 0; });
}
bool torricelly_instruction_interpreter::execute_jump_eq(const torricelly::torricelly_instruction &instruction, unsigned int &next_address)
{
   return __execute_jump(instruction, next_address, [](int value) { return value == 0; });
}
bool torricelly_instruction_interpreter::execute_jump_neq(const torricelly::torricelly_instruction &instruction, unsigned int &next_address)
{
   return __execute_jump(instruction, next_address, [](int value) { return value != 0; });
}
bool torricelly_instruction_interpreter::execute_jump_gte(const torricelly::torricelly_instruction &instruction, unsigned int &next_address)
{
   return __execute_jump(instruction, next_address, [](int value) { return value >= 0; });
}
bool torricelly_instruction_interpreter::execute_jump_gt(const torricelly::torricelly_instruction &instruction, unsigned int &next_address)
{
   return __execute_jump(instruction, next_address, [](int value) { return value > 0; });
}