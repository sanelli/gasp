#include <memory>

#include <sanelli/sanelli.hpp>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/interpreter.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::interpreter;

torricelly_instruction_interpreter::torricelly_instruction_interpreter(std::shared_ptr<torricelly_interpreter> interpreter)
    : _interpreter(interpreter) {}

torricelly_activation_record_variable torricelly_instruction_interpreter::pop_and_validate(std::shared_ptr<torricelly_activation_record> activation_record, torricelly_activation_record_local_type expected_type)
{
   auto value = activation_record->pop();
   if (value.type() != expected_type)
      throw torricelly_interpreter_execution_error(activation_record->subroutine()->name(), activation_record->ip(),
                                                   sanelli::make_string("Cannot pop a '", to_string(expected_type), "'. Top of stack is '", to_string(value.type()), "'."));
   return value;
}

unsigned int torricelly_instruction_interpreter::get_paramter_and_validate(std::shared_ptr<torricelly_activation_record> activation_record,
                                                                           const torricelly::torricelly_instruction &instruction,
                                                                           const torricelly_inst_ref_type ref_type)
{
   auto value = instruction.parameter_reference();
   if (_validate_during_executions)
   {
      if (value <= 0U)
         throw torricelly_interpreter_execution_error(activation_record->subroutine()->name(), activation_record->ip(),
                                                      sanelli::make_string("Instruction does not contain a valid variable reference."));
      if (instruction.ref_type() != ref_type)
         throw torricelly_interpreter_execution_error(activation_record->subroutine()->name(), activation_record->ip(),
                                                      sanelli::make_string("Instruction does not refernce the correct parameter."));
   }
   return value;
}

bool torricelly_instruction_interpreter::execute(const torricelly::torricelly_instruction &instruction, unsigned int &next_instruction, bool &is_jump)
{
   // By default we jump to the next instruction
   next_instruction = 0;
   is_jump = false;
   bool continue_in_subroutine = true;

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
   case torricelly_inst_code::LOAD_ARRAY_INTEGER:
      execute_load_array_integer(instruction);
      break;
   case torricelly_inst_code::LOAD_ARRAY_BOOLEAN:
      execute_load_array_boolean(instruction);
      break;
   case torricelly_inst_code::LOAD_ARRAY_CHAR:
      execute_load_array_char(instruction);
      break;
   case torricelly_inst_code::LOAD_ARRAY_FLOAT:
      execute_load_array_float(instruction);
      break;
   case torricelly_inst_code::LOAD_ARRAY_DOUBLE:
      execute_load_array_double(instruction);
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
   case torricelly_inst_code::STORE_ARRAY_INTEGER:
      execute_store_array_integer(instruction);
      break;
   case torricelly_inst_code::STORE_ARRAY_BOOLEAN:
      execute_store_array_boolean(instruction);
      break;
   case torricelly_inst_code::STORE_ARRAY_CHAR:
      execute_store_array_char(instruction);
      break;
   case torricelly_inst_code::STORE_ARRAY_FLOAT:
      execute_store_array_float(instruction);
      break;
   case torricelly_inst_code::STORE_ARRAY_DOUBLE:
      execute_store_array_double(instruction);
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
      is_jump = execute_jump(instruction, next_instruction);
      break;
   case torricelly_inst_code::JUMP_EQ_ZERO:
      is_jump = execute_jump_eq(instruction, next_instruction);
      break;
   case torricelly_inst_code::JUMP_LT_ZERO:
      is_jump = execute_jump_lt(instruction, next_instruction);
      break;
   case torricelly_inst_code::JUMP_LTE_ZERO:
      is_jump = execute_jump_lte(instruction, next_instruction);
      break;
   case torricelly_inst_code::JUMP_GT_ZERO:
      is_jump = execute_jump_gt(instruction, next_instruction);
      break;
   case torricelly_inst_code::JUMP_GTE_ZERO:
      is_jump = execute_jump_gte(instruction, next_instruction);
      break;
   case torricelly_inst_code::JUMP_NOT_ZERO:
      is_jump = execute_jump_neq(instruction, next_instruction);
      break;
   case torricelly_inst_code::CAST_INT_BOOLEAN:
      execute_cast_i2b(instruction);
      break;
   case torricelly_inst_code::CAST_INT_CHAR:
      execute_cast_i2c(instruction);
      break;
   case torricelly_inst_code::CAST_INT_FLOAT:
      execute_cast_i2f(instruction);
      break;
   case torricelly_inst_code::CAST_INT_DOUBLE:
      execute_cast_i2d(instruction);
      break;
   case torricelly_inst_code::CAST_BOOLEAN_INT:
      execute_cast_b2i(instruction);
      break;
   case torricelly_inst_code::CAST_CHAR_INT:
      execute_cast_c2i(instruction);
      break;
   case torricelly_inst_code::CAST_FLOAT_INT:
      execute_cast_f2i(instruction);
      break;
   case torricelly_inst_code::CAST_FLOAT_DOUBLE:
      execute_cast_f2d(instruction);
      break;
   case torricelly_inst_code::CAST_DOUBLE_INT:
      execute_cast_d2i(instruction);
      break;
   case torricelly_inst_code::CAST_DOUBLE_FLOAT:
      execute_cast_d2f(instruction);
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
      execute_static_invoke(instruction);
      break;
   case torricelly_inst_code::RET:
      return false;
      break;
   case torricelly_inst_code::HALT:
      _interpreter.lock()->_status = torricelly_interpreter_status::HALTED;
      break;
   default:
      throw torricelly_interpreter_error(sanelli::make_string("Cannot interpret instrution code '", to_string(code), "'."));
   }

   return continue_in_subroutine;
}
