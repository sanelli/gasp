#include <stdexcept>
#include <memory>

#include <gasp/blaise/ast.hpp>

#include <sanelli/sanelli.hpp>

using namespace sanelli;
using namespace gasp::blaise;
using namespace gasp::blaise::ast;

std::shared_ptr<blaise_ast_plain_type> blaise_ast_utility::as_plain_type(const std::shared_ptr<blaise_ast_type> type)
{
   return std::dynamic_pointer_cast<blaise_ast_plain_type>(type);
}

std::shared_ptr<blaise_ast_array_type> blaise_ast_utility::as_array_type(const std::shared_ptr<blaise_ast_type> type)
{
   return std::dynamic_pointer_cast<blaise_ast_array_type>(type);
}

std::shared_ptr<const blaise_ast_plain_type> blaise_ast_utility::as_plain_type(const std::shared_ptr<const blaise_ast_type> type)
{
   return std::dynamic_pointer_cast<const blaise_ast_plain_type>(type);
}

std::shared_ptr<const blaise_ast_array_type> blaise_ast_utility::as_array_type(const std::shared_ptr<const blaise_ast_type> type)
{
   return std::dynamic_pointer_cast<const blaise_ast_array_type>(type);
}

bool blaise_ast_utility::is_numeric(std::shared_ptr<blaise_ast_type> type, blaise_ast_system_type system_type)
{
   if (!is_numeric(type))
      return false;
   return as_plain_type(type)->system_type() == system_type;
}

bool blaise_ast_utility::is_integer(std::shared_ptr<blaise_ast_type> type)
{
   return is_numeric(type, blaise_ast_system_type::INTEGER);
}

bool blaise_ast_utility::is_numeric(std::shared_ptr<blaise_ast_type> type)
{
   if (type->type_type() != ast::blaise_ast_type_type::PLAIN)
      return false;
   auto plain_type = as_plain_type(type);
   switch (plain_type->system_type())
   {
   case blaise_ast_system_type::BYTE:
   case blaise_ast_system_type::SHORT:
   case blaise_ast_system_type::INTEGER:
   case blaise_ast_system_type::LONG:
   case blaise_ast_system_type::FLOAT:
   case blaise_ast_system_type::DOUBLE:
      return true;
   default:
      return false;
   }
}

bool blaise_ast_utility::is_integral_number(std::shared_ptr<blaise_ast_type> type)
{
   if (type->type_type() != ast::blaise_ast_type_type::PLAIN)
      return false;
   auto plain_type = as_plain_type(type);
   switch (plain_type->system_type())
   {
   case blaise_ast_system_type::BYTE:
   case blaise_ast_system_type::SHORT:
   case blaise_ast_system_type::INTEGER:
   case blaise_ast_system_type::LONG:
      return true;
   default:
      return false;
   }
}

bool blaise_ast_utility::is_array(std::shared_ptr<blaise_ast_type> type)
{
   return type->type_type() == ast::blaise_ast_type_type::ARRAY;
}

bool blaise_ast_utility::is_system_type(std::shared_ptr<blaise_ast_type> type, blaise_ast_system_type system_type)
{
   if (type->type_type() != ast::blaise_ast_type_type::PLAIN)
      return false;
   auto plain_type = as_plain_type(type);
   return system_type == plain_type->system_type();
}

bool blaise_ast_utility::is_boolean(std::shared_ptr<blaise_ast_type> type)
{
   return is_system_type(type, blaise_ast_system_type::BOOLEAN);
}

bool blaise_ast_utility::is_string(std::shared_ptr<blaise_ast_type> type)
{
   return is_system_type(type, blaise_ast_system_type::STRING);
}

bool blaise_ast_utility::is_char(std::shared_ptr<blaise_ast_type> type)
{
   return is_system_type(type, blaise_ast_system_type::CHAR);
}

bool blaise_ast_utility::is_long(std::shared_ptr<blaise_ast_type> type)
{
   return is_numeric(type, blaise_ast_system_type::LONG);
}

bool blaise_ast_utility::is_short(std::shared_ptr<blaise_ast_type> type)
{
   return is_numeric(type, blaise_ast_system_type::SHORT);
}

bool blaise_ast_utility::is_byte(std::shared_ptr<blaise_ast_type> type)
{
   return is_numeric(type, blaise_ast_system_type::BYTE);
}

