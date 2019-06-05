#include <gasp/blaise/ast.hpp>
#include <gasp/blaise/tokens.hpp>
#include <gasp/common/tokenizer.hpp>

using namespace gasp::blaise::ast;
using namespace gasp::blaise;
using namespace gasp::common;

//
// TYPE
//
blaise_ast_type gasp::blaise::ast::get_type_from_token(const gasp::common::token<gasp::blaise::blaise_token_type>& token) {
   switch(token.type()){
      case blaise_token_type::TYPE_INTEGER:
         return blaise_ast_type::INTEGER;
      case blaise_token_type::TYPE_CHAR:
         return blaise_ast_type::CHAR;
      case blaise_token_type::TYPE_STRING:
         return blaise_ast_type::STRING;
      case blaise_token_type::TYPE_FLOAT:
         return blaise_ast_type::FLOAT;
      case blaise_token_type::TYPE_DOUBLE:
         return blaise_ast_type::DOUBLE;
      case blaise_token_type::TYPE_BOOLEAN:
         return blaise_ast_type::BOOLEAN;
      default:
         throw blaise_ast_error(token.line(), token.column(), make_string("Canno convert token '", token.type() ,"' into a type."));
   }
}