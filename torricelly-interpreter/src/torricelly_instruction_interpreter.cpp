#include <memory>
#include <sanelli/sanelli.hpp>
#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/interpreter.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::interpreter;

torricelly_instruction_interpreter::torricelly_instruction_interpreter(std::shared_ptr<torricelly_interpreter> interpreter)
    : _interpreter(interpreter) {}

void torricelly_instruction_interpreter::execute(const torricelly::torricelly_instruction &instruction, unsigned int &next_instruction, bool& is_jump)
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
      break;
   case torricelly_inst_code::LOAD_BOOLEAN:
      break;
   case torricelly_inst_code::LOAD_CHAR:
      break;
   case torricelly_inst_code::LOAD_FLOAT:
      break;
   case torricelly_inst_code::LOAD_DOUBLE:
      break;
   case torricelly_inst_code::STORE_INTEGER:
      break;
   case torricelly_inst_code::STORE_BOOLEAN:
      break;
   case torricelly_inst_code::STORE_CHAR:
      break;
   case torricelly_inst_code::STORE_FLOAT:
      break;
   case torricelly_inst_code::STORE_DOUBLE:
      break;
   case torricelly_inst_code::ADD_INTEGER:
      break;
   case torricelly_inst_code::ADD_FLOAT:
      break;
   case torricelly_inst_code::ADD_DOUBLE:
      break;
   case torricelly_inst_code::SUBTRACT_INTEGER:
      break;
   case torricelly_inst_code::SUBTRACT_FLOAT:
      break;
   case torricelly_inst_code::SUBTRACT_DOUBLE:
      break;
   case torricelly_inst_code::MULTIPLY_INTEGER:
      break;
   case torricelly_inst_code::MULTIPLY_FLOAT:
      break;
   case torricelly_inst_code::MULTIPLY_DOUBLE:
      break;
   case torricelly_inst_code::DIVIDE_INTEGER:
      break;
   case torricelly_inst_code::DIVIDE_FLOAT:
      break;
   case torricelly_inst_code::DIVIDE_DOUBLE:
      break;
   case torricelly_inst_code::REMINDER_INTEGER:
      break;
   case torricelly_inst_code::NEGATE_INTEGER:
      break;
   case torricelly_inst_code::NEGATE_FLOAT:
      break;
   case torricelly_inst_code::NEGATE_DOUBLE:
      break;
   case torricelly_inst_code::CMP_INTEGER:
      break;
   case torricelly_inst_code::CMP_BOOLEAN:
      break;
   case torricelly_inst_code::CMP_CHAR:
      break;
   case torricelly_inst_code::CMP_FLOAT:
      break;
   case torricelly_inst_code::CMP_DOUBLE:
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
      break;
   case torricelly_inst_code::AND:
      break;
   case torricelly_inst_code::OR:
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
