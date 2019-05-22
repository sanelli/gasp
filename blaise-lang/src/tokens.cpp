#include <gasp/common/internal_error.hpp>
#include <gasp/common/tokenizer.hpp>
#include <gasp/blaise/tokens.hpp>

using namespace gasp::blaise;
using namespace gasp::common;

const char *gasp::blaise::get_token_rule(blaise_token token)
{
   switch (token)
   {
   case blaise_token::PROGRAM:
      return "program";
   case blaise_token::BEGIN:
      return "begin";
   case blaise_token::END:
      return "end";
   case blaise_token::VAR:
      return "var";
   case blaise_token::SEMICOLON:
      return ";";
   case blaise_token::PERIOD:
      return "\\.";
   case blaise_token::COLON:
      return "\\:";
   case blaise_token::IDENTIFIER:
      return "[a-zA-Z][a-zA-Z0-9_]*";
   default:
      throw gasp_internal_error("Unexpected token - cannot convert token into rule");
   }
}

const char *gasp::blaise::get_token_name(blaise_token token)
{
   switch (token)
   {
   case blaise_token::PROGRAM:
      return "PROGRAM";
   case blaise_token::BEGIN:
      return "BEGIN";
   case blaise_token::END:
      return "END";
   case blaise_token::VAR:
      return "VAR";
   case blaise_token::SEMICOLON:
      return "SEMICOLON";
   case blaise_token::PERIOD:
      return "PERIOD";
   case blaise_token::COLON:
      return "COLON";
   case blaise_token::IDENTIFIER:
      return "IDENTIFIER";
   default:
      throw gasp_internal_error("Unexpected token - cannot convert token into string");
   }
}

std::ostream &gasp::blaise::operator<<(std::ostream &os, const blaise_token &tok)
{
   os << std::string(get_token_name(tok));
   return os;
}