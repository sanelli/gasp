#pragma once

namespace gasp::torricelly {

enum class torricelly_inst_code : unsigned short {
   NOOP = 0x0001,

   PUSH_INTEGER,
   PUSH_BOOLEAN,
   PUSH_CHAR,
   PUSH_FLOAT,
   PUSH_DOUBLE,

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

   STORE_INTEGER,
   STORE_BOOLEAN,
   STORE_CHAR,
   STORE_FLOAT,
   STORE_DOUBLE,

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

   INVOKE,
   RET
};


class torricelly_inst_code_helper {
public:
   static bool accept_parameter(torricelly_inst_code code);
   static bool accept_reference(torricelly_inst_code code);
   static bool accept_label(torricelly_inst_code code);
};

const char* to_string(torricelly_inst_code code);
std::ostream& operator<<(std::ostream& os, torricelly_inst_code code);

}