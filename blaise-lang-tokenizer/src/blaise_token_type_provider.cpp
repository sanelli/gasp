#include <string>

#include <sanelli/sanelli.hpp>
#include <gasp/blaise/tokenizer/tokenizer.hpp>

using namespace gasp::blaise;
using namespace sanelli;

blaise_token_type_provider::blaise_token_provider_constructor::blaise_token_provider_constructor()
{
   // KEYWORDS
   add_token(blaise_token_type::PROGRAM, "program", "PROGRAM");
   add_token(blaise_token_type::MODULE, "module", "MODULE");
   add_token(blaise_token_type::USE, "use", "USE");
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
   add_token(blaise_token_type::CAST, "cast", "CAST");
   add_token(blaise_token_type::IF, "if", "IF");
   add_token(blaise_token_type::THEN, "then", "THEN");
   add_token(blaise_token_type::ELSE, "else", "ELSE");
   add_token(blaise_token_type::DO, "do", "DO");
   add_token(blaise_token_type::WHILE, "while", "WHILE");
   add_token(blaise_token_type::REPEAT, "repeat", "REPEAT");
   add_token(blaise_token_type::UNTIL, "until", "UNTIL");
   add_token(blaise_token_type::FOR, "for", "FOR");
   add_token(blaise_token_type::FROM, "from", "FROM");
   add_token(blaise_token_type::TO, "to", "TO");
   add_token(blaise_token_type::STEP, "step", "STEP");
   add_token(blaise_token_type::NEW, "new", "NEW");
   add_token(blaise_token_type::DELETE, "delete", "DELETE");

   // TYPES
   add_token(blaise_token_type::TYPE_INTEGER, "integer", "TYPE_INTEGER");
   add_token(blaise_token_type::TYPE_LONG, "long", "TYPE_LONG");
   add_token(blaise_token_type::TYPE_SHORT, "short", "TYPE_SHORT");
   add_token(blaise_token_type::TYPE_BYTE, "byte", "TYPE_BYTE");
   add_token(blaise_token_type::TYPE_CHAR, "char", "TYPE_CHAR");
   add_token(blaise_token_type::TYPE_STRING, "string", "TYPE_STRING");
   add_token(blaise_token_type::TYPE_FLOAT, "float", "TYPE_FLOAT");
   add_token(blaise_token_type::TYPE_DOUBLE, "double", "TYPE_DOUBLE");
   add_token(blaise_token_type::TYPE_BOOLEAN, "boolean", "TYPE_BOOLEAN");
   add_token(blaise_token_type::TYPE_ARRAY, "array", "TYPE_ARRAY");

   // PUNCTUATION
   add_token(blaise_token_type::ASSIGNMENT, ":=", "ASSIGNMENT", false, true, true);
   add_token(blaise_token_type::SEMICOLON, ";", "SEMICOLON", false, true, true);
   add_token(blaise_token_type::PERIOD, "\\.", "PERIOD", false, true, true);
   add_token(blaise_token_type::COLON, "\\:", "COLON", false, true, true);
   add_token(blaise_token_type::COMMA, "\\,", "COMMA", false, true, true);
   add_token(blaise_token_type::LEFT_PARENTHESES, "\\(", "LEFT_PARENTHESES", false, true, true);
   add_token(blaise_token_type::RIGHT_PARENTHESES, "\\)", "RIGHT_PARENTHESES", false, true, true);
   add_token(blaise_token_type::LEFT_BRACKET, "\\[", "LEFT_BRACKET", false, true, true);
   add_token(blaise_token_type::RIGHT_BRACKET, "\\]", "RIGHT_BRACKET", false, true, true);
   add_token(blaise_token_type::NOT_EQUAL_TO, "\\<\\>", "NOT_EQUAL_TO", false, true, true);
   add_token(blaise_token_type::EQUAL_TO, "\\=\\=", "EQUAL_TO", false, true, true);
   add_token(blaise_token_type::GREAT_THAN_OR_EQUAL_TO, "\\>\\=", "GREAT_THAN_OR_EQUAL_TO", false, true, true);
   add_token(blaise_token_type::LESS_THAN_OR_EQUAL_TO, "\\<\\=", "LESS_THAN_OR_EQUAL_TO", false, true, true);
   add_token(blaise_token_type::GREAT_THAN, "\\>", "GREAT_THAN", false, true, true);
   add_token(blaise_token_type::LESS_THAN, "\\<", "GREAT_THAN", false, true, true);
   add_token(blaise_token_type::PLUS, "\\+", "PLUS", false, true, true);
   add_token(blaise_token_type::MINUS, "\\-", "MINUS", false, true, true);
   add_token(blaise_token_type::MULTIPLY, "\\*", "MULTIPLY", false, true, true);
   add_token(blaise_token_type::DIVIDE, "\\/", "DIVIDE", false, true, true);
   add_token(blaise_token_type::REMAINDER, "\\%", "REMAINDER", false, true, true);

   // LITERALS
   add_token(blaise_token_type::LITERAL_INTEGER_BINARY, "0b[0-1]+", "LITERAL_INTEGER_BINARY", true);
   add_token(blaise_token_type::LITERAL_INTEGER_OCTAL, "0o[0-7]+", "LITERAL_INTEGER_OCTAL", true);
   add_token(blaise_token_type::LITERAL_INTEGER_HEX, "0x[0-9A-Fa-f]+", "LITERAL_INTEGER_HEX", true);
   add_token(blaise_token_type::LITERAL_FLOAT, "[0-9]+\\.[0-9]+(f|F)", "LITERAL_FLOAT", true);
   add_token(blaise_token_type::LITERAL_DOUBLE, "[0-9]+\\.[0-9]+(d|D)?", "LITERAL_DOUBLE", true);
   add_token(blaise_token_type::LITERAL_LONG, "[0-9]+(l|L)", "LITERAL_LONG", true);
   add_token(blaise_token_type::LITERAL_SHORT, "[0-9]+(s|S)", "LITERAL_SHORT", true);
   add_token(blaise_token_type::LITERAL_BYTE, "[0-9]+(b|B)", "LITERAL_BYTE", true);
   add_token(blaise_token_type::LITERAL_INTEGER, "[0-9]+", "LITERAL_INTEGER", true);
   add_token(blaise_token_type::LITERAL_STRING, "\"(?:[^\"\\\\]|\\\\(?:a|b|n|t|0|r|f|\")|\\\\u[0-9a-fA-F]{2})*\"", "STIRNG_LITERAL", true, true, true);
   add_token(blaise_token_type::LITERAL_CHAR, "'(?:[^'\\\\]|\\\\(?:a|b|n|t|0|r|f|')|\\\\u[0-9a-fA-F]{2})'", "LITERAL_CHAR", true, true, true);
   add_token(blaise_token_type::LITERAL_BOOLEAN, "true|false", "LITERAL_BOOLEAN", true);
   add_token(blaise_token_type::IDENTIFIER, "[a-zA-Z_][a-zA-Z0-9_]*", "IDENTIFIER", true);
}

blaise_token_type_provider::blaise_token_provider_constructor blaise_token_type_provider::_private;
std::vector<blaise_token_type>::const_iterator blaise_token_type_provider::cbegin() { return _private.cbegin(); }
std::vector<blaise_token_type>::const_iterator blaise_token_type_provider::cend() { return _private.cend(); }

std::string blaise_token_type_provider::rule(blaise_token_type token_type) { return _private.rule(token_type); }
std::string blaise_token_type_provider::name(blaise_token_type token_type) { return _private.name(token_type); }
bool blaise_token_type_provider::keep_value(blaise_token_type token_type) { return _private.keep_value(token_type); }
bool blaise_token_type_provider::keep_token(blaise_token_type token_type) { return _private.keep_token(token_type); }
bool blaise_token_type_provider::is_punctuation(blaise_token_type token_type) { return _private.is_punctuation(token_type); }
