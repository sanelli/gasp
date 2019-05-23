#include <vector>

#include <gasp/common/tokenizer.hpp>

#include <gasp/blaise/tokens.hpp>
#include <gasp/blaise/parser.hpp>

using namespace std;
using namespace gasp::blaise;
using namespace gasp::common;

void blaise_parser::parse(blaise_parser_context &context) const
{
   unsigned int index = 0;
   if (is_token(context, blaise_token::PROGRAM))
      parse_program(context);
   else
      throw parser_error("only programs are supported.");

   // If more tokens are available at this stage then the code is malformed
   if (context.has_more_tokens())
      throw parser_error("too many tokens at the end of the program");
}

void blaise_parser::parse_program(blaise_parser_context &context)
{
   match_token(context, blaise_token::PROGRAM);
   auto program_name = match_token(context, blaise_token::IDENTIFIER);
   match_token(context, blaise_token::SEMICOLON);

   // TODO: match uses
   // TODO: match functions and procedures
   // TODO: match constants
   // TODO: match variables

   match_token(context, blaise_token::BEGIN);

   while (!is_token(context, blaise_token::END))
   {
      // TODO: match statements
   }
   match_token(context, blaise_token::END);
   match_token(context, blaise_token::PERIOD);
}
