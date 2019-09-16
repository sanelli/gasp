#include <memory>

#include <sanelli/sanelli.hpp>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/interpreter.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::interpreter;

torricelly_instruction_interpreter::torricelly_instruction_interpreter(std::shared_ptr<torricelly_interpreter> interpreter)
    : _interpreter(interpreter) {}

torricelly_activation_record_variable torricelly_instruction_interpreter::pop_and_validate(std::shared_ptr<torricelly_activation_record> activation_record, torricelly_activation_record_variable_type expected_type)
{
   auto value = activation_record->pop();
   if (value.type() != expected_type)
      throw torricelly_interpreter_execution_error(activation_record->subroutine()->name(), activation_record->ip(),
                                                   sanelli::make_string("Cannot pop a '", to_string(expected_type), "'. Top of stack is '", to_string(value.type()), "'."));
   return value;
}

void torricelly_instruction_interpreter::execute(const torricelly::torricelly_instruction &instruction, unsigned int &next_instruction, bool &is_jump)
{
   // By default we jump to the next instruction
   next_instruction = 0;
   is_jump = false;

   auto code = instruction.code();
   switch (code)
   {
   case torricelly_inst_code::NOOP:
      execute_noop(instruction);
      break;
   case torricelly_inst_code::DUP:
      execute_dup(instruction);
      break;
   case torricelly_inst_code::POP_INTEGER:
      execute_pop_integer(instruction);
      break;
   case torricelly_inst_code::POP_BOOLEAN:
      execute_pop_boolean(instruction);
      break;
   case torricelly_inst_code::POP_CHAR:
      execute_pop_char(instruction);
      break;
   case torricelly_inst_code::POP_FLOAT:
      execute_pop_float(instruction);
      break;
   case torricelly_inst_code::POP_DOUBLE:
      execute_pop_double(instruction);
      break;
   case torricelly_inst_code::LOAD_INTEGER:
      execute_load_integer(instruction);
      break;
   case torricelly_inst_code::LOAD_BOOLEAN:
      execute_load_boolean(instruction);
      break;
   case torricelly_inst_code::LOAD_CHAR:
      execute_load_char(instruction);
      break;
   case torricelly_inst_code::LOAD_FLOAT:
      execute_load_float(instruction);
      break;
   case torricelly_inst_code::LOAD_DOUBLE:
      execute_load_double(instruction);
      break;
   case torricelly_inst_code::STORE_INTEGER:
      execute_store_integer(instruction);
      break;
   case torricelly_inst_code::STORE_BOOLEAN:
      execute_store_boolean(instruction);
      break;
   case torricelly_inst_code::STORE_CHAR:
      execute_store_char(instruction);
      break;
   case torricelly_inst_code::STORE_FLOAT:
      execute_store_float(instruction);
      break;
   case torricelly_inst_code::STORE_DOUBLE:
      execute_store_double(instruction);
      break;
   case torricelly_inst_code::ADD_INTEGER:
      execute_add_integer(instruction);
      break;
   case torricelly_inst_code::ADD_FLOAT:
      execute_add_float(instruction);
      break;
   case torricelly_inst_code::ADD_DOUBLE:
      execute_add_double(instruction);
      break;
   case torricelly_inst_code::SUBTRACT_INTEGER:
      execute_sub_integer(instruction);
      break;
   case torricelly_inst_code::SUBTRACT_FLOAT:
      execute_sub_float(instruction);
      break;
   case torricelly_inst_code::SUBTRACT_DOUBLE:
      execute_sub_double(instruction);
      break;
   case torricelly_inst_code::MULTIPLY_INTEGER:
      execute_mul_integer(instruction);
      break;
   case torricelly_inst_code::MULTIPLY_FLOAT:
      execute_mul_float(instruction);
      break;
   case torricelly_inst_code::MULTIPLY_DOUBLE:
      execute_mul_double(instruction);
      break;
   case torricelly_inst_code::DIVIDE_INTEGER:
      execute_div_integer(instruction);
      break;
   case torricelly_inst_code::DIVIDE_FLOAT:
      execute_div_float(instruction);
      break;
   case torricelly_inst_code::DIVIDE_DOUBLE:
      execute_div_double(instruction);
      break;
   case torricelly_inst_code::REMINDER_INTEGER:
      execute_rem_integer(instruction);
      break;
   case torricelly_inst_code::NEGATE_INTEGER:
      execute_negate_integer(instruction);
      break;
   case torricelly_inst_code::NEGATE_FLOAT:
      execute_negate_float(instruction);
      break;
   case torricelly_inst_code::NEGATE_DOUBLE:
      execute_negate_double(instruction);
      break;
   case torricelly_inst_code::CMP_INTEGER:
      execute_cmp_integer(instruction);
      break;
   case torricelly_inst_code::CMP_BOOLEAN:
      execute_cmp_boolean(instruction);
      break;
   case torricelly_inst_code::CMP_CHAR:
      execute_cmp_char(instruction);
      break;
   case torricelly_inst_code::CMP_FLOAT:
      execute_cmp_float(instruction);
      break;
   case torricelly_inst_code::CMP_DOUBLE:
      execute_cmp_double(instruction);
      break;
   case torricelly_inst_code::JUMP:
      break;
   case torricelly_inst_code::JUMP_EQ_ZERO:
      break;
   case torricelly_inst_code::JUMP_LT_ZERO:
      break;
   case torricelly_inst_code::JUMP_LTE_ZERO:
      break;
   case torricelly_inst_code::JUMP_GT_ZERO:
      break;
   case torricelly_inst_code::JUMP_GTE_ZERO:
      break;
   case torricelly_inst_code::JUMP_NOT_ZERO:
      break;
   case torricelly_inst_code::CAST_INT_BOOLEAN:
      break;
   case torricelly_inst_code::CAST_INT_CHAR:
      break;
   case torricelly_inst_code::CAST_INT_FLOAT:
      break;
   case torricelly_inst_code::CAST_INT_DOUBLE:
      break;
   case torricelly_inst_code::CAST_BOOLEAN_INT:
      break;
   case torricelly_inst_code::CAST_CHAR_INT:
      break;
   case torricelly_inst_code::CAST_FLOAT_INT:
      break;
   case torricelly_inst_code::CAST_FLOAT_DOUBLE:
      break;
   case torricelly_inst_code::CAST_DOUBLE_INT:
      break;
   case torricelly_inst_code::CAST_DOUBLE_FLOAT:
      break;
   case torricelly_inst_code::NOT:
      execute_not(instruction);
      break;
   case torricelly_inst_code::AND:
      execute_and(instruction);
      break;
   case torricelly_inst_code::OR:
      execute_or(instruction);
      break;
   case torricelly_inst_code::STATIC_INVOKE:
      break;
   case torricelly_inst_code::NATIVE_INVOKE:
      break;
   case torricelly_inst_code::VIRTUAL_INVOKE:
      break;
   case torricelly_inst_code::DYNAMIC_INVOKE:
      break;
   case torricelly_inst_code::RET:
      break;
   case torricelly_inst_code::HALT:
      break;
   default:
      throw torricelly_interpreter_error(sanelli::make_string("Cannot interpret instrution code '", to_string(code), "'."));
   }
}
