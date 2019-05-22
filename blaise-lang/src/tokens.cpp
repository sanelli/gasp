#include <vector>

#include <gasp/common/internal_error.hpp>
#include <gasp/common/tokenizer.hpp>
#include <gasp/blaise/tokens.hpp>

using namespace gasp::blaise;
using namespace gasp::common;

blaise_token_provider::blaise_token_provider_constructor::blaise_token_provider_constructor()
{
   // PREPROCESSOR
   add_token(blaise_token::PREPROCESSOR_IF, "#if", "PREPROCESSOR_IF");
   add_token(blaise_token::PREPROCESSOR_ELSE, "#else", "PREPROCESSOR_ELSE");
   add_token(blaise_token::PREPROCESSOR_END, "#end", "PREPROCESSOR_END");
   add_token(blaise_token::PREPROCESSOR_DEFINE, "#define", "PREPROCESSOR_DEFINE");
   add_token(blaise_token::PREPROCESSOR_UNDEFINE, "#undefine", "PREPROCESSOR_UNDEFINE");
   add_token(blaise_token::PREPROCESSOR_DEFINED, "#defined", "PREPROCESSOR_DEFINED");
   add_token(blaise_token::PREPROCESSOR_VERSION, "#version", "PREPROCESSOR_VERSION");

   // KEYWORDS
   add_token(blaise_token::PROGRAM, "program", "PROGRAM");
   add_token(blaise_token::MODULE, "module", "MODULE");
   add_token(blaise_token::USES, "uses", "USES");
   add_token(blaise_token::BEGIN, "begin", "BEGIN");
   add_token(blaise_token::END, "end", "END");
   add_token(blaise_token::VAR, "var", "VAR");
   add_token(blaise_token::FUNCTION, "function", "FUNCTION");
   add_token(blaise_token::PROCEDURE, "procedure", "PROCEDURE");
   add_token(blaise_token::LAMBDA, "lambda", "LAMBDA");
   add_token(blaise_token::IN, "in", "IN");
   add_token(blaise_token::OUT, "out", "OUT");
   add_token(blaise_token::INOUT, "inout", "INOUT");
   add_token(blaise_token::RETURN, "return", "RETURN");
   add_token(blaise_token::IF, "if", "IF");
   add_token(blaise_token::THEN, "then", "THEN");
   add_token(blaise_token::ELSE, "else", "ELSE");
   add_token(blaise_token::ELIF, "elif", "ELIF");
   add_token(blaise_token::WHILE, "while", "WHILE");
   add_token(blaise_token::DO, "do", "DO");
   add_token(blaise_token::UNTIL, "untile", "UNTIL");
   add_token(blaise_token::SWITCH, "switch", "SWITCH");
   add_token(blaise_token::CASE, "case", "CASE");
   add_token(blaise_token::FOR, "for", "FOR");
   add_token(blaise_token::FOREACH, "foreach", "FOREACH");
   add_token(blaise_token::CONTINUE, "continue", "CONTINUE");
   add_token(blaise_token::BREAK, "break", "BREAK");
   add_token(blaise_token::AND, "and", "AND");
   add_token(blaise_token::OR, "or", "OR");
   add_token(blaise_token::NOT, "not", "NOT");
   add_token(blaise_token::XOR, "xor", "XOR");
   add_token(blaise_token::SHIFT, "shift", "SHIFT");
   add_token(blaise_token::RIGHT, "right", "RIGHT");
   add_token(blaise_token::RIGHT, "left", "LEFT");
   add_token(blaise_token::EAGER, "eager", "EAGER");
   add_token(blaise_token::LAZY, "lazy", "LAZY");
   add_token(blaise_token::STATIC_CAST, "static_cast", "STATIC_CAST");
   add_token(blaise_token::CONST, "const", "CONST");
   add_token(blaise_token::TYPE, "type", "TYPE");
   add_token(blaise_token::INTERFACE, "interface", "INTERFACE");
   add_token(blaise_token::GENERIC, "generic", "GENERIC");
   add_token(blaise_token::ABSTRACT, "abstract", "ABSTRACT");
   add_token(blaise_token::CONSTRUCTOR, "constructor", "TYPE");
   add_token(blaise_token::DESTRUCTOR, "destructor", "TYPE");
   add_token(blaise_token::PUBLIC, "public", "PUBLIC");
   add_token(blaise_token::PRIVATE, "private", "PRIVATE");
   add_token(blaise_token::PROTECTED, "protected", "PROTECTED");
   add_token(blaise_token::STATIC, "static", "STATIC");
   add_token(blaise_token::VIRTUAL, "virtual", "VIRTUAL");
   add_token(blaise_token::OVERRIDE, "override", "OVERRIDE");
   add_token(blaise_token::INHERIT, "inherit", "INHERIT");
   add_token(blaise_token::IMPLEMENT, "implement", "IMPLEMENT");
   add_token(blaise_token::IMPLEMENTATION, "implementation", "IMPLEMENTATION");
   add_token(blaise_token::TRY, "try", "TRY");
   add_token(blaise_token::CATCH, "catch", "CATCH");
   add_token(blaise_token::FINALLY, "finally", "FINALLY");
   add_token(blaise_token::THROW, "throw", "THROW");
   add_token(blaise_token::UNION, "union", "UNION");
   add_token(blaise_token::ENUM, "enum", "ENUM");
   add_token(blaise_token::NAMEOF, "nameof", "NAMEOF");
   add_token(blaise_token::TYPEOF, "typeof", "TYPEOF");
   add_token(blaise_token::ADDRESSOF, "addressof", "ADDRESSOF");
   add_token(blaise_token::NATIVE, "native", "NATIVE");
   add_token(blaise_token::REQUIRE, "require", "REQUIRE");
   add_token(blaise_token::ENSURE, "ensure", "ENSURE");
   add_token(blaise_token::ASSERT, "assert", "ASSERT");
   add_token(blaise_token::ASSEMBLY, "assembly", "ASSEMBLY");
   add_token(blaise_token::ASYNC, "async", "ASYNC");
   add_token(blaise_token::AWAIT, "await", "AWAIT");

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
   add_token(blaise_token::TYPE_COMPLEX, "complex", "TYPE_COMPLEX");
   add_token(blaise_token::TYPE_ARRAY, "array", "TYPE_ARRAY");
   add_token(blaise_token::OF, "of", "OF");
   add_token(blaise_token::CONST_TRUE, "true", "CONST_TRUE");
   add_token(blaise_token::CONST_FALSE, "false", "CONST_FALSE");
   add_token(blaise_token::POINTER, "pointer", "POINTER");
   add_token(blaise_token::NEW, "new", "NEW");
   add_token(blaise_token::DELETE, "delete", "DELETE");
   add_token(blaise_token::NULLPTR, "nullptr", "NULLPTR");
   add_token(blaise_token::TYPE_TUPLE, "tuple", "TYPE_TUPLE");
   add_token(blaise_token::TYPE_SET, "set", "TYPE_SET");
   add_token(blaise_token::TYPE_DICTIONARY, "dictionaty", "TYPE_DICTIONARY");
   add_token(blaise_token::ALIAS, "alias", "ALIAS");

   // PUNCTUATION
   add_token(blaise_token::ASSIGN, ":=", "ASSIGN");
   add_token(blaise_token::SEMICOLON, ";", "SEMICOLON");
   add_token(blaise_token::RANGE, "\\.\\.", "RANGE");
   add_token(blaise_token::PERIOD, "\\.", "PERIOD");
   add_token(blaise_token::COLON, "\\:", "COLON");
   add_token(blaise_token::LEFT_BRAKET, "\\(", "LEFT_BRAKET");
   add_token(blaise_token::RIGHT_BRAKET, "\\)", "RIGHT_BRAKET");
   add_token(blaise_token::LEFT_SQUARE_BRAKET, "\\[", "LEFT_SQUARE_BRAKET");
   add_token(blaise_token::RIGHT_SQUARE_BRAKET, "\\]", "LEFT_SQUARE_BRAKET");
   add_token(blaise_token::NOT_EQUAL, "<>", "NOT_EQUAL");
   add_token(blaise_token::SPACESHIP, "<=>", "SPACESHIP");
   add_token(blaise_token::LEFT_ANGLE_BRAKET, "<", "LEFT_SQUARE_BRAKET");
   add_token(blaise_token::RIGHT_ANGLE_BRAKET, ">", "LEFT_SQUARE_BRAKET");
   add_token(blaise_token::PLUS, "\\+", "PLUS");
   add_token(blaise_token::MINUS, "-", "MINUS");
   add_token(blaise_token::MULTIPLY, "\\*", "MULTIPLY");
   add_token(blaise_token::DIVIDE, "\\\\", "DIVIDE");
   add_token(blaise_token::REMINDER, "%", "REMINDER");
   add_token(blaise_token::EQUAL, "=", "EQUAL");

   // OTHERS
   add_token(blaise_token::ATTRIBUTE, "@\\[[a-zA-Z][a-ZA-Z0-9_]*\\]", "ATTRIBUTE");
   add_token(blaise_token::IDENTIFIER, "[a-zA-Z][a-zA-Z0-9_]*", "IDENTIFIER", true);
   add_token(blaise_token::STRING_CONSTANT, "\"(?:[^\"]|\\\\.)*\"", "STRING_CONSTANT", true);
   add_token(blaise_token::INTEGER_CONSTANT, "[0-9][0-9,]*|0b[0-1][0-1,]*|0o[0-7][0-7,]*|0x[0-9a-fA-F][0-9a-fA-F,]*", "INTEGER_CONSTANT", true);
   add_token(blaise_token::CHAR_CONSTANT, "'(?:.|\\\\\\d{1,3}|\\\\u\\d{5})'", "CHAR_CONSTANT", true);
   add_token(blaise_token::FLOATING_POINT_CONSTANT, "[0-9]+\\.[0-9]+", "CHAR_CONSTANT", true);

   // COMMENT
   add_token(blaise_token::COMMENT, "\\{[^\\}]*\\}", "COMMENT", false, false);
   add_token(blaise_token::LINE_COMMENT, "#(?:.|\\s)*", "LINE_COMMENT", false, false);
}

blaise_token_provider::blaise_token_provider_constructor blaise_token_provider::_private;
std::vector<blaise_token>::const_iterator blaise_token_provider::cbegin() { return _private.cbegin(); }
std::vector<blaise_token>::const_iterator blaise_token_provider::cend() { return _private.cend(); }

std::string blaise_token_provider::rule(blaise_token token) { return _private.rule(token); }
std::string blaise_token_provider::name(blaise_token token) { return _private.name(token); }
bool blaise_token_provider::keep_value(blaise_token token) { return _private.keep_value(token); }
bool blaise_token_provider::keep_token(blaise_token token) { return _private.keep_token(token); }

std::ostream &gasp::blaise::operator<<(std::ostream &os, const blaise_token &token)
{
   return os << std::string(blaise_token_provider::name(token));
}