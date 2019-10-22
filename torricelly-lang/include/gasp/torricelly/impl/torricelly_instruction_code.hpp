#pragma once

#include <ostream>

namespace gasp::torricelly
{

enum class torricelly_inst_code : unsigned short
{
   NOOP = 0x0001,
   DUP,

   POP_INTEGER,
   POP_BOOLEAN,
   POP_CHAR,
   POP_FLOAT,
   POP_DOUBLE,

   LOAD_INTEGER,
   LOAD_BOOLEAN,
   LOAD_CHAR,
   LOAD_FLOAT,
   LOAD_DOUBLE,
   LOAD_ARRAY_INTEGER,
   LOAD_ARRAY_BOOLEAN,
   LOAD_ARRAY_CHAR,
   LOAD_ARRAY_FLOAT,
   LOAD_ARRAY_DOUBLE,

   STORE_INTEGER,
   STORE_BOOLEAN,
   STORE_CHAR,
   STORE_FLOAT,
   STORE_DOUBLE,
   STORE_ARRAY_INTEGER,
   STORE_ARRAY_BOOLEAN,
   STORE_ARRAY_CHAR,
   STORE_ARRAY_FLOAT,
   STORE_ARRAY_DOUBLE,

   ADD_INTEGER,
   ADD_FLOAT,
   ADD_DOUBLE,

   SUBTRACT_INTEGER,
   SUBTRACT_FLOAT,
   SUBTRACT_DOUBLE,

   MULTIPLY_INTEGER,
   MULTIPLY_FLOAT,
   MULTIPLY_DOUBLE,

   DIVIDE_INTEGER,
   DIVIDE_FLOAT,
   DIVIDE_DOUBLE,

   REMINDER_INTEGER,

   NEGATE_INTEGER,
   NEGATE_FLOAT,
   NEGATE_DOUBLE,

   CMP_INTEGER,
   CMP_BOOLEAN,
   CMP_CHAR,
   CMP_FLOAT,
   CMP_DOUBLE,

   JUMP,
   JUMP_EQ_ZERO,
   JUMP_LT_ZERO,
   JUMP_LTE_ZERO,
   JUMP_GT_ZERO,
   JUMP_GTE_ZERO,
   JUMP_NOT_ZERO,

   CAST_INT_BOOLEAN,
   CAST_INT_CHAR,
   CAST_INT_FLOAT,
   CAST_INT_DOUBLE,
   CAST_BOOLEAN_INT,
   CAST_CHAR_INT,
   CAST_FLOAT_INT,
   CAST_FLOAT_DOUBLE,
   CAST_DOUBLE_INT,
   CAST_DOUBLE_FLOAT,

   NOT,
   AND,
   OR,

   STATIC_INVOKE,
   NATIVE_INVOKE,
   VIRTUAL_INVOKE,
   DYNAMIC_INVOKE,
   RET,

   HALT,

   ALLOCATE_INT_ARRAY,
   ALLOCATE_CHAR_ARRAY,
   ALLOCATE_FLOAT_ARRAY,
   ALLOCATE_DOUBLE_ARRAY,
   ALLOCATE_BOOLEAN_ARRAY,
   FREE_ARRAY

};

class torricelly_inst_code_helper
{
public:
   static bool accept_parameter(torricelly_inst_code code);
   static bool accept_reference(torricelly_inst_code code);
   static bool accept_label(torricelly_inst_code code);
};

const char *to_string(torricelly_inst_code code);
std::ostream &operator<<(std::ostream &os, torricelly_inst_code code);

} // namespace gasp::torricelly