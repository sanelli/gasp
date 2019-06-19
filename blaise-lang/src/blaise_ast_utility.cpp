#include <stdexcept>
#include <memory>

#include <gasp/blaise/ast.hpp>
#include <gasp/blaise/tokens.hpp>
#include <gasp/common/tokenizer.hpp>
#include <gasp/common/string.hpp>
#include <gasp/common/debug.hpp>

using namespace gasp::common;
using namespace gasp::blaise;
using namespace gasp::blaise::ast;

std::shared_ptr<blaise_ast_plain_type> blaise_ast_utility::as_plain_type(std::shared_ptr<blaise_ast_type> type){
   return std::dynamic_pointer_cast<blaise_ast_plain_type>(type);
}

bool blaise_ast_utility::is_integer(std::shared_ptr<blaise_ast_type> type) {
   if(!is_numeric(type)) return false;
   return as_plain_type(type)->system_type() == blaise_ast_system_type::INTEGER;
}

bool blaise_ast_utility::is_numeric(std::shared_ptr<blaise_ast_type> type){
   if(type->type_type() != ast::blaise_ast_type_type::PLAIN) return false;
   auto plain_type = as_plain_type(type);
   switch(plain_type->system_type()) {
      case blaise_ast_system_type::INTEGER:
      case blaise_ast_system_type::FLOAT:
      case blaise_ast_system_type::DOUBLE:
         return true;
      default:
         return false;
   }
}

bool blaise_ast_utility::is_boolean(std::shared_ptr<blaise_ast_type> type){
   if(type->type_type() != ast::blaise_ast_type_type::PLAIN) return false;
   auto plain_type = as_plain_type(type);
   switch(plain_type->system_type()) {      
      case blaise_ast_system_type::BOOLEAN:
         return true;
      default:
         return false;
   }
}

bool blaise_ast_utility::is_string(std::shared_ptr<blaise_ast_type> type){
   if(type->type_type() != ast::blaise_ast_type_type::PLAIN) return false;
   auto plain_type = as_plain_type(type);
   switch(plain_type->system_type()) {
      case blaise_ast_system_type::STRING:
         return true;
      default:
         return false;
   }
}

bool blaise_ast_utility::is_char(std::shared_ptr<blaise_ast_type> type){
   if(type->type_type() != ast::blaise_ast_type_type::PLAIN) return false;
   auto plain_type = as_plain_type(type);
   switch(plain_type->system_type()) {
      case blaise_ast_system_type::CHAR:
         return true;
      default:
         return false;
   }
}

std::shared_ptr<blaise_ast_type> blaise_ast_utility::get_binary_numeric_result(std::shared_ptr<blaise_ast_type> left, std::shared_ptr<blaise_ast_type> right){
   if(!is_numeric(left))
      throw std::runtime_error("left operand is not a numeric type");
   if(!is_numeric(right))
      throw std::runtime_error("right operand is not a numeric type");

   auto system_type_left = as_plain_type(left)->system_type();
   auto system_type_right = as_plain_type(right)->system_type();

   if(system_type_left == blaise_ast_system_type::INTEGER && system_type_right == blaise_ast_system_type::INTEGER)
      return ast::make_plain_type(blaise_ast_system_type::INTEGER);
   if((system_type_left == blaise_ast_system_type::FLOAT && system_type_right == blaise_ast_system_type::FLOAT) ||
      (system_type_left == blaise_ast_system_type::INTEGER && system_type_right == blaise_ast_system_type::FLOAT) ||
      (system_type_left == blaise_ast_system_type::FLOAT && system_type_right == blaise_ast_system_type::INTEGER))
      return ast::make_plain_type(blaise_ast_system_type::FLOAT);
   if(system_type_left == blaise_ast_system_type::DOUBLE || system_type_right == blaise_ast_system_type::DOUBLE)
      return ast::make_plain_type(blaise_ast_system_type::DOUBLE);

   throw std::runtime_error("Unexpected runtime combination");
}

std::shared_ptr<blaise_ast_type> blaise_ast_utility::get_binary_boolean_result(std::shared_ptr<blaise_ast_type> left, std::shared_ptr<blaise_ast_type> right){
   if(!is_boolean(left))
      throw std::runtime_error("left operand is not a boolean type");
   if(!is_boolean(right))
      throw std::runtime_error("right operand is not a boolean type");

   auto system_type_left = as_plain_type(left)->system_type();
   auto system_type_right = as_plain_type(right)->system_type();

   if(system_type_left == blaise_ast_system_type::BOOLEAN && system_type_right == blaise_ast_system_type::BOOLEAN)
      return ast::make_plain_type(blaise_ast_system_type::BOOLEAN);
   throw std::runtime_error("Unexpected runtime combination");
}

std::shared_ptr<blaise_ast_type> blaise_ast_utility::get_binary_char_result(std::shared_ptr<blaise_ast_type> left, std::shared_ptr<blaise_ast_type> right){
   if(!is_char(left))
      throw std::runtime_error("left operand is not a char type");
   if(!is_char(right))
      throw std::runtime_error("right operand is not a char type");

   auto system_type_left = as_plain_type(left)->system_type();
   auto system_type_right = as_plain_type(right)->system_type();

   if(system_type_left == blaise_ast_system_type::CHAR && system_type_right == blaise_ast_system_type::CHAR)
      return ast::make_plain_type(blaise_ast_system_type::CHAR);
   throw std::runtime_error("Unexpected runtime combination");
}

