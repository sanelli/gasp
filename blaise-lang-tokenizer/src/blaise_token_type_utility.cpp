#include <sanelli/sanelli.hpp>
#include <gasp/blaise/tokenizer/tokenizer.hpp>

using namespace gasp::blaise;

bool blaise_token_type_utility::is_type(blaise_token_type token_type)
{
   switch (token_type)
   {
   case blaise_token_type::TYPE_BYTE:
   case blaise_token_type::TYPE_SHORT:
   case blaise_token_type::TYPE_INTEGER:
   case blaise_token_type::TYPE_LONG:
   case blaise_token_type::TYPE_CHAR:
   case blaise_token_type::TYPE_FLOAT:
   case blaise_token_type::TYPE_DOUBLE:
   case blaise_token_type::TYPE_BOOLEAN:
   case blaise_token_type::TYPE_ARRAY:
      return true;
   default:
      return false;
   };
}

bool blaise_token_type_utility::is_unsigned_type(blaise_token_type token_type)
{
   switch (token_type)
   {
   case blaise_token_type::TYPE_BYTE:
   case blaise_token_type::TYPE_INTEGER:
   case blaise_token_type::TYPE_LONG:
   case blaise_token_type::TYPE_SHORT:
      return true;
   default:
      return false;
   };
}

bool blaise_token_type_utility::is_operator(blaise_token_type token_type)
{
   switch (token_type)
   {
   case blaise_token_type::NEW:
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
   case blaise_token_type::LOGICAL_OR:
      return true;
   default:
      return false;
   }
}

// Loosly following https://it.cppreference.com/w/cpp/language/operator_precedence
uint8_t blaise_token_type_utility::get_operator_precedence(blaise_token_type token_type)
{
   switch (token_type)
   {
   case blaise_token_type::NEW:
      return 120;
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
      return 50;
   case blaise_token_type::LOGICAL_OR:
      return 40;
   case blaise_token_type::IF: // Terary
      return 30;
   default:
      throw sanelli::tokenizer_error(0, 0, sanelli::make_string("unexpected token with type '", token_type, "': it is not a supported operator"));
   }
}

bool blaise_token_type_utility::is_number(blaise_token_type token_type)
{
   switch (token_type)
   {
   case blaise_token_type::LITERAL_BYTE:
   case blaise_token_type::LITERAL_SHORT:
   case blaise_token_type::LITERAL_INTEGER:
   case blaise_token_type::LITERAL_LONG:
   case blaise_token_type::LITERAL_DOUBLE:
   case blaise_token_type::LITERAL_FLOAT:
   case blaise_token_type::LITERAL_INTEGER_BINARY:
   case blaise_token_type::LITERAL_INTEGER_OCTAL:
   case blaise_token_type::LITERAL_INTEGER_HEX:
      return true;
   default:
      return false;
   }
}

bool blaise_token_type_utility::is_boolean(blaise_token_type token_type)
{
   switch (token_type)
   {
   case blaise_token_type::LITERAL_BOOLEAN:
      return true;
   default:
      return false;
   }
}