uint8_t blaise_ast_utility::get_numeric_size(blaise_ast_system_type system_type)
{
   switch (system_type)
   {
   case blaise_ast_system_type::BYTE:
      return 1;
   case blaise_ast_system_type::SHORT:
      return 2;
   case blaise_ast_system_type::INTEGER:
      return 3;
   case blaise_ast_system_type::LONG:
      return 4;
   case blaise_ast_system_type::FLOAT:
      return 5;
   case blaise_ast_system_type::DOUBLE:
      return 6;
   default:
      throw std::runtime_error("Type is not a numeric type and cannot get the size");
   }
}

blaise_ast_system_type blaise_ast_utility::get_system_numeric_type_from_size(uint8_t size)
{
   switch (size)
   {
   case 1:
      return blaise_ast_system_type::BYTE;
   case 2:
      return blaise_ast_system_type::SHORT;
   case 3:
      return blaise_ast_system_type::INTEGER;
   case 4:
      return blaise_ast_system_type::LONG;
   case 5:
      return blaise_ast_system_type::FLOAT;
   case 6:
      return blaise_ast_system_type::DOUBLE;
   default:
      throw std::runtime_error("Invalid size");
   }
}

std::shared_ptr<blaise_ast_type> blaise_ast_utility::get_binary_numeric_result(std::shared_ptr<blaise_ast_type> left, std::shared_ptr<blaise_ast_type> right)
{
   if (!is_numeric(left))
      throw std::runtime_error("left operand is not a numeric type");
   if (!is_numeric(right))
      throw std::runtime_error("right operand is not a numeric type");

   auto system_type_left = as_plain_type(left)->system_type();
   auto system_type_right = as_plain_type(right)->system_type();

   auto left_size = get_numeric_size(system_type_left);
   auto right_size = get_numeric_size(system_type_right);
   auto max_size = std::max(left_size, right_size);
   auto max_type = get_system_numeric_type_from_size(max_size);

   return ast::make_plain_type(max_type);
}

std::shared_ptr<blaise_ast_type> blaise_ast_utility::get_binary_boolean_result(std::shared_ptr<blaise_ast_type> left, std::shared_ptr<blaise_ast_type> right)
{
   if (!is_boolean(left))
      throw std::runtime_error("left operand is not a boolean type");
   if (!is_boolean(right))
      throw std::runtime_error("right operand is not a boolean type");

   auto system_type_left = as_plain_type(left)->system_type();
   auto system_type_right = as_plain_type(right)->system_type();

   if (system_type_left == blaise_ast_system_type::BOOLEAN && system_type_right == blaise_ast_system_type::BOOLEAN)
      return ast::make_plain_type(blaise_ast_system_type::BOOLEAN);
   throw std::runtime_error("Unexpected runtime combination");
}

std::shared_ptr<blaise_ast_type> blaise_ast_utility::get_resulting_type(const sanelli::token<gasp::blaise::blaise_token_type> &reference, blaise_token_type op, std::shared_ptr<blaise_ast_type> operand)
{
   switch (op)
   {
   case blaise_token_type::LOGICAL_NOT:
   {
      if (is_boolean(operand))
         return ast::make_plain_type(blaise_ast_system_type::BOOLEAN);
   }
   break;
   case blaise_token_type::MINUS:
   {
      if (is_numeric(operand))
         return operand;
   }
   break;
   default:
      throw blaise_ast_error(reference.line(), reference.column(), sanelli::make_string("Unsupported unary operator ", op, " for expression of type ", operand));
   }

   throw blaise_ast_error(reference.line(), reference.column(), sanelli::make_string("Operator ", op, " does not support expression of type ", operand));
}

