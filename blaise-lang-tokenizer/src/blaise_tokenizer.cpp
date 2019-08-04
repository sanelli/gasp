#include <vector>
#include <string>


#include <gasp/blaise/tokenizer/tokenizer.hpp>
#include <gasp/blaise/tokenizer/tokens.hpp>

using namespace gasp::blaise;

using namespace std;

blaise_tokenizer::blaise_tokenizer()
{
   // Set up the tokenizer
   for (auto it = blaise_token_type_provider::cbegin(); it != blaise_token_type_provider::cend(); ++it)
   {
      auto token = *it;
      auto rule = blaise_token_type_provider::rule(token);
      auto keep_value = blaise_token_type_provider::keep_value(token);
      auto keep_token = blaise_token_type_provider::keep_token(token);
      auto is_punctuation = blaise_token_type_provider::is_punctuation(token);

      add(token, rule, keep_value, keep_token, is_punctuation);
   }
}
