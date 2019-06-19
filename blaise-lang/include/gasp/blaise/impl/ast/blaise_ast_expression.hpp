#pragma once

#include <string>
#include <memory>
#include <vector>

#include <gasp/blaise/tokens.hpp>
#include <gasp/common/tokenizer.hpp>
#include <gasp/common/memory.hpp>

#include <gasp/blaise/impl/ast/blaise_ast_common.hpp>
#include <gasp/blaise/impl/ast/blaise_ast_type.hpp>
#include <gasp/blaise/impl/ast/blaise_ast_expression_type.hpp>
#include <gasp/blaise/impl/ast/blaise_ast_subroutine.hpp>

namespace gasp::blaise::ast {

// Need this forward declaration otherwise the compile will complain
class blaise_ast_subroutine;

class blaise_ast_expression : public blaise_ast {
   std::shared_ptr<blaise_ast_type> _result_type;
   blaise_ast_expression_type _expression_type;

   public:
      blaise_ast_expression(const gasp::common::token<gasp::blaise::blaise_token_type>& reference, 
                     blaise_ast_expression_type expression_type, std::shared_ptr<blaise_ast_type> result_type);
      std::shared_ptr<blaise_ast_type> result_type() const;
      blaise_ast_expression_type expression_type() const;
};

// 
// TERNARY EXPRESSION
//
class blaise_ast_ternary_cast :  public blaise_ast_expression {
   std::shared_ptr<blaise_ast_expression> _condition;
   std::shared_ptr<blaise_ast_expression> _then_expression;
   std::shared_ptr<blaise_ast_expression> _else_expression;
   blaise_ast_ternary_cast(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
      std::shared_ptr<blaise_ast_expression> condition,
      std::shared_ptr<blaise_ast_expression> then_expression,
      std::shared_ptr<blaise_ast_expression> else_expression
   );
public:
   std::shared_ptr<blaise_ast_expression> condition() const;
   std::shared_ptr<blaise_ast_expression> then_expression() const;
   std::shared_ptr<blaise_ast_expression> else_expression() const;
   friend gasp::common::memory;
};
std::shared_ptr<blaise_ast_ternary_cast> make_blaise_ast_ternary_cast(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
      std::shared_ptr<blaise_ast_expression> condition,
      std::shared_ptr<blaise_ast_expression> then_expression,
      std::shared_ptr<blaise_ast_expression> else_expression
   );

// 
// CAST EXPRESSION
//
class blaise_ast_expression_cast :  public blaise_ast_expression {
   std::shared_ptr<blaise_ast_expression> _operand;
   blaise_ast_expression_cast(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
      std::shared_ptr<blaise_ast_type> target_type,
      std::shared_ptr<blaise_ast_expression> operand
   );
public:
   std::shared_ptr<blaise_ast_expression> operand() const;
   friend gasp::common::memory;
};
std::shared_ptr<blaise_ast_expression_cast> make_blaise_ast_expression_cast(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
      std::shared_ptr<blaise_ast_type> target_type,
      std::shared_ptr<blaise_ast_expression> operand);
std::shared_ptr<blaise_ast_expression> introduce_cast_if_required(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
      std::shared_ptr<blaise_ast_type> target_type,
      std::shared_ptr<blaise_ast_expression> expression);
void introduce_cast_if_required(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
      std::shared_ptr<blaise_ast_subroutine> subroutine,
      std::vector<std::shared_ptr<blaise_ast_expression>>& expressions);

//
// SUBROUTINE CALL EXPRESSION
//
class blaise_ast_expression_subroutine_call : public blaise_ast_expression {
   std::shared_ptr<blaise_ast_subroutine> _subroutine;
   std::vector<std::shared_ptr<blaise_ast_expression>> _expressions;
   blaise_ast_expression_subroutine_call(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
      std::shared_ptr<blaise_ast_subroutine> subroutine,
      const std::vector<std::shared_ptr<blaise_ast_expression>>& expressions
   );
public:
   typename std::vector<std::shared_ptr<blaise_ast_expression>>::const_iterator begin_actual_parameters() const;
   typename std::vector<std::shared_ptr<blaise_ast_expression>>::const_iterator end_actual_parameters() const;
   std::shared_ptr<blaise_ast_subroutine> subroutine() const;
   friend gasp::common::memory;
};
std::shared_ptr<blaise_ast_expression_subroutine_call> make_blaise_ast_expression_subroutine_call(
   gasp::common::token<gasp::blaise::blaise_token_type> subroutine_name_token,
   std::shared_ptr<blaise_ast_subroutine> subroutine,
   const std::vector<std::shared_ptr<blaise_ast_expression>>& expressions);

//
// BINARY EXPRESSION
//
class blaise_ast_expression_binary : public blaise_ast_expression {
   gasp::blaise::blaise_token_type _operator;
   std::shared_ptr<blaise_ast_expression> _left;
   std::shared_ptr<blaise_ast_expression> _right;
   blaise_ast_expression_binary(
               const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
               std::shared_ptr<blaise_ast_expression> left,
               gasp::blaise::blaise_token_type op,
               std::shared_ptr<blaise_ast_expression> right
   );
public:
   std::shared_ptr<blaise_ast_expression> left() const;
   std::shared_ptr<blaise_ast_expression> right() const;
   gasp::blaise::blaise_token_type op() const;
   friend gasp::common::memory;
};
std::shared_ptr<blaise_ast_expression_binary> make_blaise_ast_expression_binary(
               std::shared_ptr<blaise_ast_expression> left,
               gasp::common::token<gasp::blaise::blaise_token_type> token_operator,
               std::shared_ptr<blaise_ast_expression> right);

//
// UNARY EXPRESSION
//
class blaise_ast_expression_unary : public blaise_ast_expression {
   gasp::blaise::blaise_token_type _operator;
   std::shared_ptr<blaise_ast_expression> _operand;
   blaise_ast_expression_unary(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
      gasp::blaise::blaise_token_type op,
      std::shared_ptr<blaise_ast_expression> operand);
public:
   std::shared_ptr<blaise_ast_expression> operand() const;
   gasp::blaise::blaise_token_type op() const;
   friend gasp::common::memory;
};
std::shared_ptr<blaise_ast_expression_unary> make_blaise_ast_expression_unary(gasp::common::token<gasp::blaise::blaise_token_type> token,
                        std::shared_ptr<blaise_ast_expression> operand);

//
// EXPRESSION MEMORY LOCATION
//
class blaise_ast_expression_memory_location : public blaise_ast_expression {
   std::shared_ptr<blaise_ast_generic_memory_location> _memory_location;
   blaise_ast_expression_memory_location(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
                     std::shared_ptr<blaise_ast_generic_memory_location> variable);
public:
   std::shared_ptr<blaise_ast_generic_memory_location> memory_location() const;
   friend gasp::common::memory;
};
std::shared_ptr<blaise_ast_expression_memory_location> make_blaise_ast_expression_memory_location(const std::shared_ptr<blaise_ast_subroutine>& subroutine, gasp::common::token<gasp::blaise::blaise_token_type> token);

//
// EXPRESSION LITERAL VALUES
//
template<blaise_ast_expression_type TExpressionType, 
         blaise_ast_system_type TSystemType, 
         typename TValue>
class blaise_ast_expression_value : public blaise_ast_expression {
   TValue _value;
   public:
      blaise_ast_expression_value(const gasp::common::token<gasp::blaise::blaise_token_type>& reference, TValue value) 
            : blaise_ast_expression(reference, TExpressionType, make_plain_type(TSystemType)), _value(value) {}

