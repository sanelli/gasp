#include <memory>

#include <sanelli/sanelli.hpp>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/interpreter.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::interpreter;

torricelly_instruction_interpreter::torricelly_instruction_interpreter(std::shared_ptr<torricelly_interpreter> interpreter)
    : _interpreter(interpreter) {}

torricelly_activation_record_local torricelly_instruction_interpreter::pop_and_validate(std::shared_ptr<torricelly_activation_record> activation_record, torricelly_activation_record_local_type expected_type)
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
                                                      sanelli::make_string("Instruction does not contain a valid local reference."));
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
   case torricelly_inst_code::POP_BYTE:
      execute_pop_byte(instruction);
      break;
   case torricelly_inst_code::POP_SHORT:
      execute_pop_short(instruction);
      break;
   case torricelly_inst_code::POP_INTEGER:
      execute_pop_integer(instruction);
      break;
   case torricelly_inst_code::POP_LONG:
      execute_pop_long(instruction);
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
   case torricelly_inst_code::LOAD_BYTE:
      execute_load_byte(instruction);
      break;
   case torricelly_inst_code::LOAD_SHORT:
      execute_load_short(instruction);
      break;
   case torricelly_inst_code::LOAD_INTEGER:
      execute_load_integer(instruction);
      break;
   case torricelly_inst_code::LOAD_LONG:
      execute_load_long(instruction);
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
   case torricelly_inst_code::LOAD_ARRAY:
      execute_load_array(instruction);
      break;
   case torricelly_inst_code::LOAD_ARRAY_BYTE:
      execute_load_array_byte(instruction);
      break;
   case torricelly_inst_code::LOAD_ARRAY_SHORT:
      execute_load_array_short(instruction);
      break;
   case torricelly_inst_code::LOAD_ARRAY_INTEGER:
      execute_load_array_integer(instruction);
      break;
   case torricelly_inst_code::LOAD_ARRAY_LONG:
      execute_load_array_long(instruction);
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
   case torricelly_inst_code::STORE_BYTE:
      execute_store_byte(instruction);
      break;
   case torricelly_inst_code::STORE_SHORT:
      execute_store_short(instruction);
      break;
   case torricelly_inst_code::STORE_INTEGER:
      execute_store_integer(instruction);
      break;
   case torricelly_inst_code::STORE_LONG:
      execute_store_long(instruction);
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
   case torricelly_inst_code::STORE_ARRAY:
      execute_store_array(instruction);
      break;
   case torricelly_inst_code::STORE_ARRAY_BYTE:
      execute_store_array_byte(instruction);
      break;
   case torricelly_inst_code::STORE_ARRAY_SHORT:
      execute_store_array_short(instruction);
      break;
   case torricelly_inst_code::STORE_ARRAY_INTEGER:
      execute_store_array_integer(instruction);
      break;
   case torricelly_inst_code::STORE_ARRAY_LONG:
      execute_store_array_long(instruction);
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
   case torricelly_inst_code::ADD_BYTE:
      execute_add_byte(instruction);
      break;
   case torricelly_inst_code::ADD_SHORT:
      execute_add_short(instruction);
      break;
   case torricelly_inst_code::ADD_INTEGER:
      execute_add_integer(instruction);
      break;
   case torricelly_inst_code::ADD_LONG:
      execute_add_long(instruction);
      break;
   case torricelly_inst_code::ADD_FLOAT:
      execute_add_float(instruction);
      break;
   case torricelly_inst_code::ADD_DOUBLE:
      execute_add_double(instruction);
      break;
   case torricelly_inst_code::SUBTRACT_BYTE:
      execute_sub_byte(instruction);
      break;
   case torricelly_inst_code::SUBTRACT_SHORT:
      execute_sub_short(instruction);
      break;
   case torricelly_inst_code::SUBTRACT_INTEGER:
      execute_sub_integer(instruction);
      break;
   case torricelly_inst_code::SUBTRACT_LONG:
      execute_sub_long(instruction);
      break;
   case torricelly_inst_code::SUBTRACT_FLOAT:
      execute_sub_float(instruction);
      break;
   case torricelly_inst_code::SUBTRACT_DOUBLE:
      execute_sub_double(instruction);
      break;
   case torricelly_inst_code::MULTIPLY_BYTE:
      execute_mul_byte(instruction);
      break;
   case torricelly_inst_code::MULTIPLY_SHORT:
      execute_mul_short(instruction);
      break;
   case torricelly_inst_code::MULTIPLY_INTEGER:
      execute_mul_integer(instruction);
      break;
   case torricelly_inst_code::MULTIPLY_LONG:
      execute_mul_long(instruction);
      break;
   case torricelly_inst_code::MULTIPLY_FLOAT:
      execute_mul_float(instruction);
      break;
   case torricelly_inst_code::MULTIPLY_DOUBLE:
      execute_mul_double(instruction);
      break;
   case torricelly_inst_code::DIVIDE_BYTE:
      execute_div_byte(instruction);
      break;
   case torricelly_inst_code::DIVIDE_SHORT:
      execute_div_short(instruction);
      break;
   case torricelly_inst_code::DIVIDE_INTEGER:
      execute_div_integer(instruction);
      break;
   case torricelly_inst_code::DIVIDE_LONG:
      execute_div_long(instruction);
      break;
   case torricelly_inst_code::DIVIDE_FLOAT:
      execute_div_float(instruction);
      break;
   case torricelly_inst_code::DIVIDE_DOUBLE:
      execute_div_double(instruction);
      break;
   case torricelly_inst_code::REMINDER_BYTE:
      execute_rem_byte(instruction);
      break;
   case torricelly_inst_code::REMINDER_SHORT:
      execute_rem_short(instruction);
      break;
   case torricelly_inst_code::REMINDER_INTEGER:
      execute_rem_integer(instruction);
      break;
   case torricelly_inst_code::REMINDER_LONG:
      execute_rem_long(instruction);
      break;
   case torricelly_inst_code::NEGATE_BYTE:
      execute_negate_byte(instruction);
      break;
   case torricelly_inst_code::NEGATE_SHORT:
      execute_negate_short(instruction);
      break;
   case torricelly_inst_code::NEGATE_INTEGER:
      execute_negate_integer(instruction);
      break;
   case torricelly_inst_code::NEGATE_LONG:
      execute_negate_long(instruction);
      break;
   case torricelly_inst_code::NEGATE_FLOAT:
      execute_negate_float(instruction);
      break;
   case torricelly_inst_code::NEGATE_DOUBLE:
      execute_negate_double(instruction);
      break;
   case torricelly_inst_code::CMP_BYTE:
      execute_cmp_byte(instruction);
      break;
   case torricelly_inst_code::CMP_SHORT:
      execute_cmp_short(instruction);
      break;
   case torricelly_inst_code::CMP_INTEGER:
      execute_cmp_integer(instruction);
      break;
   case torricelly_inst_code::CMP_LONG:
      execute_cmp_long(instruction);
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
   case torricelly_inst_code::CAST_BYTE_SHORT:
      execute_cast_y2s(instruction);
      break;
   case torricelly_inst_code::CAST_BYTE_INT:
      execute_cast_y2i(instruction);
      break;
   case torricelly_inst_code::CAST_BYTE_LONG:
      execute_cast_y2l(instruction);
      break;
   case torricelly_inst_code::CAST_BYTE_BOOLEAN:
      execute_cast_y2b(instruction);
      break;
   case torricelly_inst_code::CAST_BYTE_CHAR:
      execute_cast_y2c(instruction);
      break;
   case torricelly_inst_code::CAST_BYTE_FLOAT:
      execute_cast_y2f(instruction);
      break;
   case torricelly_inst_code::CAST_BYTE_DOUBLE:
      execute_cast_y2d(instruction);
      break;
   case torricelly_inst_code::CAST_SHORT_BYTE:
      execute_cast_s2y(instruction);
      break;
   case torricelly_inst_code::CAST_SHORT_INT:
      execute_cast_s2i(instruction);
      break;
   case torricelly_inst_code::CAST_SHORT_LONG:
      execute_cast_s2l(instruction);
      break;
   case torricelly_inst_code::CAST_SHORT_BOOLEAN:
      execute_cast_s2b(instruction);
      break;
   case torricelly_inst_code::CAST_SHORT_CHAR:
      execute_cast_s2c(instruction);
      break;
   case torricelly_inst_code::CAST_SHORT_FLOAT:
      execute_cast_s2f(instruction);
      break;
   case torricelly_inst_code::CAST_SHORT_DOUBLE:
      execute_cast_s2d(instruction);
      break;
   case torricelly_inst_code::CAST_INT_BYTE:
      execute_cast_i2y(instruction);
      break;
   case torricelly_inst_code::CAST_INT_SHORT:
      execute_cast_i2s(instruction);
      break;
   case torricelly_inst_code::CAST_INT_LONG:
      execute_cast_i2l(instruction);
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
   case torricelly_inst_code::CAST_LONG_BYTE:
      execute_cast_l2y(instruction);
      break;
   case torricelly_inst_code::CAST_LONG_SHORT:
      execute_cast_l2s(instruction);
      break;
   case torricelly_inst_code::CAST_LONG_INT:
      execute_cast_l2i(instruction);
      break;
   case torricelly_inst_code::CAST_LONG_FLOAT:
      execute_cast_l2f(instruction);
      break;
   case torricelly_inst_code::CAST_LONG_DOUBLE:
      execute_cast_l2d(instruction);
      break;
   case torricelly_inst_code::CAST_BOOLEAN_BYTE:
      execute_cast_b2y(instruction);
      break;
   case torricelly_inst_code::CAST_BOOLEAN_SHORT:
      execute_cast_b2s(instruction);
      break;
   case torricelly_inst_code::CAST_BOOLEAN_INT:
      execute_cast_b2i(instruction);
      break;
   case torricelly_inst_code::CAST_CHAR_BYTE:
      execute_cast_c2y(instruction);
      break;
   case torricelly_inst_code::CAST_CHAR_SHORT:
      execute_cast_c2s(instruction);
      break;
   case torricelly_inst_code::CAST_CHAR_INT:
      execute_cast_c2i(instruction);
      break;
   case torricelly_inst_code::CAST_FLOAT_BYTE:
      execute_cast_f2y(instruction);
      break;
   case torricelly_inst_code::CAST_FLOAT_SHORT:
      execute_cast_f2s(instruction);
      break;
   case torricelly_inst_code::CAST_FLOAT_INT:
      execute_cast_f2i(instruction);
      break;
   case torricelly_inst_code::CAST_FLOAT_LONG:
      execute_cast_f2l(instruction);
      break;
   case torricelly_inst_code::CAST_FLOAT_DOUBLE:
      execute_cast_f2d(instruction);
      break;
   case torricelly_inst_code::CAST_DOUBLE_BYTE:
      execute_cast_d2y(instruction);
      break;
   case torricelly_inst_code::CAST_DOUBLE_SHORT:
      execute_cast_d2s(instruction);
      break;
   case torricelly_inst_code::CAST_DOUBLE_INT:
      execute_cast_d2i(instruction);
      break;
   case torricelly_inst_code::CAST_DOUBLE_LONG:
      execute_cast_d2l(instruction);
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
   case torricelly_inst_code::NATIVE_INVOKE:
      execute_native_invoke(instruction);
      break;
   case torricelly_inst_code::RET:
      return false;
      break;
   case torricelly_inst_code::HALT:
      _interpreter.lock()->_status = torricelly_interpreter_status::HALTED;
      break;
   case torricelly_inst_code::ALLOCATE_BYTE_ARRAY:
      execute_allocate_byte_array(instruction);
      break;
   case torricelly_inst_code::ALLOCATE_SHORT_ARRAY:
      execute_allocate_short_array(instruction);
      break;
   case torricelly_inst_code::ALLOCATE_INT_ARRAY:
      execute_allocate_integer_array(instruction);
      break;
   case torricelly_inst_code::ALLOCATE_LONG_ARRAY:
      execute_allocate_long_array(instruction);
      break;
   case torricelly_inst_code::ALLOCATE_CHAR_ARRAY:
      execute_allocate_char_array(instruction);
      break;
   case torricelly_inst_code::ALLOCATE_BOOLEAN_ARRAY:
      execute_allocate_boolean_array(instruction);
      break;
   case torricelly_inst_code::ALLOCATE_FLOAT_ARRAY:
      execute_allocate_float_array(instruction);
      break;
   case torricelly_inst_code::ALLOCATE_DOUBLE_ARRAY:
      execute_allocate_double_array(instruction);
      break;
   case torricelly_inst_code::FREE_ARRAY:
      execute_free_array(instruction);
      break;
   default:
      throw torricelly_interpreter_error(sanelli::make_string("Cannot interpret instrution code '", to_string(code), "'."));
   }

   return continue_in_subroutine;
}
