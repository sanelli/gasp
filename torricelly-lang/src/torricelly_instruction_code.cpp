#include <gasp/torricelly/torricelly.hpp>

using namespace gasp;
using namespace gasp::torricelly;

const char *torricelly::to_string(torricelly_inst_code code)
{
   switch (code)
   {  
   case torricelly_inst_code::NOOP: return "noop";
   case torricelly_inst_code::PUSH_INTEGER: return "ipush";
   case torricelly_inst_code::PUSH_BOOLEAN: return "bpush";
   case torricelly_inst_code::PUSH_CHAR: return "cpush";
   case torricelly_inst_code::PUSH_FLOAT: return "fpush";
   case torricelly_inst_code::PUSH_DOUBLE: return "dpush";
   case torricelly_inst_code::POP_INTEGER: return "ipop";
   case torricelly_inst_code::POP_BOOLEAN: return "bpop";
   case torricelly_inst_code::POP_CHAR: return "cpop";
   case torricelly_inst_code::POP_FLOAT: return "fpop";
   case torricelly_inst_code::POP_DOUBLE: return "dpop";
   case torricelly_inst_code::LOAD_INTEGER: return "iload";
   case torricelly_inst_code::LOAD_BOOLEAN: return "bload";
   case torricelly_inst_code::LOAD_CHAR: return "cload";
   case torricelly_inst_code::LOAD_FLOAT: return "float";
   case torricelly_inst_code::LOAD_DOUBLE: return "dload";
   case torricelly_inst_code::STORE_INTEGER: return "istore";
   case torricelly_inst_code::STORE_BOOLEAN: return "bstore";
   case torricelly_inst_code::STORE_CHAR: return "cstore";
   case torricelly_inst_code::STORE_FLOAT: return "fstore";
   case torricelly_inst_code::STORE_DOUBLE: return "dstore";
   case torricelly_inst_code::ADD_INTEGER: return "iadd";
   case torricelly_inst_code::ADD_FLOAT: return "fadd";
   case torricelly_inst_code::ADD_DOUBLE: return "dadd";
   case torricelly_inst_code::SUBTRACT_INTEGER: return "isub";
   case torricelly_inst_code::SUBTRACT_FLOAT: return "fsub";
   case torricelly_inst_code::SUBTRACT_DOUBLE: return "dsub";
   case torricelly_inst_code::MULTIPLY_INTEGER: return "imul";
   case torricelly_inst_code::MULTIPLY_FLOAT: return "fmul";
   case torricelly_inst_code::MULTIPLY_DOUBLE: return "dmul";
   case torricelly_inst_code::DIVIDE_INTEGER: return "idiv";
   case torricelly_inst_code::DIVIDE_FLOAT: return "fdiv";
   case torricelly_inst_code::DIVIDE_DOUBLE: return "ddiv";
   case torricelly_inst_code::REMINDER_INTEGER: return "irem";
   case torricelly_inst_code::NEGATE_INTEGER: return "ineg";
   case torricelly_inst_code::NEGATE_FLOAT: return "fneg";
   case torricelly_inst_code::NEGATE_DOUBLE: return "dneg";
   case torricelly_inst_code::CMP_INTEGER: return "icmp";
   case torricelly_inst_code::CMP_BOOLEAN: return "bcmp";
   case torricelly_inst_code::CMP_CHAR: return "ccmp";
   case torricelly_inst_code::CMP_FLOAT: return "fcmp";
   case torricelly_inst_code::CMP_DOUBLE: return "dcmp";
   case torricelly_inst_code::JUMP: return "jmp";
   case torricelly_inst_code::JUMP_EQ_ZERO: return "jez";
   case torricelly_inst_code::JUMP_LT_ZERO: return "jlz";
   case torricelly_inst_code::JUMP_LTE_ZERO: return "jlez";
   case torricelly_inst_code::JUMP_GT_ZERO: return "jgz";
   case torricelly_inst_code::JUMP_GTE_ZERO: return "jgez";
   case torricelly_inst_code::CAST_INT_BOOLEAN: return "itob";
   case torricelly_inst_code::CAST_INT_CHAR: return "itoc";
   case torricelly_inst_code::CAST_INT_FLOAT: return "itof";
   case torricelly_inst_code::CAST_INT_DOUBLE: return "itod";
   case torricelly_inst_code::CAST_BOOLEAN_INT: return "btoi";
   case torricelly_inst_code::CAST_CHAR_INT: return "ctoi";
   case torricelly_inst_code::CAST_FLOAT_INT: return "ftoi";
   case torricelly_inst_code::CAST_FLOAT_DOUBLE: return "ftod";
   case torricelly_inst_code::CAST_DOUBLE_INT: return "dtoi";
   case torricelly_inst_code::CAST_DOUBLE_FLOAT: return "dtof";
   case torricelly_inst_code::NOT: return "not";
   case torricelly_inst_code::AND: return "and";
   case torricelly_inst_code::OR: return "or";
   case torricelly_inst_code::INVOKE: return "invoke";
   case torricelly_inst_code::RET: return "ret";
   default: 
      throw torricelly_error("Cannot get instruction code string representation: unknown instruction code");
   }
}

bool torricelly_inst_code_helper::accept_parameter(torricelly_inst_code code)
{
   return accept_reference(code) || accept_label(code);
}

bool torricelly_inst_code_helper::accept_reference(torricelly_inst_code code)
{
   switch (code)
   {  
   case torricelly_inst_code::PUSH_INTEGER: 
   case torricelly_inst_code::PUSH_BOOLEAN:
   case torricelly_inst_code::PUSH_CHAR:
   case torricelly_inst_code::PUSH_FLOAT:
   case torricelly_inst_code::PUSH_DOUBLE:
   case torricelly_inst_code::LOAD_INTEGER:
   case torricelly_inst_code::LOAD_BOOLEAN:
   case torricelly_inst_code::LOAD_CHAR:
   case torricelly_inst_code::LOAD_FLOAT:
   case torricelly_inst_code::LOAD_DOUBLE:
   case torricelly_inst_code::STORE_INTEGER:
   case torricelly_inst_code::STORE_BOOLEAN:
   case torricelly_inst_code::STORE_CHAR:
   case torricelly_inst_code::STORE_FLOAT:
   case torricelly_inst_code::STORE_DOUBLE:
   case torricelly_inst_code::INVOKE:
      return true;
   default: 
      return false;
   }
}

bool torricelly_inst_code_helper::accept_label(torricelly_inst_code code)
{
   switch (code)
   {  
   case torricelly_inst_code::JUMP:
   case torricelly_inst_code::JUMP_EQ_ZERO:
   case torricelly_inst_code::JUMP_LT_ZERO:
   case torricelly_inst_code::JUMP_LTE_ZERO:
   case torricelly_inst_code::JUMP_GT_ZERO:
   case torricelly_inst_code::JUMP_GTE_ZERO:
      return true;
   default: 
      return false;
   }
}

std::ostream& torricelly::operator<<(std::ostream& os, torricelly_inst_code code){
   return os << to_string(code);
}