      TValue value(){ return _value; }
};

class blaise_ast_expression_integer_value : public blaise_ast_expression_value<blaise_ast_expression_type::LITERAL_INTEGER,blaise_ast_system_type::INTEGER, int> {
   friend gasp::common::memory;
   blaise_ast_expression_integer_value(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,int value) : blaise_ast_expression_value(reference, value) {}
   };
class blaise_ast_expression_float_value : public blaise_ast_expression_value<blaise_ast_expression_type::LITERAL_FLOAT, blaise_ast_system_type::FLOAT, float> { 
   friend gasp::common::memory;
   blaise_ast_expression_float_value(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,float value) : blaise_ast_expression_value(reference, value) {}
};
class blaise_ast_expression_double_value : public blaise_ast_expression_value<blaise_ast_expression_type::LITERAL_DOUBLE, blaise_ast_system_type::DOUBLE, double> { 
   friend gasp::common::memory;
   blaise_ast_expression_double_value(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,double value) : blaise_ast_expression_value(reference, value) {}
};
class blaise_ast_expression_char_value : public blaise_ast_expression_value<blaise_ast_expression_type::LITERAL_CHAR, blaise_ast_system_type::CHAR, char> { 
   friend gasp::common::memory;
   blaise_ast_expression_char_value(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,char value) : blaise_ast_expression_value(reference, value) {}
};
class blaise_ast_expression_string_value : public blaise_ast_expression_value<blaise_ast_expression_type::LITERAL_STRING, blaise_ast_system_type::STRING, std::string> { 
   friend gasp::common::memory;
   blaise_ast_expression_string_value(const gasp::common::token<gasp::blaise::blaise_token_type>& reference, std::string value) : blaise_ast_expression_value(reference, value) {}
};
class blaise_ast_expression_boolean_value : public blaise_ast_expression_value<blaise_ast_expression_type::LITERAL_BOOLEAN, blaise_ast_system_type::BOOLEAN, bool> { 
   friend gasp::common::memory;
   blaise_ast_expression_boolean_value(const gasp::common::token<gasp::blaise::blaise_token_type>& reference, bool value) : blaise_ast_expression_value(reference, value) {}
};

}