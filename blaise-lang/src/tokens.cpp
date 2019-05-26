#include <vector>

#include <gasp/common/internal_error.hpp>
#include <gasp/common/tokenizer.hpp>
#include <gasp/blaise/tokens.hpp>

using namespace gasp::blaise;
using namespace gasp::common;

blaise_token_provider::blaise_token_provider_constructor::blaise_token_provider_constructor()
{
   // KEYWORDS
   add_token(blaise_token_type::PROGRAM, "program", "PROGRAM");
   add_token(blaise_token_type::MODULE, "module", "MODULE");
   add_token(blaise_token_type::USES, "uses", "USES");
   add_token(blaise_token_type::BEGIN, "begin", "BEGIN");
   add_token(blaise_token_type::END, "end", "END");
   add_token(blaise_token_type::VAR, "var", "VAR");
   add_token(blaise_token_type::LOGICAL_AND, "and", "AND");
   add_token(blaise_token_type::LOGICAL_OR, "or", "OR");
   add_token(blaise_token_type::LOGICAL_NOT, "not", "NOT");
   add_token(blaise_token_type::LOGICAL_EAGER, "eager", "EAGER");

   // TYPES
   add_token(blaise_token_type::TYPE_BYTE, "byte", "TYPE_BYTE");
   add_token(blaise_token_type::TYPE_INTEGER, "integer", "TYPE_INTEGER");
   add_token(blaise_token_type::TYPE_LONG, "long", "TYPE_LONG");
   add_token(blaise_token_type::UNSIGNED, "unsigned", "UNSIGNED");
   add_token(blaise_token_type::TYPE_CHAR, "char", "TYPE_CHAR");
   add_token(blaise_token_type::TYPE_STRING, "string", "TYPE_STRING");
   add_token(blaise_token_type::TYPE_FLOAT, "float", "TYPE_FLOAT");
   add_token(blaise_token_type::TYPE_DOUBLE, "double", "TYPE_DOUBLE");
   add_token(blaise_token_type::TYPE_BOOLEAN, "boolean", "TYPE_BOOLEAN");

   // PUNCTUATION
   add_token(blaise_token_type::ASSIGNMENT, ":=", "ASSIGNMENT");
   add_token(blaise_token_type::SEMICOLON, ";", "SEMICOLON");
   add_token(blaise_token_type::PERIOD, "\\.", "PERIOD");
   add_token(blaise_token_type::COLON, "\\:", "COLON");
   add_token(blaise_token_type::COMMA, "\\,", "COMMA");
   add_token(blaise_token_type::LEFT_PARENTHESES, "\\(", "LEFT_PARENTHESES");
   add_token(blaise_token_type::RIGHT_PARENTHESES, "\\)", "RIGHT_PARENTHESES");
   add_token(blaise_token_type::NOT_EQUAL_TO, "\\<\\>", "NOT_EQUAL_TO");
   add_token(blaise_token_type::EQUAL_TO, "\\=\\=", "EQUAL_TO");
   add_token(blaise_token_type::GREAT_THAN, "\\>", "GREAT_THAN");
   add_token(blaise_token_type::LESS_THAN, "\\<", "GREAT_THAN");
   add_token(blaise_token_type::GREAT_THAN_OR_EQUAL_TO, "\\>\\=", "GREAT_THAN_OR_EQUAL_TO");
   add_token(blaise_token_type::LESS_THAN_OR_EQUAL_TO, "\\<\\=", "LESS_THAN_OR_EQUAL_TO");
   add_token(blaise_token_type::PLUS, "\\+", "PLUS");
   add_token(blaise_token_type::MINUS, "\\-", "MINUS");
   add_token(blaise_token_type::MULTIPLY, "\\*", "MULTIPLY");
   add_token(blaise_token_type::DIVIDE, "\\/", "DIVIDE");
   add_token(blaise_token_type::REMAINDER, "\\%", "REMAINDER");

   // LITERALS
   add_token(blaise_token_type::INTEGER_BASE_TWO, "0b[0-1]+", "INTEGER_BASE_TWO", true);
   add_token(blaise_token_type::INTEGER_BASE_EIGHT, "0o[0-7]+", "INTEGER_BASE_EIGHT", true);
   add_token(blaise_token_type::INTEGER_BASE_SIXTEEN, "0x[0-9A-Fa-f]+", "INTEGER_BASE_SIXTEEN", true);
   add_token(blaise_token_type::NUMBER, "[0-9]+(?:\\.[0-9]+)?", "NUMBER", true);
   add_token(blaise_token_type::STRING_LITERAL, "\"(?:[^\"]|\\\\.)*\"", "STIRNG_LITERAL", true);
   add_token(blaise_token_type::CHAR_LITERAL, "'(?:[^']|\\\\.)'", "CHAR_LITERAL", true);
   add_token(blaise_token_type::IDENTIFIER, "[a-zA-Z][a-zA-Z0-9_]*", "IDENTIFIER", true);
}

blaise_token_provider::blaise_token_provider_constructor blaise_token_provider::_private;
std::vector<blaise_token_type>::const_iterator blaise_token_provider::cbegin() { return _private.cbegin(); }
std::vector<blaise_token_type>::const_iterator blaise_token_provider::cend() { return _private.cend(); }

std::string blaise_token_provider::rule(blaise_token_type token_type) { return _private.rule(token_type); }
std::string blaise_token_provider::name(blaise_token_type token_type) { return _private.name(token_type); }
bool blaise_token_provider::keep_value(blaise_token_type token_type) { return _private.keep_value(token_type); }
bool blaise_token_provider::keep_token(blaise_token_type token_type) { return _private.keep_token(token_type); }

bool blaise_token_utility::is_type(blaise_token_type token_type)
{
   switch (token_type)
   {
   case blaise_token_type::TYPE_BYTE:
   case blaise_token_type::TYPE_INTEGER:
   case blaise_token_type::TYPE_LONG:
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

bool blaise_token_utility::is_unsigned_type(blaise_token_type token_type)
{
   switch (token_type)
   {
   case blaise_token_type::TYPE_BYTE:
   case blaise_token_type::TYPE_INTEGER:
   case blaise_token_type::TYPE_LONG:
      return true;
   default:
      return false;
   };
}

bool blaise_token_utility::is_operator(blaise_token_type token_type){
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
      case blaise_token_type::LOGICAL_OR:
         return true;
      default:
         return false;
   }
}

bool blaise_token_utility::get_operator_precedence(blaise_token_type token_type){
   switch(token_type){
      case blaise_token_type::LOGICAL_NOT:
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
      default:
         throw gasp::common::tokenizer_error(0,0,"unexpected token type - it is not an operator");
   }
}

bool blaise_token_utility::is_number(blaise_token_type token_type){
   switch(token_type){
      case blaise_token_type::NUMBER:
      case blaise_token_type::INTEGER_BASE_TWO:
      case blaise_token_type::INTEGER_BASE_EIGHT:
      case blaise_token_type::INTEGER_BASE_SIXTEEN:
         return true;
      default:
         return false;
   }
}

std::ostream &gasp::blaise::operator<<(std::ostream &os, const blaise_token_type &token_type)
{
   return os << std::string(blaise_token_provider::name(token_type));
}