std::shared_ptr<blaise_ast_type> blaise_ast_utility::get_resulting_type(const token<blaise_token_type> &reference, blaise_token_type op, std::shared_ptr<blaise_ast_type> left, std::shared_ptr<blaise_ast_type> right)
{
   switch (op)
   {
   case blaise_token_type::LOGICAL_AND:
   case blaise_token_type::LOGICAL_OR:
   {
      if (is_boolean(left) && is_boolean(right))
         return get_binary_boolean_result(left, right);
   }
   break;
   case blaise_token_type::PLUS:
   case blaise_token_type::MINUS:
   case blaise_token_type::MULTIPLY:
   case blaise_token_type::DIVIDE:
   {
      if (is_numeric(left) && is_numeric(right))
         return get_binary_numeric_result(left, right);
   }
   break;
   case blaise_token_type::REMAINDER:
   {
      if (is_integral_number(left) && is_integral_number(right))
      {
         auto system_type_left = as_plain_type(left)->system_type();
         auto system_type_right = as_plain_type(right)->system_type();

         auto left_size = get_numeric_size(system_type_left);
         auto right_size = get_numeric_size(system_type_right);
         auto max_size = std::max(left_size, right_size);
         auto max_type = get_system_numeric_type_from_size(max_size);

         return make_plain_type(max_type);
      }
   }
   break;
   case blaise_token_type::GREAT_THAN:
   case blaise_token_type::LESS_THAN:
   case blaise_token_type::GREAT_THAN_OR_EQUAL_TO:
   case blaise_token_type::LESS_THAN_OR_EQUAL_TO:
   case blaise_token_type::EQUAL_TO:
   case blaise_token_type::NOT_EQUAL_TO:
   {
      if ((is_numeric(left) && is_numeric(right)) ||
          (is_char(left) && is_char(right)) ||
          (is_string(left) && is_string(right)) ||
          (is_boolean(left) && is_boolean(right)))
         return make_plain_type(blaise_ast_system_type::BOOLEAN);
   }
   break;
   default:
      throw blaise_ast_error(reference.line(), reference.column(), sanelli::make_string("Unsupported operator ", op, " between expression of type ", left, " and ", right));
   }

   throw blaise_ast_error(reference.line(), reference.column(), sanelli::make_string("Operator ", op, " between expression of type ", left, " and ", right, " is not allowed."));
}

bool blaise_ast_utility::can_auto_cast(std::shared_ptr<blaise_ast_type> from, std::shared_ptr<blaise_ast_type> to)
{
   if (is_numeric(from) && is_numeric(to))
   {
      auto system_type_from = as_plain_type(from)->system_type();
      auto system_type_to = as_plain_type(to)->system_type();
      switch (system_type_from)
      {
      case blaise_ast_system_type::BYTE:
         switch (system_type_to)
         {
         case blaise_ast_system_type::BYTE:
         case blaise_ast_system_type::SHORT:
         case blaise_ast_system_type::INTEGER:
         case blaise_ast_system_type::LONG:
         case blaise_ast_system_type::FLOAT:
         case blaise_ast_system_type::DOUBLE:
            return true;
         default:
            throw std::runtime_error(sanelli::make_string("Unsupported type '", to, "' passed as 'to' parameter"));
         }
      case blaise_ast_system_type::SHORT:
         switch (system_type_to)
         {
         case blaise_ast_system_type::BYTE:
            return false;
         case blaise_ast_system_type::SHORT:
         case blaise_ast_system_type::INTEGER:
         case blaise_ast_system_type::LONG:
         case blaise_ast_system_type::FLOAT:
         case blaise_ast_system_type::DOUBLE:
            return true;
         default:
            throw std::runtime_error(sanelli::make_string("Unsupported type '", to, "' passed as 'to' parameter"));
         }
      case blaise_ast_system_type::INTEGER:
         switch (system_type_to)
         {
         case blaise_ast_system_type::BYTE:
         case blaise_ast_system_type::SHORT:
            return false;
         case blaise_ast_system_type::INTEGER:
         case blaise_ast_system_type::LONG:
         case blaise_ast_system_type::FLOAT:
         case blaise_ast_system_type::DOUBLE:
            return true;
         default:
            throw std::runtime_error(sanelli::make_string("Unsupported type '", to, "' passed as 'to' parameter"));
         }
      case blaise_ast_system_type::LONG:
         switch (system_type_to)
         {
         case blaise_ast_system_type::BYTE:
         case blaise_ast_system_type::SHORT:
         case blaise_ast_system_type::INTEGER:
            return false;
         case blaise_ast_system_type::LONG:
         case blaise_ast_system_type::FLOAT:
         case blaise_ast_system_type::DOUBLE:
            return true;
         default:
            throw std::runtime_error(sanelli::make_string("Unsupported type '", to, "' passed as 'to' parameter"));
         }
      case blaise_ast_system_type::FLOAT:
         switch (system_type_to)
         {
         case blaise_ast_system_type::BYTE:
         case blaise_ast_system_type::SHORT:
         case blaise_ast_system_type::INTEGER:
         case blaise_ast_system_type::LONG:
            return false;
         case blaise_ast_system_type::FLOAT:
         case blaise_ast_system_type::DOUBLE:
            return true;
         default:
            throw std::runtime_error(sanelli::make_string("Unsupported type '", to, "' passed as 'to' parameter"));
         }
      case blaise_ast_system_type::DOUBLE:
         switch (system_type_to)
         {
         case blaise_ast_system_type::BYTE:
         case blaise_ast_system_type::SHORT:
         case blaise_ast_system_type::INTEGER:
         case blaise_ast_system_type::LONG:
         case blaise_ast_system_type::FLOAT:
            return false;
         case blaise_ast_system_type::DOUBLE:
            return true;
         default:
            throw std::runtime_error(sanelli::make_string("Unsupported type '", to, "' passed as 'to' parameter"));
         }
      default:
         throw std::runtime_error(sanelli::make_string("Unsupported type '", from, "' passed as 'from' parameter"));
      }
   }

   return false;
}

