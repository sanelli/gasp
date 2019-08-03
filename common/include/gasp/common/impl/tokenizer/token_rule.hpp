#pragma once

#include <string>
#include <regex>
#include <tuple>
#include <utility>

#include <sanelli/sanelli.hpp>
#include <gasp/common/debug.hpp>
#include <gasp/common/gasp_error.hpp>
#include <gasp/common/impl/tokenizer/tokenizer_error.hpp>
#include <gasp/common/impl/tokenizer/token.hpp>
#include <gasp/common/impl/tokenizer/token_io.hpp>

namespace gasp::common
{

template <typename TTokenType>
class token_rule
{
   TTokenType _token_type;
   std::regex _regexp;
   bool _keep_value;
   bool _keep_token;
   bool _is_punctuation;
   std::string _plain_regexp;

public:
   token_rule(TTokenType token_type, std::string regular_expression, bool keep_value, bool keep_token, bool is_punctuation = false)
   : _token_type(token_type), _keep_value(keep_value), _keep_token(keep_token), _plain_regexp(regular_expression), _is_punctuation(is_punctuation)
   {
      auto after_expression_match_word = std::string("(?:\\b|\\s+|$)");
      auto after_expression = std::string("(?:\\s+|$|)");
      auto expression_group = std::string("(?:" + regular_expression + ")");
      std::string expression = expression_group;
      if(!is_punctuation) expression = expression + after_expression_match_word;
      else expression = expression + after_expression;
      std::string final_expression = "^(?:" +  expression + ")";
      _regexp.assign(final_expression, std::regex_constants::ECMAScript | std::regex_constants::optimize);
   }

   std::tuple<bool, token<TTokenType>, std::string, bool> match(std::string input, int line, int column, bool ignore_spaces) const
   {
      GASP_DEBUG("common-tokenizer", std::string("\t MATCHING WITH: '") << _plain_regexp << std::string("' : "))

      std::smatch match;
      bool found = std::regex_search(input, match, _regexp);

      GASP_DEBUG("common-tokenizer", (found ? std::string("YES") : std::string("NO")) << std::endl);

      if (found)
         {
            auto value = _keep_value ? match[0] : std::string("");
            if(_keep_value && ignore_spaces){
               sanelli::ltrim(value);
               sanelli::rtrim(value);
            }
            return std::make_tuple<>(true, token(_token_type, value, line, column), match[0], _keep_token);
         }
      return std::make_tuple<>(false, token<TTokenType>(), "", _keep_token);
   }
};
}