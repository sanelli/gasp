#include <string>
#include <memory>
#include <algorithm>

#include <gasp/blaise/language.hpp>
#include <gasp/blaise/tokens.hpp>
#include <gasp/common/tokenizer.hpp>

using namespace gasp::blaise::language;
using namespace gasp::blaise;
using namespace gasp::common;
using namespace std;

//
// TYPE
//
blaise_expression_type gasp::blaise::language::get_type_from_token(const gasp::common::token<gasp::blaise::blaise_token_type>& token) {
   switch(token.type()){
      case blaise_token_type::TYPE_INTEGER:
         return blaise_expression_type::INTEGER;
      case blaise_token_type::TYPE_CHAR:
         return blaise_expression_type::CHAR;
      case blaise_token_type::TYPE_STRING:
         return blaise_expression_type::STRING;
      case blaise_token_type::TYPE_FLOAT:
         return blaise_expression_type::FLOAT;
      case blaise_token_type::TYPE_DOUBLE:
         return blaise_expression_type::DOUBLE;
      case blaise_token_type::TYPE_BOOLEAN:
         return blaise_expression_type::BOOLEAN;
      default:
         throw blaise_language_error(token.line(), token.column(), make_string("Canno convert token '", token.type() ,"' into a type."));
   }
}

//
// VARIABLES
//
gasp::blaise::language::blaise_variable::blaise_variable(const token<blaise_token_type>& identifier, 
                      const token<blaise_token_type>& type) : _name(identifier.value()), _type(get_type_from_token(type)) {
   if(identifier.type() != blaise_token_type::IDENTIFIER)
      throw blaise_language_error(identifier.line(), identifier.column(), make_string("A token of type '", blaise_token_type::IDENTIFIER,"' was expected but '", identifier.type(),"' was found."));
}

std::string blaise_variable::name() const { return _name; }
blaise_expression_type blaise_variable::type() const { return _type; }

//
// SUBROUTINE
//
gasp::blaise::language::blaise_subroutine::blaise_subroutine(const std::string& name): _name(name), _return_type(blaise_expression_type::VOID) { }
gasp::blaise::language::blaise_subroutine::blaise_subroutine(const std::string& name, blaise_expression_type return_type): _name(name), _return_type(return_type) { }

std::string gasp::blaise::language::blaise_subroutine::name() const { return _name; }
blaise_expression_type gasp::blaise::language::blaise_subroutine::return_type() const { return _return_type; }

void gasp::blaise::language::blaise_subroutine::add_variable(const token<blaise_token_type>& identifier, 
                     const token<blaise_token_type>& type){
   _variables.push_back(make_shared<blaise_variable>(identifier, type));
                     }
std::shared_ptr<blaise_variable> gasp::blaise::language::blaise_subroutine::get_variable(const token<blaise_token_type>& identifier) const {
   auto it = std::begin(_variables);
   auto end = std::end(_variables);
   while(it != end) {
      auto variable = *it;
      if(variable->name() == identifier.value())
         return variable;
   }
   return nullptr;
}

//
// MODULE
//
gasp::blaise::language::blaise_module::blaise_module(const token<blaise_token_type>& identifier, blaise_module_type type) 
: _name(identifier.value()), _type(type) {
  if(identifier.type() != blaise_token_type::IDENTIFIER)
      throw blaise_language_error(identifier.line(), identifier.column(), make_string("A token of type '", blaise_token_type::IDENTIFIER,"' was expected but '", identifier.type(),"' was found."));
}

std::string gasp::blaise::language::blaise_module::name() { return _name; }
blaise_module_type gasp::blaise::language::blaise_module::type() { return _type; }
   
void gasp::blaise::language::blaise_module::add_subroutine(const token<blaise_token_type>& identifier, 
                      const token<blaise_token_type>& return_type){
   _subroutines.push_back(make_shared<blaise_subroutine>(identifier.value(), get_type_from_token(return_type)));
   }

void gasp::blaise::language::blaise_module::add_subroutine(const token<blaise_token_type>& identifier){
   _subroutines.push_back(make_shared<blaise_subroutine>(identifier.value()));
   }

shared_ptr<blaise_subroutine> gasp::blaise::language::blaise_module::get_subroutine(const token<blaise_token_type>& identifier) const {
   auto it = std::begin(_subroutines);
   auto end = std::end(_subroutines);
   while(it != end) {
      auto subroutine = *it;
      if(subroutine->name() == identifier.value())
         return subroutine;
   }
   return nullptr;
}


//
// EXPRSSION
//
shared_ptr<blaise_expression> gasp::blaise::language::blaise_expression_value_factory(token<blaise_token_type> token){
   switch(token.type()){
      case blaise_token_type::INTEGER_LITERAL:
         return make_shared<blaise_expression_integer_value>(stoi(token.value()));
      case blaise_token_type::INTEGER_BASE_TWO_LITERAL:
         return make_shared<blaise_expression_integer_value>(stoi(token.value().substr(2), nullptr, 2));
      case blaise_token_type::INTEGER_BASE_EIGHT_LITERAL:
         return make_shared<blaise_expression_integer_value>(stoi(token.value().substr(2), nullptr, 8));
      case blaise_token_type::INTEGER_BASE_SIXTEEN_LITERAL:
         return make_shared<blaise_expression_integer_value>(stoi(token.value().substr(2), nullptr, 16));
      case blaise_token_type::FLOAT_LITERAL:
         return make_shared<blaise_expression_float_value>(stof(token.value()));
      case blaise_token_type::DOUBLE_LITERAL:
         return make_shared<blaise_expression_double_value>(stod(token.value()));
      case blaise_token_type::CHAR_LITERAL:
         // TODO: Handle the scenario where the char contains an escape sequence
         return make_shared<blaise_expression_char_value>(token.value()[1]);
      case blaise_token_type::STRING_LITERAL:
         // TODO: Handle the scenario where the char contains an escape sequence
         return make_shared<blaise_expression_string_value>(token.value().substr(1, token.value().length()-2));
      default:
         throw blaise_language_error(token.line(), token.column(), "Token is not an expression");
   }
}
