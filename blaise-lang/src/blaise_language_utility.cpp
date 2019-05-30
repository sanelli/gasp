#include <gasp/blaise/language.hpp>
#include <gasp/blaise/tokens.hpp>
#include <gasp/common/tokenizer.hpp>

using namespace gasp::blaise;
using namespace gasp::blaise::language;

bool blaise_language_utility::is_numeric(blaise_language_expression_type type){
   switch(type){
      case blaise_language_expression_type::INTEGER:
      case blaise_language_expression_type::FLOAT:
      case blaise_language_expression_type::DOUBLE:
         return true;
      default:
         return false;
   }
}

bool blaise_language_utility::is_boolean(blaise_language_expression_type type){
   switch(type){
      // TODO: ADD a TRIBOOLEAN (true, false, undefined)
      case blaise_language_expression_type::BOOLEAN:
         return true;
      default:
         return false;
   }
}

bool blaise_language_utility::is_string(blaise_language_expression_type type){
   switch(type){
      // TODO: ADD a WIDE_CHAT for 16 bit charcater
      case blaise_language_expression_type::STRING:
         return true;
      default:
         return false;
   }
}

bool blaise_language_utility::is_char(blaise_language_expression_type type){
   switch(type){
      // TODO: ADD a WIDE_STRING type
      case blaise_language_expression_type::CHAR:
         return true;
      default:
         return false;
   }
}

blaise_language_expression_type blaise_language_utility::get_binary_numeric_result(blaise_language_expression_type left, blaise_language_expression_type right){
   if(!is_numeric(left))
      throw std::runtime_error("left operand is not a numeric type");
   if(!is_numeric(right))
      throw std::runtime_error("right operand is not a numeric type");

   if(left == blaise_language_expression_type::INTEGER && right == blaise_language_expression_type::INTEGER)
      return blaise_language_expression_type::INTEGER;
   if(left == blaise_language_expression_type::FLOAT && right == blaise_language_expression_type::FLOAT ||
      left == blaise_language_expression_type::INTEGER && right == blaise_language_expression_type::FLOAT ||
      left == blaise_language_expression_type::FLOAT && right == blaise_language_expression_type::INTEGER)
      return blaise_language_expression_type::FLOAT;
   if(left == blaise_language_expression_type::DOUBLE || right == blaise_language_expression_type::DOUBLE)
      return blaise_language_expression_type::DOUBLE;

   std::runtime_error("Unexpected runtime combination");
}

blaise_language_expression_type blaise_language_utility::get_binary_boolean_result(blaise_language_expression_type left, blaise_language_expression_type right){
   if(!is_boolean(left))
      throw std::runtime_error("left operand is not a boolean type");
   if(!is_boolean(right))
      throw std::runtime_error("right operand is not a boolean type");

   if(left == blaise_language_expression_type::BOOLEAN && right == blaise_language_expression_type::BOOLEAN)
      return blaise_language_expression_type::BOOLEAN;
   std::runtime_error("Unexpected runtime combination");
}

blaise_language_expression_type blaise_language_utility::get_binary_char_result(blaise_language_expression_type left, blaise_language_expression_type right){
   if(!is_char(left))
      throw std::runtime_error("left operand is not a char type");
   if(!is_char(right))
      throw std::runtime_error("right operand is not a char type");

   if(left == blaise_language_expression_type::CHAR && right == blaise_language_expression_type::CHAR)
      return blaise_language_expression_type::CHAR;
   std::runtime_error("Unexpected runtime combination");
}

blaise_language_expression_type blaise_language_utility::get_binary_string_result(blaise_language_expression_type left, blaise_language_expression_type right){
   if(!is_char(left) && !is_string(left))
      throw std::runtime_error("left operand is not a string/char type");
   if(!is_char(right) && !is_stirng(right))
      throw std::runtime_error("right operand is not a string/char type");
   if(!is_string(left) && !is_string(right))
      throw std::runtime_error("either the left or the right operand must be a string type");

   if(left == blaise_language_expression_type::STRING || right == blaise_language_expression_type::STRING)
      return blaise_language_expression_type::STRING;
   std::runtime_error("Unexpected runtime combination");
}

blaise_language_expression_type blaise_language_utility::get_resulting_type(const token<blaise_token_type>& token_operator, blaise_language_expression_type operand) {
   switch(token_operator.type()) {
      case blaise_token_type::LOGICAL_NOT:
         {
            if(is_boolean(operand))
               return operand;
         }
         break;
      case blaise_token_type::MINUS:
      {
         if(is_numeric(operand))
            return operand;
      }
   }

   throw blaise_language_error(token.line(), token.column(), make_string("Unsupported operator ", token_operator.type(), " for expression of type ", operand));
}

blaise_language_expression_type blaise_language_utility::get_resulting_type(const token<blaise_token_type>& token_operator, blaise_language_expression_type left, blaise_language_expression_type right) {
   switch(token_operator.type()) {
      case blaise_token_type::LOGICAL_AND:
      case blaise_token_type::LOGICAL_OR:
      case blaise_token_type::LOGICAL_EAGER_AND:
      case blaise_token_type::LOGICAL_EAGER_NOT:
      {
         if(is_boolean(left) && is_boolean(right))
            return get_binary_numeric_result(left, right);
      }
      break;
      case blaise_token_type::PLUS:
      {
         if(is_numeric(left) && is_numeric(right))
            return get_binary_numeric_result(left, right);

         if(is_char(left) && is_char(right))
            return get_binary_char_result(left, right);

         if((is_string(left) && (is_string(right) || is_char(right))) || 
            (is_string(right) && (is_string(left) || is_char(left))))
            return get_binary_string_result(left, right);
      }
      case blaise_token_type::MINUS:
      case blaise_token_type::MULTIPLY:
      case blaise_token_type::DIVIDE:
      {
         if(is_numeric(left) && is_numeric(right))
            return get_binary_numeric_result(left, right);
      }
      case blaise_token_type::REMINDER:
      {
         if(left == blaise_language_expression_type::INTEGER && right == blaise_language_expression_type::INTEGER)
            return blaise_language_expression_type::INTEGER;
      }
      case blaise_token_type::GREAT_THAN: 
      case blaise_token_type::LESS_THAN: 
      case blaise_token_type::GREAT_THAN_OR_EQUAL_TO: 
      case blaise_token_type::LESS_THAN_OR_EQUAL_TO: 
      case blaise_token_type::EQUAL_TO: 
      case blaise_token_type::NOT_EQUAL_TO:
      {
         if(is_numeric(left) && is_numeric(right) ||
            is_char(left) && is_char(right) ||
            is_string(left) && is_string(right))
               return blaise_language_expression_type::BOOLEAN;
      }
   }

   throw blaise_language_error(token.line(), token.column(), make_string("Unsupported operator ", token_operator.type(), " between expression of type ", left, " and ", right));
}