#pragma once

#include <string>
#include <memory>
#include <stdexcept>

#include <gasp/blaise/tokens.hpp>
#include <gasp/common/tokenizer.hpp>

namespace gasp::blaise::language {

class blaise_language_error : public std::runtime_error
{
   int _line;
   int _column;

public:
   blaise_language_error(int line, int column, const char *message) : runtime_error(message), _line(line), _column(column) {}
   blaise_language_error(int line, int column, const std::string &message) : runtime_error(message), _line(line), _column(column) {}

   int line() { return _line; }
   int column() { return _column; }
};


   enum class blaise_module_type {
      PROGRAM,
      MODULE
   };

   class blaise_module {
      blaise_module_type module_type;
      std::string module_name;
   };

   class blaise_statement {

   };

   //
   // EXPRESSION
   //
   enum class blaise_expression_type {
      INTEGER_LITERAL,
      FLOAT_LITERAL,
      DOUBLE_LITERAL,
      CHAR_LITERAL,
      STRING_LITERAL
   };

   class blaise_expression {
      blaise_expression_type _type;
      public:
         blaise_expression(blaise_expression_type type) : _type(type){}
         blaise_expression_type type() { return _type; }
   };

   template<blaise_expression_type TExpressionType, typename TValue>
   class blaise_expression_value : public blaise_expression {
      TValue _value;
      public:
         blaise_expression_value(const TValue& value) : blaise_expression(TExpressionType), _value(value) {}

         TValue value(){ return _value; }
   };

   class blaise_expression_integer_value : public blaise_expression_value<blaise_expression_type::INTEGER_LITERAL, int> {
      public: 
         blaise_expression_integer_value(int value) : blaise_expression_value(value) {}
    };
   class blaise_expression_float_value : public blaise_expression_value<blaise_expression_type::FLOAT_LITERAL, float> { 
       public: 
         blaise_expression_float_value(float value) : blaise_expression_value(value) {}
   };
   class blaise_expression_double_value : public blaise_expression_value<blaise_expression_type::DOUBLE_LITERAL, double> { 
       public: 
         blaise_expression_double_value(double value) : blaise_expression_value(value) {}
   };
   class blaise_expression_char_value : public blaise_expression_value<blaise_expression_type::CHAR_LITERAL, char> { 
       public: 
         blaise_expression_char_value(char value) : blaise_expression_value(value) {}
   };
   class blaise_expression_string_value : public blaise_expression_value<blaise_expression_type::STRING_LITERAL, std::string> { 
       public: 
         blaise_expression_string_value(const std::string& value) : blaise_expression_value(value) {}
   };

   std::shared_ptr<blaise_expression> blaise_expression_value_factory(gasp::common::token<gasp::blaise::blaise_token_type> token);

}