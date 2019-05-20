#pragma once

#include <string>
#include <regex>
#include <tuple>
#include <utility>
#include <vector>

namespace gasp::common
{
template <typename TToken>
class token
{
   TToken _token;
   std::string _value;

public:
   token() : _token(0), _value("") {}
   token(TToken token) : _token(token), _value("") {}
   token(TToken token, std::string value) : _token(token), _value(value) {}
   token(const token<TToken> &other) : _token(other._token), _value(other._value) {}
   token(token<TToken> &&other) : _token(std::move(other._token)), _value(std::move(other._value)) {}
};

template <typename TToken>
class token_rule
{
   TToken _token;
   std::regex _regexp;
   bool _keep_value;

public:
   token_rule(TToken token, std::string regular_expression, bool keep_value) : _token(token), _keep_value(keep_value)
   {
      _regexp.assign("^(" + regular_expression + ")", std::regex_constants::ECMAScript);
   }

   std::tuple<bool, token<TToken>, std::string> match(std::string input)
   {
      std::smatch match;
      bool found = std::regex_search(input, match, _regexp);
      if (found)
         return std::make_tuple<>(true, token(_token, _keep_value ? match[0] : std::string("")), match[0]);
      return std::make_tuple<>(false, token<TToken>(), "");
   }
};

template <typename TToken>
class tokenizer
{
   bool _ignore_spaces;
   std::vector<token_rule<TToken>> _rules;

public:
   tokenizer(bool ingore_spaces = true) : _ignore_spaces(ingore_spaces) {}

   void add(TToken token, const std::string &rule, bool keep_value)
   {
      _rules.emplace_back(token, rule, keep_value);
   }

   void parse(const std::string &line, std::vector<token<TToken>> &tokens)
   {
   }
};
} // namespace gasp::common