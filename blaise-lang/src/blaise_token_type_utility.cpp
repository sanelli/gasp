#include <gasp/blaise/tokens.hpp>

using namespace gasp::blaise;
using namespace gasp::common;

bool blaise_token_type_utility::is_type(blaise_token_type token_type)
{
   switch (token_type)
   {
   case blaise_token_type::TYPE_INTEGER:
   case blaise_token_type::TYPE_CHAR:
   case blaise_token_type::TYPE_STRING:
   case blaise_token_type::TYPE_FLOAT:
   case blaise_token_type::TYPE_DOUBLE:
   case blaise_token_type::TYPE_BOOLEAN:
      return true;
   default:
      return false;
   };
}

bool blaise_token_type_utility::is_unsigned_type(blaise_token_type token_type)
{
   switch (token_type)
   {
   case blaise_token_type::TYPE_INTEGER:
      return true;
   default:
      return false;
   };
}

bool blaise_token_type_utility::is_operator(blaise_token_type token_type){
   switch(token_type){
      case blaise_token_type::LOGICAL_NOT:
      case blaise_token_type::MULTIPLY:
      case blaise_token_type::DIVIDE:
      case blaise_token_type::REMAINDER:
      case blaise_token_type::PLUS:
      case blaise_token_type::MINUS:
      case blaise_token_type::GREAT_THAN_OR_EQUAL_TO:
      case blaise_token_type::LESS_THAN_OR_EQUAL_TO:
      case blaise_token_type::GREAT_THAN:
      case blaise_token_type::LESS_THAN:
      case blaise_token_type::EQUAL_TO:
      case blaise_token_type::NOT_EQUAL_TO:
      case blaise_token_type::LOGICAL_AND:
      case blaise_token_type::LOGICAL_EAGER_AND:
      case blaise_token_type::LOGICAL_EAGER_OR:
         return true;
      default:
         return false;
   }
}

// Loosly following https://it.cppreference.com/w/cpp/language/operator_precedence
bool blaise_token_type_utility::get_operator_precedence(blaise_token_type token_type){
   switch(token_type){
      case blaise_token_type::LOGICAL_NOT:
      case blaise_token_type::CAST:
         return 100;
      case blaise_token_type::MULTIPLY:
      case blaise_token_type::DIVIDE:
      case blaise_token_type::REMAINDER:
         return 90;
      case blaise_token_type::PLUS:
      case blaise_token_type::MINUS:
         return 80;
      case blaise_token_type::GREAT_THAN_OR_EQUAL_TO:
      case blaise_token_type::LESS_THAN_OR_EQUAL_TO:
      case blaise_token_type::GREAT_THAN:
      case blaise_token_type::LESS_THAN:
         return 70;
      case blaise_token_type::EQUAL_TO:
      case blaise_token_type::NOT_EQUAL_TO:
         return 60;
      case blaise_token_type::LOGICAL_AND:
      case blaise_token_type::LOGICAL_EAGER_AND:
         return 50;
      case blaise_token_type::LOGICAL_OR:
      case blaise_token_type::LOGICAL_EAGER_OR:
         return 40;
      case blaise_token_type::IF: // Terary
         return 30;
      default:
         throw gasp::common::tokenizer_error(0,0, make_string("unexpected token with type '",token_type,"': it is not a supported operator"));
   }
}

bool blaise_token_type_utility::is_number(blaise_token_type token_type){
   switch(token_type){
      case blaise_token_type::INTEGER_LITERAL:
      case blaise_token_type::DOUBLE_LITERAL: 
      case blaise_token_type::FLOAT_LITERAL: 
      case blaise_token_type::INTEGER_BASE_TWO_LITERAL: 
      case blaise_token_type::INTEGER_BASE_EIGHT_LITERAL:
      case blaise_token_type::INTEGER_BASE_SIXTEEN_LITERAL:
         return true;
      default:
         return false;
   }
}

bool blaise_token_type_utility::is_boolean(blaise_token_type token_type){
   switch(token_type){
      case blaise_token_type::BOOLEAN_LITERAL:
         return true;
      default:
         return false;
   }
}