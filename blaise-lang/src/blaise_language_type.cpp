#include <gasp/blaise/language.hpp>
#include <gasp/blaise/tokens.hpp>
#include <gasp/common/tokenizer.hpp>

using namespace gasp::blaise::language;
using namespace gasp::blaise;
using namespace gasp::common;

//
// TYPE
//
blaise_language_type gasp::blaise::language::get_type_from_token(const gasp::common::token<gasp::blaise::blaise_token_type>& token) {
   switch(token.type()){
      case blaise_token_type::TYPE_INTEGER:
         return blaise_language_type::INTEGER;
      case blaise_token_type::TYPE_CHAR:
         return blaise_language_type::CHAR;
      case blaise_token_type::TYPE_STRING:
         return blaise_language_type::STRING;
      case blaise_token_type::TYPE_FLOAT:
         return blaise_language_type::FLOAT;
      case blaise_token_type::TYPE_DOUBLE:
         return blaise_language_type::DOUBLE;
      case blaise_token_type::TYPE_BOOLEAN:
         return blaise_language_type::BOOLEAN;
      default:
         throw blaise_language_error(token.line(), token.column(), make_string("Canno convert token '", token.type() ,"' into a type."));
   }
}