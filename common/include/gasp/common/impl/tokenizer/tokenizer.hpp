#pragma once

#include <string>
#include <vector>

#include <sanelli/sanelli.hpp>
#include <gasp/common/debug.hpp>
#include <gasp/common/impl/tokenizer/tokenizer_error.hpp>
#include <gasp/common/impl/tokenizer/token.hpp>
#include <gasp/common/impl/tokenizer/token_io.hpp>
#include <gasp/common/impl/tokenizer/token_rule.hpp>

namespace gasp::common
{

template <typename TTokenType>
class tokenizer
{
   bool _ignore_spaces;
   std::vector<token_rule<TTokenType>> _rules;

public:
   tokenizer(bool ingore_spaces = true) : _ignore_spaces(ingore_spaces) {}

   void set_ignore_spaces(bool ignore_spaces) { _ignore_spaces = ignore_spaces; }
   void ignore_spaces() { return _ignore_spaces; }

   void add(TTokenType token, const std::string &rule, bool keep_value = false, bool keep_token = true, bool is_punctuation = false)
   {
      if (!keep_token && keep_value)
         throw gasp::common::gasp_error("Trying to add a token where keep_token = false but keep_value = true");
      _rules.emplace_back(token, rule, keep_value, keep_token, is_punctuation);
   }

   template <typename TTokenContainer>
   void tokenize(const std::string &input, int line_number, TTokenContainer &tokens) const
   {
      auto line = input;
      int column = 0;
      if (_ignore_spaces)
         column += sanelli::ltrim(line);
      if (line.length() == 0)
         return; // Skip empty lines
      while (line.length() > 0)
      {
         auto found_rule = false;
         GASP_DEBUG("common-tokenizer", std::string("--------------------------") << std::endl);
         GASP_DEBUG("common-tokenizer", std::string("LINE: '") << line << std::string("'") << std::endl)
         for (auto rule : _rules)
         {
            auto [match, token, substring, keep_token] = rule.match(line, line_number, column, _ignore_spaces);
            if (match)
            {
               found_rule = true;
               if (keep_token)
                  tokens.push_back(token);
               line = line.substr(substring.length());
               column += substring.length();
               if (_ignore_spaces)
                  column += sanelli::ltrim(line);
               break;
            }
         }
         if (!found_rule)
            throw tokenizer_error(line_number, column, "Invalid character sequence");
      }
   }

   template <typename TTokenContainer>
   void tokenize(std::istream &input, TTokenContainer &tokens) const
   {
      auto line_number = 1;
      while (!input.eof())
      {
         std::string line;
         std::getline(input, line, '\n');
         tokenize(line, line_number, tokens);
         ++line_number;
      }
   }
};

}