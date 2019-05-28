#include <catch2/catch.hpp>

#include <vector>
#include <string>

#include <gasp/blaise/tokenizer.hpp>
#include <gasp/common/tokenizer.hpp>

using namespace gasp::common;
using namespace gasp::blaise;

SCENARIO("Parse single tokens", "[blaise-lang][tokenizer][base]")
{
   std::vector<token<blaise_token_type>> tokens;
   GIVEN("the blaise tokenizer"){

      blaise_tokenizer tokenizer;

      /* *************************************************************** *
       * KEYWORD                                                         *
       * *************************************************************** */      
      WHEN("the token is a simple keyword")
      {
         THEN("it can match the simple keyword"){
            tokenizer.tokenize("program", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::PROGRAM);
            REQUIRE(tokens.at(0).value() == "");
         }

         THEN("it can match the simple keyword with trailing spaces")
         {
            tokenizer.tokenize("program     ", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::PROGRAM);
            REQUIRE(tokens.at(0).value() == "");
         }

         THEN("it can match the simple keyword with heading spaces")
         {
            tokenizer.tokenize("       program", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::PROGRAM);
            REQUIRE(tokens.at(0).value() == "");
         }

         THEN("it can match the simple keyword with heading and trailing spaces")
         {
            tokenizer.tokenize("       program      ", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::PROGRAM);
            REQUIRE(tokens.at(0).value() == "");
         }
      }

      /* *************************************************************** *
       * PUCTUATION                                                      *
       * *************************************************************** */
      WHEN("the token is a pnctuation")
      {
         THEN("it can match the punctuation"){
            tokenizer.tokenize(";", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::SEMICOLON);
            REQUIRE(tokens.at(0).value() == "");
         }

         THEN("it can match the punctuation with trailing spaces")
         {
            tokenizer.tokenize(";     ", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::SEMICOLON);
            REQUIRE(tokens.at(0).value() == "");
         }

         THEN("it can match the punctuation with heading spaces")
         {
            tokenizer.tokenize("       ;", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::SEMICOLON);
            REQUIRE(tokens.at(0).value() == "");
         }

         THEN("it can match the punctuation with heading and trailing spaces")
         {
            tokenizer.tokenize("       ;      ", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::SEMICOLON);
            REQUIRE(tokens.at(0).value() == "");
         }
      }

      /* *************************************************************** *
       * STRING                                                         *
       * *************************************************************** */  
      WHEN("the token is a string")
      {
         THEN("it can match the string"){
            tokenizer.tokenize("\"hello world\"", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::STRING_LITERAL);
            REQUIRE(tokens.at(0).value() == "\"hello world\"");
         }

         THEN("it can match the string if it contains an escape sequence")
         {
            tokenizer.tokenize("\"hello \\n world\"", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::STRING_LITERAL);
            REQUIRE(tokens.at(0).value() == "\"hello \\n world\"");
         }

         THEN("it can match the string if it contains an escaped double quote (\")")
         {
            tokenizer.tokenize("\"ste \\\" fano\"", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::STRING_LITERAL);
            REQUIRE(tokens.at(0).value() == "\"ste \\\" fano\"");
         }

         THEN("it can match a string containig trailing spaces")
         {
            tokenizer.tokenize("\"hello world\"     ", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::STRING_LITERAL);
            REQUIRE(tokens.at(0).value() == "\"hello world\"");
         }

         THEN("it can match a string containig heading spaces")
         {
            tokenizer.tokenize("    \"hello world\"", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::STRING_LITERAL);
            REQUIRE(tokens.at(0).value() == "\"hello world\"");
         }

         THEN("it can match a string containig heading and trailing spaces")
         {
            tokenizer.tokenize("    \"hello world\"       ", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::STRING_LITERAL);
            REQUIRE(tokens.at(0).value() == "\"hello world\"");
         }  
      }

      /* *************************************************************** *
       * CHARACTER                                                       *
       * *************************************************************** */  
      WHEN("the token is a character")
      {
         THEN("it can match the character"){
            tokenizer.tokenize("'c'", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::CHAR_LITERAL);
            REQUIRE(tokens.at(0).value() == "'c'");
         }

         THEN("it can match a character containig an escape sequence single character")
         {
            tokenizer.tokenize("'\\n'", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::CHAR_LITERAL);
            REQUIRE(tokens.at(0).value() == "'\\n'");
         }

         THEN("it can match a character containig an escaped unicode sequence")
         {
            tokenizer.tokenize("'\\u9F0a'", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::CHAR_LITERAL);
            REQUIRE(tokens.at(0).value() == "'\\u9F0a'");
         }

         THEN("it can match a character token containig trailing spaces")
         {
            tokenizer.tokenize("'c'  ", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::CHAR_LITERAL);
            REQUIRE(tokens.at(0).value() == "'c'");
         }

         THEN("it can match a character token containig heading spaces")
         {
            tokenizer.tokenize("    'c'", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::CHAR_LITERAL);
            REQUIRE(tokens.at(0).value() == "'c'");
         }

         THEN("it can match a character token containig heading and trailing spaces")
         {
            tokenizer.tokenize("    'c'    ", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::CHAR_LITERAL);
            REQUIRE(tokens.at(0).value() == "'c'");
         }
      }

      /* *************************************************************** *
       * INTEGER                                                         *
       * *************************************************************** */  
      WHEN("the token is a integer literal")
      {
         THEN("it can match the integer literal"){
            tokenizer.tokenize("17", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::INTEGER_LITERAL);
            REQUIRE(tokens.at(0).value() == "17");
         }

         THEN("it can match 0"){
            tokenizer.tokenize("0", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::INTEGER_LITERAL);
            REQUIRE(tokens.at(0).value() == "0");
         }

         THEN("it can match the integer literal with trailing spaces"){
            tokenizer.tokenize("17   ", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::INTEGER_LITERAL);
            REQUIRE(tokens.at(0).value() == "17");
         }

         THEN("it can match the integer literal with heading spaces"){
            tokenizer.tokenize("   17", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::INTEGER_LITERAL);
            REQUIRE(tokens.at(0).value() == "17");
         }

         THEN("it can match the integer literal with heading and trailing spaces"){
            tokenizer.tokenize("   17    ", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::INTEGER_LITERAL);
            REQUIRE(tokens.at(0).value() == "17");
         }
      }

      /* *************************************************************** *
       * INTEGER BASE 2                                                  *
       * *************************************************************** */  
      WHEN("the token is a integer base 2 literal")
      {
         THEN("it can match the integer literal"){
            tokenizer.tokenize("0b100101", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::INTEGER_BASE_TWO_LITERAL);
            REQUIRE(tokens.at(0).value() == "0b100101");
         }

         THEN("it can match 0b0"){
            tokenizer.tokenize("0b0", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::INTEGER_BASE_TWO_LITERAL);
            REQUIRE(tokens.at(0).value() == "0b0");
         }

         THEN("it can match the integer base 2 literal with trailing spaces"){
            tokenizer.tokenize("0b100101   ", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::INTEGER_BASE_TWO_LITERAL);
            REQUIRE(tokens.at(0).value() == "0b100101");
         }

         THEN("it can match the integer base 2 literal with heading spaces"){
            tokenizer.tokenize("   0b100101", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::INTEGER_BASE_TWO_LITERAL);
            REQUIRE(tokens.at(0).value() == "0b100101");
         }

         THEN("it can match the integer base 2 literal with heading and trailing spaces"){
            tokenizer.tokenize("   0b100101    ", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::INTEGER_BASE_TWO_LITERAL);
            REQUIRE(tokens.at(0).value() == "0b100101");
         }
      }

      /* *************************************************************** *
       * INTEGER BASE 8                                                  *
       * *************************************************************** */  
      WHEN("the token is a integer base 8 literal")
      {
         THEN("it can match the integer literal"){
            tokenizer.tokenize("0o176", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::INTEGER_BASE_EIGHT_LITERAL);
            REQUIRE(tokens.at(0).value() == "0o176");
         }

         THEN("it can match 0o0"){
            tokenizer.tokenize("0o0", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::INTEGER_BASE_EIGHT_LITERAL);
            REQUIRE(tokens.at(0).value() == "0o0");
         }

         THEN("it can match the integer base 8 literal with trailing spaces"){
            tokenizer.tokenize("0o176   ", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::INTEGER_BASE_EIGHT_LITERAL);
            REQUIRE(tokens.at(0).value() == "0o176");
         }

         THEN("it can match the integer base 8 literal with heading spaces"){
            tokenizer.tokenize("   0o176", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::INTEGER_BASE_EIGHT_LITERAL);
            REQUIRE(tokens.at(0).value() == "0o176");
         }

         THEN("it can match the integer base 8 literal with heading and trailing spaces"){
            tokenizer.tokenize("   0o176    ", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::INTEGER_BASE_EIGHT_LITERAL);
            REQUIRE(tokens.at(0).value() == "0o176");
         }
      }

      /* *************************************************************** *
       * INTEGER BASE 16                                                 *
       * *************************************************************** */  
      WHEN("the token is a integer base 16 literal")
      {
         THEN("it can match the integer literal"){
            tokenizer.tokenize("0x1fA0", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::INTEGER_BASE_SIXTEEN_LITERAL);
            REQUIRE(tokens.at(0).value() == "0x1fA0");
         }

         THEN("it can match 0x0"){
            tokenizer.tokenize("0x0", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::INTEGER_BASE_SIXTEEN_LITERAL);
            REQUIRE(tokens.at(0).value() == "0x0");
         }

         THEN("it can match the integer base 16 literal with trailing spaces"){
            tokenizer.tokenize("0x1fA0   ", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::INTEGER_BASE_SIXTEEN_LITERAL);
            REQUIRE(tokens.at(0).value() == "0x1fA0");
         }

         THEN("it can match the integer base 16 literal with heading spaces"){
            tokenizer.tokenize("   0x1fA0", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::INTEGER_BASE_SIXTEEN_LITERAL);
            REQUIRE(tokens.at(0).value() == "0x1fA0");
         }

         THEN("it can match the integer base 16 literal with heading and trailing spaces"){
            tokenizer.tokenize("   0x1fA0    ", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::INTEGER_BASE_SIXTEEN_LITERAL);
            REQUIRE(tokens.at(0).value() == "0x1fA0");
         }
      }

      /* *************************************************************** *
       * DECIMAL                                                         *
       * *************************************************************** */  
      WHEN("the token is a decimal literal")
      {
         THEN("it can match the decimal literal"){
            tokenizer.tokenize("17.13", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::DECIMAL_LITERAL);
            REQUIRE(tokens.at(0).value() == "17.13");
         }

         THEN("it can match 0.0"){
            tokenizer.tokenize("0.0", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::DECIMAL_LITERAL);
            REQUIRE(tokens.at(0).value() == "0.0");
         }

         THEN("it can match the decimal literal with trailing spaces"){
            tokenizer.tokenize("17.13   ", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::DECIMAL_LITERAL);
            REQUIRE(tokens.at(0).value() == "17.13");
         }

         THEN("it can match the decimal literal with heading spaces"){
            tokenizer.tokenize("   17.13", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::DECIMAL_LITERAL);
            REQUIRE(tokens.at(0).value() == "17.13");
         }

         THEN("it can match the decimal literal with heading and trailing spaces"){
            tokenizer.tokenize("   17.13    ", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::DECIMAL_LITERAL);
            REQUIRE(tokens.at(0).value() == "17.13");
         }
      }

      /* *************************************************************** *
       * IDENTIFIER                                                       *
       * *************************************************************** */  
      WHEN("the token is an identifier")
      {
         THEN("it can match the identifier"){
            tokenizer.tokenize("foo", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::IDENTIFIER);
            REQUIRE(tokens.at(0).value() == "foo");
         }

         THEN("it can match the identifier starting with underscore"){
            tokenizer.tokenize("__foo", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::IDENTIFIER);
            REQUIRE(tokens.at(0).value() == "__foo");
         }

         THEN("it can match the identifier containig underscores"){
            tokenizer.tokenize("f_o_o", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::IDENTIFIER);
            REQUIRE(tokens.at(0).value() == "f_o_o");
         }

         THEN("it can match the identifier containig numbers"){
            tokenizer.tokenize("foo001", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::IDENTIFIER);
            REQUIRE(tokens.at(0).value() == "foo001");
         }

         THEN("it can match the identifier containig number and underscores"){
            tokenizer.tokenize("foo_001", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::IDENTIFIER);
            REQUIRE(tokens.at(0).value() == "foo_001");
         }

         THEN("it can match the identifier containig lowercase and uppercase (starting lowercase)"){
            tokenizer.tokenize("fooBAR", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::IDENTIFIER);
            REQUIRE(tokens.at(0).value() == "fooBAR");
         }

         THEN("it can match the identifier containig lowercase and uppercase (starting uppercase)"){
            tokenizer.tokenize("Foo", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::IDENTIFIER);
            REQUIRE(tokens.at(0).value() == "Foo");
         }

         THEN("it can match a identifier containig trailing spaces")
         {
            tokenizer.tokenize("foo     ", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::IDENTIFIER);
            REQUIRE(tokens.at(0).value() == "foo");
         }

         THEN("it can match a identifier containig heading spaces")
         {
            tokenizer.tokenize("    foo", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::IDENTIFIER);
            REQUIRE(tokens.at(0).value() == "foo");
         }

         THEN("it can match a string containig heading and trailing spaces")
         {
            tokenizer.tokenize("    foo       ", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::IDENTIFIER);
            REQUIRE(tokens.at(0).value() == "foo");
         }

         THEN("it can match the identifier"){
            tokenizer.tokenize("foo", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::IDENTIFIER);
            REQUIRE(tokens.at(0).value() == "foo");
         }
      }
   }
}

