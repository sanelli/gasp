#include <stdexcept>

#include <gasp/blaise/ast.hpp>
#include <gasp/blaise/tokens.hpp>
#include <gasp/common/tokenizer.hpp>
#include <gasp/common/string.hpp>
#include <gasp/common/debug.hpp>

using namespace gasp::common;
using namespace gasp::blaise;
using namespace gasp::blaise::ast;

bool blaise_ast_utility::is_numeric(blaise_ast_type type){
   switch(type){
      case blaise_ast_type::INTEGER:
      case blaise_ast_type::FLOAT:
      case blaise_ast_type::DOUBLE:
         return true;
      default:
         return false;
   }
}

bool blaise_ast_utility::is_boolean(blaise_ast_type type){
   switch(type){
      case blaise_ast_type::BOOLEAN:
         return true;
      default:
         return false;
   }
}

bool blaise_ast_utility::is_string(blaise_ast_type type){
   switch(type){
      case blaise_ast_type::STRING:
         return true;
      default:
         return false;
   }
}

bool blaise_ast_utility::is_char(blaise_ast_type type){
   switch(type){
      case blaise_ast_type::CHAR:
         return true;
      default:
         return false;
   }
}

blaise_ast_type blaise_ast_utility::get_binary_numeric_result(blaise_ast_type left, blaise_ast_type right){
   if(!is_numeric(left))
      throw std::runtime_error("left operand is not a numeric type");
   if(!is_numeric(right))
      throw std::runtime_error("right operand is not a numeric type");

   if(left == blaise_ast_type::INTEGER && right == blaise_ast_type::INTEGER)
      return blaise_ast_type::INTEGER;
   if((left == blaise_ast_type::FLOAT && right == blaise_ast_type::FLOAT) ||
      (left == blaise_ast_type::INTEGER && right == blaise_ast_type::FLOAT) ||
      (left == blaise_ast_type::FLOAT && right == blaise_ast_type::INTEGER))
      return blaise_ast_type::FLOAT;
   if(left == blaise_ast_type::DOUBLE || right == blaise_ast_type::DOUBLE)
      return blaise_ast_type::DOUBLE;

   throw std::runtime_error("Unexpected runtime combination");
}

blaise_ast_type blaise_ast_utility::get_binary_boolean_result(blaise_ast_type left, blaise_ast_type right){
   if(!is_boolean(left))
      throw std::runtime_error("left operand is not a boolean type");
   if(!is_boolean(right))
      throw std::runtime_error("right operand is not a boolean type");

   if(left == blaise_ast_type::BOOLEAN && right == blaise_ast_type::BOOLEAN)
      return blaise_ast_type::BOOLEAN;
   throw std::runtime_error("Unexpected runtime combination");
}

blaise_ast_type blaise_ast_utility::get_binary_char_result(blaise_ast_type left, blaise_ast_type right){
   if(!is_char(left))
      throw std::runtime_error("left operand is not a char type");
   if(!is_char(right))
      throw std::runtime_error("right operand is not a char type");

   if(left == blaise_ast_type::CHAR && right == blaise_ast_type::CHAR)
      return blaise_ast_type::CHAR;
   throw std::runtime_error("Unexpected runtime combination");
}

blaise_ast_type blaise_ast_utility::get_binary_string_result(blaise_ast_type left, blaise_ast_type right){
   if(!is_char(left) && !is_string(left))
      throw std::runtime_error("left operand is not a string/char type");
   if(!is_char(right) && !is_string(right))
      throw std::runtime_error("right operand is not a string/char type");
   if(!is_string(left) && !is_string(right))
      throw std::runtime_error("either the left or the right operand must be a string type");

   if(left == blaise_ast_type::STRING || right == blaise_ast_type::STRING)
      return blaise_ast_type::STRING;
   throw std::runtime_error("Unexpected runtime combination");
}

blaise_ast_type blaise_ast_utility::get_resulting_type(const gasp::common::token<gasp::blaise::blaise_token_type>& reference, blaise_token_type op, blaise_ast_type operand) {
   switch(op) {
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
         break;
      default:
         throw blaise_ast_error(reference.line(), reference.column(), make_string("Unsupported unary operator ", op, " for expression of type ", operand));
   }

   throw blaise_ast_error(reference.line(), reference.column(), make_string("Operator ", op, " does not support expression of type ", operand));
}

