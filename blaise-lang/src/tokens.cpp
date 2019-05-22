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
   add_token(blaise_token::BEGIN, "begin", "BEGIN");
   add_token(blaise_token::END, "end", "END");
   add_token(blaise_token::VAR, "var", "VAR");

   // TYPES
   //add_token(blaise_token::PROGRAM, "program", "program");

   // PUNCTUATION
   add_token(blaise_token::SEMICOLON, ";", "SEMICOLON");
   add_token(blaise_token::PERIOD, "\\.", "PERIOD");
   add_token(blaise_token::COLON, "\\:", "COLON");

   // OTHERS
   add_token(blaise_token::IDENTIFIER, "[a-zA-Z][a-zA-Z0-9_]*", "IDENTIFIER", true);
}

blaise_token_provider::blaise_token_provider_constructor blaise_token_provider::_private;
std::vector<blaise_token>::const_iterator blaise_token_provider::cbegin() { return _private.cbegin(); }
std::vector<blaise_token>::const_iterator blaise_token_provider::cend() { return _private.cend(); }

std::string blaise_token_provider::rule(blaise_token token) { return _private.rule(token); }
std::string blaise_token_provider::name(blaise_token token) { return _private.name(token); }
bool blaise_token_provider::keep_value(blaise_token token) { return _private.keep_value(token); }

std::ostream &gasp::blaise::operator<<(std::ostream &os, const blaise_token &token)
{
   return os << std::string(blaise_token_provider::name(token));
}