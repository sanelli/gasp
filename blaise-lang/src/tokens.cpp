#include <vector>

#include <gasp/common/internal_error.hpp>
#include <gasp/common/tokenizer.hpp>
#include <gasp/blaise/tokens.hpp>

using namespace gasp::blaise;
using namespace gasp::common;

blaise_token_provider::blaise_token_provider_constructor::blaise_token_provider_constructor()
{
   add_token(blaise_token::NUMBER, "[0-9]+(?:\\.[0-9]+)", "NUMBER", true);

   // KEYWORDS
   add_token(blaise_token::PROGRAM, "program", "PROGRAM");
   add_token(blaise_token::MODULE, "module", "MODULE");
   add_token(blaise_token::USES, "uses", "USES");
   add_token(blaise_token::BEGIN, "begin", "BEGIN");
   add_token(blaise_token::END, "end", "END");
   add_token(blaise_token::VAR, "var", "VAR");
   add_token(blaise_token::LOGICAL_AND, "and", "AND");
   add_token(blaise_token::LOGICAL_OR, "or", "OR");
   add_token(blaise_token::LOGICAL_NOT, "not", "NOT");
   add_token(blaise_token::LOGICAL_EAGER, "eager", "EAGER");

   // TYPES
   add_token(blaise_token::TYPE_BYTE, "byte", "TYPE_BYTE");
   add_token(blaise_token::TYPE_INTEGER, "integer", "TYPE_INTEGER");
   add_token(blaise_token::TYPE_LONG, "long", "TYPE_LONG");
   add_token(blaise_token::UNSIGNED, "unsigned", "UNSIGNED");
   add_token(blaise_token::TYPE_CHAR, "char", "TYPE_CHAR");
   add_token(blaise_token::TYPE_STRING, "string", "TYPE_STRING");
   add_token(blaise_token::TYPE_FLOAT, "float", "TYPE_FLOAT");
   add_token(blaise_token::TYPE_DOUBLE, "double", "TYPE_DOUBLE");
   add_token(blaise_token::TYPE_BOOLEAN, "boolean", "TYPE_BOOLEAN");

   // PUNCTUATION
   add_token(blaise_token::ASSIGNMENT, ":=", "ASSIGNMENT");
   add_token(blaise_token::SEMICOLON, ";", "SEMICOLON");
   add_token(blaise_token::PERIOD, "\\.", "PERIOD");
   add_token(blaise_token::COLON, "\\:", "COLON");
   add_token(blaise_token::COMMA, "\\,", "COMMA");
   add_token(blaise_token::LEFT_PARENTHESES, "\\(", "LEFT_PARENTHESES");
   add_token(blaise_token::RIGHT_PARENTHESES, "\\)", "RIGHT_PARENTHESES");
   add_token(blaise_token::NOT_EQUAL_TO, "\\<\\>", "NOT_EQUAL_TO");
   add_token(blaise_token::EQUAL_TO, "\\=\\=", "EQUAL_TO");
   add_token(blaise_token::GREAT_THAN, "\\>", "GREAT_THAN");
   add_token(blaise_token::LESS_THAN, "\\<", "GREAT_THAN");
   add_token(blaise_token::GREAT_THAN_OR_EQUAL_TO, "\\>\\=", "GREAT_THAN_OR_EQUAL_TO");
   add_token(blaise_token::LESS_THAN_OR_EQUAL_TO, "\\<\\=", "LESS_THAN_OR_EQUAL_TO");
   add_token(blaise_token::PLUS, "\\+", "PLUS");
   add_token(blaise_token::MINUS, "\\-", "MINUS");
   add_token(blaise_token::MULTIPLY, "\\*", "MULTIPLY");
   add_token(blaise_token::DIVIDE, "\\/", "DIVIDE");
   add_token(blaise_token::REMAINDER, "\\%", "REMAINDER");

   // OTHERS
   add_token(blaise_token::IDENTIFIER, "[a-zA-Z][a-zA-Z0-9_]*", "IDENTIFIER", true);
}

blaise_token_provider::blaise_token_provider_constructor blaise_token_provider::_private;
std::vector<blaise_token>::const_iterator blaise_token_provider::cbegin() { return _private.cbegin(); }
std::vector<blaise_token>::const_iterator blaise_token_provider::cend() { return _private.cend(); }

std::string blaise_token_provider::rule(blaise_token token_type) { return _private.rule(token_type); }
std::string blaise_token_provider::name(blaise_token token_type) { return _private.name(token_type); }
bool blaise_token_provider::keep_value(blaise_token token_type) { return _private.keep_value(token_type); }
bool blaise_token_provider::keep_token(blaise_token token_type) { return _private.keep_token(token_type); }

bool blaise_token_utility::is_type(blaise_token token_type)
{
   switch (token_type)
   {
   case blaise_token::TYPE_BYTE:
   case blaise_token::TYPE_INTEGER:
   case blaise_token::TYPE_LONG:
   case blaise_token::TYPE_CHAR:
   case blaise_token::TYPE_STRING:
   case blaise_token::TYPE_FLOAT:
   case blaise_token::TYPE_DOUBLE:
   case blaise_token::TYPE_BOOLEAN:
      return true;
   default:
      return false;
   };
}

bool blaise_token_utility::is_unsigned_type(blaise_token token_type)
{
   switch (token_type)
   {
   case blaise_token::TYPE_BYTE:
   case blaise_token::TYPE_INTEGER:
   case blaise_token::TYPE_LONG:
      return true;
   default:
      return false;
   };
}

bool blaise_token_utility::is_operator(blaise_token token_type){
   switch(token_type){
      case blaise_token::LOGICAL_NOT:
      case blaise_token::MULTIPLY:
      case blaise_token::DIVIDE:
      case blaise_token::REMAINDER:
      case blaise_token::PLUS:
      case blaise_token::MINUS:
      case blaise_token::GREAT_THAN_OR_EQUAL_TO:
      case blaise_token::LESS_THAN_OR_EQUAL_TO:
      case blaise_token::GREAT_THAN:
      case blaise_token::LESS_THAN:
      case blaise_token::EQUAL_TO:
      case blaise_token::NOT_EQUAL_TO:
      case blaise_token::LOGICAL_AND:
      case blaise_token::LOGICAL_OR:
         return true;
      default:
         return false;
   }
}

bool blaise_token_utility::get_operator_precedence(blaise_token token_type){
   switch(token_type){
      case blaise_token::LOGICAL_NOT:
         return 100;
      case blaise_token::MULTIPLY:
      case blaise_token::DIVIDE:
      case blaise_token::REMAINDER:
         return 90;
      case blaise_token::PLUS:
      case blaise_token::MINUS:
         return 80;
      case blaise_token::GREAT_THAN_OR_EQUAL_TO:
      case blaise_token::LESS_THAN_OR_EQUAL_TO:
      case blaise_token::GREAT_THAN:
      case blaise_token::LESS_THAN:
         return 70;
      case blaise_token::EQUAL_TO:
      case blaise_token::NOT_EQUAL_TO:
         return 60;
      case blaise_token::LOGICAL_AND:
         return 50;
      case blaise_token::LOGICAL_OR:
         return 40;
      default:
         throw gasp::common::tokenizer_error(0,0,"unexpected token type - it is not an operator");
   }
}

std::ostream &gasp::blaise::operator<<(std::ostream &os, const blaise_token &token_type)
{
   return os << std::string(blaise_token_provider::name(token_type));
}