#include <string>
#include <memory>

#include <gasp/blaise/language.hpp>
#include <gasp/blaise/tokens.hpp>
#include <gasp/common/tokenizer.hpp>

using namespace gasp::blaise::language;
using namespace gasp::blaise;
using namespace gasp::common;
using namespace std;

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
