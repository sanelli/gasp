#include <catch2/catch.hpp>

#include <vector>
#include <string>

#include <gasp/blaise/tokenizer.hpp>
#include <gasp/common/tokenizer.hpp>

using namespace gasp::common;
using namespace gasp::blaise;

SCENARIO("Parse single tokens", "[blaise-lang][tokenizer]")
{
   std::vector<token<blaise_token_type>> tokens;
   GIVEN("the blaise tokenizer"){

      blaise_tokenizer tokenizer;

      WHEN("the token is 'program' it is matched as PROGRAM")
      {
         tokenizer.tokenize("program", 0, tokens);
         REQUIRE(tokens.size() == 1);
         REQUIRE(tokens.at(0).type() == blaise_token_type::PROGRAM);
      }

      WHEN("the token is '\"stefano\"' it is matches as STRING_LITERAL")
      {
         tokenizer.tokenize("\"stefano\"", 0, tokens);
         REQUIRE(tokens.size() == 1);
         REQUIRE(tokens.at(0).type() == blaise_token_type::STRING_LITERAL);
         REQUIRE(tokens.at(0).value() == "\"stefano\"");
      }
   }
}