std::shared_ptr<blaise_ast_type> blaise_ast_utility::get_binary_string_result(std::shared_ptr<blaise_ast_type> left, std::shared_ptr<blaise_ast_type> right){
   if(!is_char(left) && !is_string(left))
      throw std::runtime_error("left operand is not a string/char type");
   if(!is_char(right) && !is_string(right))
      throw std::runtime_error("right operand is not a string/char type");
   if(!is_string(left) && !is_string(right))
      throw std::runtime_error("either the left or the right operand must be a string type");

   auto system_type_left = as_plain_type(left)->system_type();
   auto system_type_right = as_plain_type(right)->system_type();

   if(system_type_left == blaise_ast_system_type::STRING || system_type_right == blaise_ast_system_type::STRING)
      return ast::make_plain_type(blaise_ast_system_type::STRING);
   throw std::runtime_error("Unexpected runtime combination");
}

std::shared_ptr<blaise_ast_type> blaise_ast_utility::get_resulting_type(const gasp::common::token<gasp::blaise::blaise_token_type>& reference, blaise_token_type op, std::shared_ptr<blaise_ast_type> operand) {
   switch(op) {
      case blaise_token_type::LOGICAL_NOT:
         {
            if(is_boolean(operand))
               return ast::make_plain_type(blaise_ast_system_type::BOOLEAN);
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

std::shared_ptr<blaise_ast_type> blaise_ast_utility::get_resulting_type(const token<blaise_token_type>& reference, blaise_token_type op, std::shared_ptr<blaise_ast_type> left, std::shared_ptr<blaise_ast_type> right) {
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
         auto system_type_left = as_plain_type(left)->system_type();
         auto system_type_right = as_plain_type(right)->system_type();
         if(system_type_left == blaise_ast_system_type::INTEGER && system_type_right == blaise_ast_system_type::INTEGER)
            return make_plain_type(blaise_ast_system_type::INTEGER);
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
               return make_plain_type(blaise_ast_system_type::BOOLEAN);
      }
      break;
      default:
            throw blaise_ast_error(reference.line(), reference.column(), make_string("Unsupported operator ", op, " between expression of type ", left, " and ", right));
   }

   throw blaise_ast_error(reference.line(), reference.column(), make_string("Operator ", op, " between expression of type ", left, " and ", right," is not allowed."));
}

bool blaise_ast_utility::can_auto_cast(std::shared_ptr<blaise_ast_type> from, std::shared_ptr<blaise_ast_type> to) {
   if(is_numeric(from) && is_numeric(to)){
      auto system_type_from = as_plain_type(from)->system_type();
      auto system_type_to = as_plain_type(to)->system_type();
      switch(system_type_from){
         case blaise_ast_system_type::INTEGER:
            switch(system_type_to){
               case blaise_ast_system_type::INTEGER:
               case blaise_ast_system_type::FLOAT:
               case blaise_ast_system_type::DOUBLE:
                  return true;
               default:
                  throw std::runtime_error(make_string("Unsupported type '", to,"' passed as 'to' parameter"));
            }
         case blaise_ast_system_type::FLOAT:
            switch(system_type_to){
               case blaise_ast_system_type::INTEGER:
                  return false;
               case blaise_ast_system_type::FLOAT:
               case blaise_ast_system_type::DOUBLE:
                  return true;
               default:
                  throw std::runtime_error(make_string("Unsupported type '", to,"' passed as 'to' parameter"));
            }
         case blaise_ast_system_type::DOUBLE:
            switch(system_type_to){
               case blaise_ast_system_type::INTEGER:
               case blaise_ast_system_type::FLOAT:
                  return false;
               case blaise_ast_system_type::DOUBLE:
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

bool blaise_ast_utility::can_force_cast(std::shared_ptr<blaise_ast_type> from, std::shared_ptr<blaise_ast_type> to) {
   // Same types I always can force a conversion
   if(is_numeric(from) && is_numeric(to)) return true;
   if(is_boolean(from) && is_boolean(to)) return true;
   if(is_char(from) && is_char(to)) return true;
   if(is_string(from) && is_string(to)) return true;

   // integer <--> boolean
   if(is_integer(from) && is_boolean(to)) return true;
   if(is_boolean(from) && is_integer(to)) return true;

   // integer <--> char
   if(is_integer(from) && is_char(to)) return true;
   if(is_char(from) && is_integer(to)) return true;
   return false;
}

std::shared_ptr<blaise_ast_type> blaise_ast_utility::get_common_type(const gasp::common::token<gasp::blaise::blaise_token_type>& reference, std::shared_ptr<blaise_ast_type> typeA, std::shared_ptr<blaise_ast_type> typeB){
   if(typeA->equals(typeB)) return typeA;
   if(can_auto_cast(typeA, typeB)) return typeB;
   if(can_auto_cast(typeB, typeA)) return typeA;
   throw blaise_ast_error(reference.line(), reference.column(), make_string("Mismatch types '", typeA,"' and '", typeB, "'."));
}