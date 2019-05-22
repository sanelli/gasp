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
   for (auto it = blaise_token_provider::cbegin(); it != blaise_token_provider::cend(); ++it)
   {
      auto token = *it;
      auto rule = blaise_token_provider::rule(token);
      auto keep_value = blaise_token_provider::keep_value(token);
      auto keep_token = blaise_token_provider::keep_token(token);

      _tokenizer.add(token, rule, keep_value, keep_token);
   }
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
