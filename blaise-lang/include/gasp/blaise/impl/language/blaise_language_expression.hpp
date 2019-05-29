#pragma once

#include <string>
#include <memory>

#include <gasp/blaise/tokens.hpp>
#include <gasp/common/tokenizer.hpp>

#include <gasp/blaise/impl/language/blaise_language_expression_type.hpp>

namespace gasp::blaise::language {

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

class blaise_expression_integer_value : public blaise_expression_value<blaise_expression_type::INTEGER, int> {
   public: 
      blaise_expression_integer_value(int value) : blaise_expression_value(value) {}
   };
class blaise_expression_float_value : public blaise_expression_value<blaise_expression_type::FLOAT, float> { 
      public: 
      blaise_expression_float_value(float value) : blaise_expression_value(value) {}
};
class blaise_expression_double_value : public blaise_expression_value<blaise_expression_type::DOUBLE, double> { 
      public: 
      blaise_expression_double_value(double value) : blaise_expression_value(value) {}
};
class blaise_expression_char_value : public blaise_expression_value<blaise_expression_type::CHAR, char> { 
      public: 
      blaise_expression_char_value(char value) : blaise_expression_value(value) {}
};
class blaise_expression_string_value : public blaise_expression_value<blaise_expression_type::STRING, std::string> { 
      public: 
      blaise_expression_string_value(const std::string& value) : blaise_expression_value(value) {}
};

std::shared_ptr<blaise_expression> blaise_expression_value_factory(gasp::common::token<gasp::blaise::blaise_token_type> token);
}