blaise_ast_type blaise_ast_utility::get_resulting_type(const token<blaise_token_type>& reference, blaise_token_type op, blaise_ast_type left, blaise_ast_type right) {
   switch(op) {
      case blaise_token_type::LOGICAL_AND:
      case blaise_token_type::LOGICAL_OR:
      case blaise_token_type::LOGICAL_EAGER_AND:
      case blaise_token_type::LOGICAL_EAGER_OR:
      {
         if(is_boolean(left) && is_boolean(right))
            return get_binary_boolean_result(left, right);
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
      break;
      case blaise_token_type::MINUS:
      case blaise_token_type::MULTIPLY:
      case blaise_token_type::DIVIDE:
      {
         if(is_numeric(left) && is_numeric(right))
            return get_binary_numeric_result(left, right);
      }
      break;
      case blaise_token_type::REMAINDER:
      {
         if(left == blaise_ast_type::INTEGER && right == blaise_ast_type::INTEGER)
            return blaise_ast_type::INTEGER;
      }
      break;
      case blaise_token_type::GREAT_THAN: 
      case blaise_token_type::LESS_THAN: 
      case blaise_token_type::GREAT_THAN_OR_EQUAL_TO: 
      case blaise_token_type::LESS_THAN_OR_EQUAL_TO: 
      case blaise_token_type::EQUAL_TO: 
      case blaise_token_type::NOT_EQUAL_TO:
      {
         if((is_numeric(left) && is_numeric(right)) ||
            (is_char(left) && is_char(right)) ||
            (is_string(left) && is_string(right)))
               return blaise_ast_type::BOOLEAN;
      }
      break;
      default:
            throw blaise_ast_error(reference.line(), reference.column(), make_string("Unsupported operator ", op, " between expression of type ", left, " and ", right));
   }

   throw blaise_ast_error(reference.line(), reference.column(), make_string("Operator ", op, " between expression of type ", left, " and ", right," is not allowed."));
}

bool blaise_ast_utility::can_cast(blaise_ast_type from, blaise_ast_type to) {
   if(is_numeric(from) && is_numeric(to)){
      switch(from){
         case blaise_ast_type::INTEGER:
            switch(to){
               case blaise_ast_type::INTEGER:
               case blaise_ast_type::FLOAT:
               case blaise_ast_type::DOUBLE:
                  return true;
               default:
                  throw std::runtime_error(make_string("Unsupported type '", to,"' passed as 'to' parameter"));
            }
         case blaise_ast_type::FLOAT:
            switch(to){
               case blaise_ast_type::INTEGER:
                  return false;
               case blaise_ast_type::FLOAT:
               case blaise_ast_type::DOUBLE:
                  return true;
               default:
                  throw std::runtime_error(make_string("Unsupported type '", to,"' passed as 'to' parameter"));
            }
         case blaise_ast_type::DOUBLE:
            switch(to){
               case blaise_ast_type::INTEGER:
               case blaise_ast_type::FLOAT:
                  return false;
               case blaise_ast_type::DOUBLE:
                  return true;
               default:
                  throw std::runtime_error(make_string("Unsupported type '", to,"' passed as 'to' parameter"));
            }
         default:
            throw std::runtime_error(make_string("Unsupported type '", from,"' passed as 'from' parameter"));
      }
   }

   return false;
}

bool blaise_ast_utility::can_force_cast(blaise_ast_type from, blaise_ast_type to) {
   // Same types I always can force a conversion
   if(is_numeric(from) && is_numeric(to)) return true;
   if(is_boolean(from) && is_boolean(to)) return true;
   if(is_char(from) && is_char(to)) return true;
   if(is_string(from) && is_string(to)) return true;

   // integer <--> boolean
   if(from == blaise_ast_type::INTEGER && is_boolean(to)) return true;
   if(is_boolean(from) && to == blaise_ast_type::INTEGER) return true;

   // integer <--> char
   if(from == blaise_ast_type::INTEGER && is_char(to)) return true;
   if(is_char(from) && to == blaise_ast_type::INTEGER) return true;
   return false;
}