SCENARIO("Parse multiple tokens sequences", "[blaise-lang][tokenizer][base]")
{
   std::vector<token<blaise_token_type>> tokens;
   GIVEN("the blaise tokenizer") {

      blaise_tokenizer tokenizer;

      WHEN("the string is 'program HelloWorld;'")
      {
         tokenizer.tokenize("program HelloWorld;", 0, tokens);
         THEN("it matches 3 tokens (PROGRAM, IDENTIFIER, COMMA)"){
            REQUIRE(tokens.size() == 3);
            REQUIRE(tokens.at(0).type() == blaise_token_type::PROGRAM);
            REQUIRE(tokens.at(1).type() == blaise_token_type::IDENTIFIER);
            REQUIRE(tokens.at(1).value() == "HelloWorld");
            REQUIRE(tokens.at(2).type() == blaise_token_type::SEMICOLON);
         }
      }

      WHEN("the string is 'foo and bar eager and moo'")
      {
         tokenizer.tokenize("foo and bar eager and moo", 0, tokens);
         THEN("it correctly mathes eager and non eager 'and' operators"){
            REQUIRE(tokens.size() == 5);
            REQUIRE(tokens.at(0).type() == blaise_token_type::IDENTIFIER);
            REQUIRE(tokens.at(1).type() == blaise_token_type::LOGICAL_AND);
            REQUIRE(tokens.at(2).type() == blaise_token_type::IDENTIFIER);
            REQUIRE(tokens.at(3).type() == blaise_token_type::LOGICAL_EAGER_AND);
            REQUIRE(tokens.at(4).type() == blaise_token_type::IDENTIFIER);
         }
      }

      WHEN("the string is 'foo or bar eager or moo'")
      {
         tokenizer.tokenize("foo or bar eager or moo", 0, tokens);
         THEN("it correctly mathes eager and non eager 'or' operators"){
            REQUIRE(tokens.size() == 5);
            REQUIRE(tokens.at(0).type() == blaise_token_type::IDENTIFIER);
            REQUIRE(tokens.at(1).type() == blaise_token_type::LOGICAL_OR);
            REQUIRE(tokens.at(2).type() == blaise_token_type::IDENTIFIER);
            REQUIRE(tokens.at(3).type() == blaise_token_type::LOGICAL_EAGER_OR);
            REQUIRE(tokens.at(4).type() == blaise_token_type::IDENTIFIER);
         }
      }

   }
}

