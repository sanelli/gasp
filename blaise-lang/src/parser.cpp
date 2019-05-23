#include <vector>

#include <gasp/common/tokenizer.hpp>

#include <gasp/blaise/tokens.hpp>
#include <gasp/blaise/parser.hpp>

using namespace std;
using namespace gasp::blaise;
using namespace gasp::common;

void blaise_parser::parse(const vector<token<blaise_token>> &tokens) const
{
   unsigned int index = 0;
   if (is_token(blaise_token::PROGRAM, index, tokens))
      parse_program(index, tokens);
   else
      throw parser_error(tokens[index].line(), tokens[index].column(), "only programs are supported.");
}

void blaise_parser::parse_program(unsigned int &index, const vector<token<blaise_token>> &tokens)
{
   match_token(blaise_token::PROGRAM, index, tokens);
   auto program_name = match_token(blaise_token::IDENTIFIER, index, tokens);
   match_token(blaise_token::SEMICOLON, index, tokens);

   // TODO: match uses
   // TODO: match functions and procedures
   // TODO: match constants
   // TODO: match variables

   match_token(blaise_token::BEGIN, index, tokens);

   while (!is_token(blaise_token::END, index, tokens))
   {
      // TODO: match statements
   }
   match_token(blaise_token::END, index, tokens);
   match_token(blaise_token::PERIOD, index, tokens);
}
