#include <vector>

#include <gasp/common/internal_error.hpp>
#include <gasp/common/tokenizer.hpp>
#include <gasp/blaise/tokens.hpp>

using namespace gasp::blaise;
using namespace gasp::common;

blaise_token_provider::blaise_token_provider_constructor::blaise_token_provider_constructor()
{

   // KEYWORDS
   add_token(blaise_token::PROGRAM, "program", "PROGRAM");
   add_token(blaise_token::MODULE, "module", "MODULE");
   add_token(blaise_token::USES, "uses", "USES");
   add_token(blaise_token::BEGIN, "begin", "BEGIN");
   add_token(blaise_token::END, "end", "END");
   add_token(blaise_token::VAR, "var", "VAR");

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
   add_token(blaise_token::SEMICOLON, ";", "SEMICOLON");
   add_token(blaise_token::PERIOD, "\\.", "PERIOD");
   add_token(blaise_token::COLON, "\\:", "COLON");
   add_token(blaise_token::COMMA, "\\,", "COMMA");

   // OTHERS
   add_token(blaise_token::IDENTIFIER, "[a-zA-Z][a-zA-Z0-9_]*", "IDENTIFIER", true);
}

blaise_token_provider::blaise_token_provider_constructor blaise_token_provider::_private;
std::vector<blaise_token>::const_iterator blaise_token_provider::cbegin() { return _private.cbegin(); }
std::vector<blaise_token>::const_iterator blaise_token_provider::cend() { return _private.cend(); }

std::string blaise_token_provider::rule(blaise_token token) { return _private.rule(token); }
std::string blaise_token_provider::name(blaise_token token) { return _private.name(token); }
bool blaise_token_provider::keep_value(blaise_token token) { return _private.keep_value(token); }
bool blaise_token_provider::keep_token(blaise_token token) { return _private.keep_token(token); }

bool blaise_token_utility::is_type(blaise_token token)
{
   switch (token)
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

bool blaise_token_utility::is_unsigned_type(blaise_token token)
{
   switch (token)
   {
   case blaise_token::TYPE_BYTE:
   case blaise_token::TYPE_INTEGER:
   case blaise_token::TYPE_LONG:
      return true;
   default:
      return false;
   };
}

std::ostream &gasp::blaise::operator<<(std::ostream &os, const blaise_token &token)
{
   return os << std::string(blaise_token_provider::name(token));
}