SCENARIO("Parse all keyword and punctuation tokens", "[blaise-lang][tokenizer][pedantic]")
{
   std::vector<token<blaise_token_type>> tokens;
   GIVEN("the blaise tokenizer") {

      blaise_tokenizer tokenizer;

      WHEN("any of the keyword is passed")
      {
         THEN("it matches the 'program' keyword"){
            tokenizer.tokenize("program", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::PROGRAM);
         }

         THEN("it matches the 'var' keyword"){
            tokenizer.tokenize("var", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::VAR);
         }

         THEN("it matches the 'uses' keyword"){
            tokenizer.tokenize("uses", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::USES);
         }

         THEN("it matches the 'begin' keyword"){
            tokenizer.tokenize("begin", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::BEGIN);
         }

         THEN("it matches the 'end' keyword"){
            tokenizer.tokenize("end", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::END);
         }

         THEN("it matches the 'and' keyword"){
            tokenizer.tokenize("and", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::LOGICAL_AND);
         }

         THEN("it matches the 'or' keyword"){
            tokenizer.tokenize("or", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::LOGICAL_OR);
         }

         THEN("it matches the 'not' keyword"){
            tokenizer.tokenize("not", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::LOGICAL_NOT);
         }

         THEN("it matches the 'eager and' keyword"){
            tokenizer.tokenize("eager and", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::LOGICAL_EAGER_AND);
         }

         THEN("it matches the 'eager or' keyword"){
            tokenizer.tokenize("eager or", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::LOGICAL_EAGER_OR);
         }
         // TODO: Add all the other keyword
      }

      WHEN("any of the punctuation is passed")
      {
         THEN("it matches the ';' puntuation"){
            tokenizer.tokenize(";", 0, tokens);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens.at(0).type() == blaise_token_type::SEMICOLON);
         }
         //TODO: Add all the other punctuation
      }
   }
}