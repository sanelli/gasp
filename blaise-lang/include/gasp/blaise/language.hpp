#pragma once

#include <string>
#include <memory>
#include <stdexcept>

#include <gasp/blaise/tokens.hpp>
#include <gasp/common/tokenizer.hpp>

namespace gasp::blaise::language {

//
// EXCEPTION
//
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

//
// TYPES
//
enum class blaise_module_type {
   PROGRAM,
   MODULE
};

enum class blaise_expression_type {
   VOID,
   INTEGER,
   FLOAT,
   DOUBLE,
   CHAR,
   STRING,
   BOOLEAN
};

blaise_expression_type get_type_from_token(const gasp::common::token<gasp::blaise::blaise_token_type>& token);

//
// VARIABLE
//
class blaise_variable {
   std::string _name;
   blaise_expression_type _type;
   public:
      blaise_variable(const gasp::common::token<gasp::blaise::blaise_token_type>& identifier, 
                      const gasp::common::token<gasp::blaise::blaise_token_type>& type);

      std::string name() const;
      blaise_expression_type type() const;
};

class blaise_function_parameters : public blaise_variable{
   // TODO: Add modifiers like IN/OUT/INPUT
};

//
// SUB ROUTINES
//
class blaise_subroutine {
   std::string _name;
   blaise_expression_type _return_type;
   //TODO: Add parameters
   std::vector<std::shared_ptr<blaise_variable>> _variables;
public:
   blaise_subroutine(const std::string& name);
   blaise_subroutine(const std::string& name, blaise_expression_type return_type);

   std::string name() const;
   blaise_expression_type return_type() const;
   
   // Variables management;
   void add_variable(const gasp::common::token<gasp::blaise::blaise_token_type>& identifier, 
                     const gasp::common::token<gasp::blaise::blaise_token_type>& type);
   std::shared_ptr<blaise_variable> get_variable(const gasp::common::token<gasp::blaise::blaise_token_type>& identifier) const;
};

//
// MODULE
//
class blaise_module {
   blaise_module_type _type;
   std::string _name;
   std::vector<std::shared_ptr<blaise_subroutine>> _subroutines;

   public:
   blaise_module(const gasp::common::token<gasp::blaise::blaise_token_type>& identifier, blaise_module_type type);

   std::string name();
   blaise_module_type type();
   
   // subroutine management;
   void add_subroutine(const gasp::common::token<gasp::blaise::blaise_token_type>& identifier);
   void add_subroutine(const gasp::common::token<gasp::blaise::blaise_token_type>& identifier, 
                      const gasp::common::token<gasp::blaise::blaise_token_type>& return_type);

   std::shared_ptr<blaise_subroutine> get_subroutine(const gasp::common::token<gasp::blaise::blaise_token_type>& identifier) const;
};

//
// STATEMENTS
//
class blaise_statement {

};

//
// EXPRESSION
//


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