bool blaise_ast_utility::can_force_cast(std::shared_ptr<blaise_ast_type> from, std::shared_ptr<blaise_ast_type> to)
{
   // Same types I always can force a conversion
   if (is_numeric(from) && is_numeric(to))
      return true;
   if (is_boolean(from) && is_boolean(to))
      return true;
   if (is_char(from) && is_char(to))
      return true;
   if (is_string(from) && is_string(to))
      return true;

   // byte <--> boolean
   if (is_byte(from) && is_boolean(to))
      return true;
   if (is_boolean(from) && is_byte(to))
      return true;

   // short <--> boolean
   if (is_short(from) && is_boolean(to))
      return true;
   if (is_boolean(from) && is_short(to))
      return true;

   // integer <--> boolean
   if (is_integer(from) && is_boolean(to))
      return true;
   if (is_boolean(from) && is_integer(to))
      return true;

   // byte <--> char
   if (is_byte(from) && is_char(to))
      return true;
   if (is_char(from) && is_byte(to))
      return true;

   // short <--> char
   if (is_short(from) && is_char(to))
      return true;
   if (is_char(from) && is_short(to))
      return true;

   // integer <--> char
   if (is_integer(from) && is_char(to))
      return true;
   if (is_char(from) && is_integer(to))
      return true;

   return false;
}

std::shared_ptr<blaise_ast_type> blaise_ast_utility::get_common_type(const sanelli::token<gasp::blaise::blaise_token_type> &reference, std::shared_ptr<blaise_ast_type> typeA, std::shared_ptr<blaise_ast_type> typeB)
{
   if (typeA->equals(typeB))
      return typeA;
   if (can_auto_cast(typeA, typeB))
      return typeB;
   if (can_auto_cast(typeB, typeA))
      return typeA;
   throw blaise_ast_error(reference.line(), reference.column(), sanelli::make_string("Mismatch types '", typeA, "' and '", typeB, "'."));
}

bool blaise_ast_utility::is_allowed_for_constant(std::shared_ptr<gasp::blaise::ast::blaise_ast_expression> expression)
{
   switch (expression->expression_type())
   {
   case gasp::blaise::ast::blaise_ast_expression_type::LITERAL_BYTE:
   case gasp::blaise::ast::blaise_ast_expression_type::LITERAL_SHORT:
   case gasp::blaise::ast::blaise_ast_expression_type::LITERAL_INTEGER:
   case gasp::blaise::ast::blaise_ast_expression_type::LITERAL_LONG:
   case gasp::blaise::ast::blaise_ast_expression_type::LITERAL_FLOAT:
   case gasp::blaise::ast::blaise_ast_expression_type::LITERAL_DOUBLE:
   case gasp::blaise::ast::blaise_ast_expression_type::LITERAL_CHAR:
   case gasp::blaise::ast::blaise_ast_expression_type::LITERAL_BOOLEAN:
   case gasp::blaise::ast::blaise_ast_expression_type::LITERAL_STRING:
      return true;
   default:
      return false;
   }
}
