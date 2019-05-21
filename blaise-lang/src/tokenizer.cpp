#include <vector>
#include <string>

#include <gasp/common/tokenizer.hpp>
#include <gasp/blaise/tokenizer.hpp>
#include <gasp/blaise/tokens.hpp>

using namespace gasp::blaise;
using namespace gasp::common;
using namespace std;

blaise_tokenizer::blaise_tokenizer()
{
   // KEYWORDS
   _tokenizer.add(blaise_token::PROGRAM, get_token_rule(blaise_token::PROGRAM), false);
   _tokenizer.add(blaise_token::BEGIN, get_token_rule(blaise_token::BEGIN), false);
   _tokenizer.add(blaise_token::END, get_token_rule(blaise_token::END), false);

   // PUNCTUATION
   _tokenizer.add(blaise_token::SEMICOLON, get_token_rule(blaise_token::SEMICOLON), false);
   _tokenizer.add(blaise_token::PERIOD, get_token_rule(blaise_token::PERIOD), false);

   // OTHERS
   _tokenizer.add(blaise_token::IDENTIFIER, get_token_rule(blaise_token::IDENTIFIER), true);
}

void blaise_tokenizer::parse(std::istream &input, std::vector<gasp::common::token<gasp::blaise::blaise_token>> &tokens) const
{
   auto line_counter = 1;
   while (!input.eof())
   {
      std::string line;
      std::getline(input, line, '\n');
      _tokenizer.parse(line_counter, line, tokens);
      ++line_counter;
   }
}
