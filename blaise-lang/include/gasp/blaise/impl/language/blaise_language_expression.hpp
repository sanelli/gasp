#pragma once

#include <string>
#include <memory>

#include <gasp/blaise/tokens.hpp>
#include <gasp/common/tokenizer.hpp>

#include <gasp/blaise/impl/language/blaise_language_type.hpp>
#include <gasp/blaise/impl/language/blaise_language_expression_type.hpp>
#include <gasp/blaise/impl/language/blaise_language_subroutine.hpp>

namespace gasp::blaise::language {

// TODO: Add function call expression

class blaise_expression {
   blaise_language_type _result_type;
   blaise_language_expression_type _expression_type;

   public:
      blaise_expression(blaise_language_expression_type expression_type, blaise_language_type result_type);
      blaise_language_type result_type() const;
      blaise_language_expression_type expression_type() const;
};

//
// BINARY EXPRESSION
//
class blaise_expression_binary : public blaise_expression {
   gasp::blaise::blaise_token_type _operator;
   std::shared_ptr<blaise_expression> _left;
   std::shared_ptr<blaise_expression> _right;
public:
   blaise_expression_binary(std::shared_ptr<blaise_expression> left,
               gasp::common::token<gasp::blaise::blaise_token_type> token,
               std::shared_ptr<blaise_expression> right
   );
   std::shared_ptr<blaise_expression> left() const;
   std::shared_ptr<blaise_expression> right() const;
   gasp::blaise::blaise_token_type op() const;
};
std::shared_ptr<blaise_expression_binary> blaise_expression_binary_factory(
               std::shared_ptr<blaise_expression> left,
               gasp::common::token<gasp::blaise::blaise_token_type> token_operator,
               std::shared_ptr<blaise_expression> right);

//
// UNARY EXPRESSION
//
class blaise_expression_unary : public blaise_expression {
   gasp::blaise::blaise_token_type _operator;
   std::shared_ptr<blaise_expression> _operand;
public:
   blaise_expression_unary(gasp::common::token<gasp::blaise::blaise_token_type> token_operator, 
      std::shared_ptr<blaise_expression> operand);
   std::shared_ptr<blaise_expression> operand() const;
   gasp::blaise::blaise_token_type op() const;
};
std::shared_ptr<blaise_expression_unary> blaise_expression_unary_factory(gasp::common::token<gasp::blaise::blaise_token_type> token,
                        std::shared_ptr<blaise_expression> operand);

//
// EXPRESSION VARIABLE
//
class blaise_expression_variable : public blaise_expression {
   std::shared_ptr<blaise_generic_memory_location> _memory_location;
public:
   blaise_expression_variable(std::shared_ptr<blaise_generic_memory_location> variable);
   std::shared_ptr<blaise_generic_memory_location> memory_location() const;
};
std::shared_ptr<blaise_expression_variable> blaise_expression_variable_factory(const std::shared_ptr<blaise_subroutine>& subroutine, gasp::common::token<gasp::blaise::blaise_token_type> token);

//
// EXPRESSION LITERAL VALUES
//
template<blaise_language_expression_type TExpressionType, 
         blaise_language_type TType, 
         typename TValue>
class blaise_expression_value : public blaise_expression {
   TValue _value;
   public:
      blaise_expression_value(TValue value) 
            : blaise_expression(TExpressionType, TType), _value(value) {}

      TValue value(){ return _value; }
};

class blaise_expression_integer_value : public blaise_expression_value<blaise_language_expression_type::LITERAL_INTEGER,blaise_language_type::INTEGER, int> {
   public: 
      blaise_expression_integer_value(int value) : blaise_expression_value(value) {}
   };
class blaise_expression_float_value : public blaise_expression_value<blaise_language_expression_type::LITERAL_FLOAT, blaise_language_type::FLOAT, float> { 
      public: 
      blaise_expression_float_value(float value) : blaise_expression_value(value) {}
};
class blaise_expression_double_value : public blaise_expression_value<blaise_language_expression_type::LITERAL_DOUBLE, blaise_language_type::DOUBLE, double> { 
      public: 
      blaise_expression_double_value(double value) : blaise_expression_value(value) {}
};
class blaise_expression_char_value : public blaise_expression_value<blaise_language_expression_type::LITERAL_CHAR, blaise_language_type::CHAR, char> { 
      public: 
      blaise_expression_char_value(char value) : blaise_expression_value(value) {}
};
class blaise_expression_string_value : public blaise_expression_value<blaise_language_expression_type::LITERAL_STRING, blaise_language_type::STRING, std::string> { 
      public: 
      blaise_expression_string_value(std::string value) : blaise_expression_value(value) {}
};
class blaise_expression_boolean_value : public blaise_expression_value<blaise_language_expression_type::LITERAL_BOOLEAN, blaise_language_type::BOOLEAN, bool> { 
      public: 
      blaise_expression_boolean_value(bool value) : blaise_expression_value(value) {}
};

std::shared_ptr<blaise_expression> blaise_expression_value_factory(gasp::common::token<gasp::blaise::blaise_token_type> token);
}