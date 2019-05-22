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
   // Set up the tokenizer
   _tokenizer.set_token_rule_provider(get_token_rule);

   // KEYWORDS
   _tokenizer.add(blaise_token::PROGRAM);
   _tokenizer.add(blaise_token::BEGIN);
   _tokenizer.add(blaise_token::END);

   // PUNCTUATION
   _tokenizer.add(blaise_token::SEMICOLON);
   _tokenizer.add(blaise_token::PERIOD);
   _tokenizer.add(blaise_token::COLON);

   // OTHERS
   _tokenizer.add(blaise_token::IDENTIFIER, true);
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
