#include <string>

#include <gasp/blaise/tokens.hpp>

using namespace gasp::blaise;
using namespace gasp::common;

blaise_token_type_provider::blaise_token_provider_constructor::blaise_token_provider_constructor()
{
   // TODO: Allow CHAR_LITERAL to be like java unicode sequence '\u0F8a'
   // TODO: STRING_LITERAL not accept escape \""
   // TODO: STRING_LITERAL value do not  \""
   // TODO: IDENTIFIER is not matches for "false_" or "true_" (they are treated like a boolean + identifier)

   // KEYWORDS
   add_token(blaise_token_type::PROGRAM, "program", "PROGRAM");
   add_token(blaise_token_type::MODULE, "module", "MODULE");
   add_token(blaise_token_type::USES, "uses", "USES");
   add_token(blaise_token_type::BEGIN, "begin", "BEGIN");
   add_token(blaise_token_type::END, "end", "END");
   add_token(blaise_token_type::VAR, "var", "VAR");
   add_token(blaise_token_type::LOGICAL_AND, "and", "LOGICAL_AND");
   add_token(blaise_token_type::LOGICAL_OR, "or", "LOGICAL_OR");
   add_token(blaise_token_type::LOGICAL_NOT, "not", "LOGICAL_NOT");
   add_token(blaise_token_type::LOGICAL_EAGER_AND, "eager\\s+and", "LOGICAL_EAGER_AND");
   add_token(blaise_token_type::LOGICAL_EAGER_OR, "eager\\s+or", "LOGICAL_EAGER_OR");
   add_token(blaise_token_type::FUNCTION, "function", "FUNCTION");
   add_token(blaise_token_type::PROCEDURE, "procedure", "PROCEDURE");
   add_token(blaise_token_type::NATIVE, "native", "NATIVE");

   // TYPES
   add_token(blaise_token_type::TYPE_INTEGER, "integer", "TYPE_INTEGER");
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
   add_token(blaise_token_type::INTEGER_BASE_TWO_LITERAL, "0b[0-1]+", "INTEGER_BASE_TWO_LITERAL", true);
   add_token(blaise_token_type::INTEGER_BASE_EIGHT_LITERAL, "0o[0-7]+", "INTEGER_BASE_EIGHT_LITERAL", true);
   add_token(blaise_token_type::INTEGER_BASE_SIXTEEN_LITERAL, "0x[0-9A-Fa-f]+", "INTEGER_BASE_SIXTEEN_LITERAL", true);
   add_token(blaise_token_type::FLOAT_LITERAL, "[0-9]+\\.[0-9]+f", "FLOAT_LITERAL", true);
   add_token(blaise_token_type::DOUBLE_LITERAL, "[0-9]+\\.[0-9]+d?", "DOUBLE_LITERAL", true);
   add_token(blaise_token_type::INTEGER_LITERAL, "[0-9]+", "INTEGER_LITERAL", true);
   add_token(blaise_token_type::STRING_LITERAL, "\"(?:[^\"]|\\\\.|\\\\u[0-9a-fA-f]{4})*\"", "STIRNG_LITERAL", true);
   add_token(blaise_token_type::CHAR_LITERAL, "'(?:[^']|\\\\.|\\\\u[0-9a-fA-f]{4})'", "CHAR_LITERAL", true);
   add_token(blaise_token_type::BOOLEAN_LITERAL, "true|false", "BOOLEAN_LITERAL", true);
   add_token(blaise_token_type::IDENTIFIER, "[a-zA-Z_][a-zA-Z0-9_]*", "IDENTIFIER", true);
}

blaise_token_type_provider::blaise_token_provider_constructor blaise_token_type_provider::_private;
std::vector<blaise_token_type>::const_iterator blaise_token_type_provider::cbegin() { return _private.cbegin(); }
std::vector<blaise_token_type>::const_iterator blaise_token_type_provider::cend() { return _private.cend(); }

std::string blaise_token_type_provider::rule(blaise_token_type token_type) { return _private.rule(token_type); }
std::string blaise_token_type_provider::name(blaise_token_type token_type) { return _private.name(token_type); }
bool blaise_token_type_provider::keep_value(blaise_token_type token_type) { return _private.keep_value(token_type); }
bool blaise_token_type_provider::keep_token(blaise_token_type token_type) { return _private.keep_token(token_type); }