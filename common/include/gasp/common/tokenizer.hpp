#pragma once

#include <string>
#include <regex>
#include <tuple>
#include <utility>
#include <vector>
#include <exception>
#include <iostream>

#include <gasp/common/string.hpp>

namespace gasp::common
{

template <typename TToken>
class token
{
   TToken _token;
   std::string _value;

public:
   token() : _token(static_cast<TToken>(0)), _value("") {}
   token(TToken token) : _token(token), _value("") {}
   token(TToken token, std::string value) : _token(token), _value(value) {}
   token(const token<TToken> &other) : _token(other._token), _value(other._value) {}
   token(token<TToken> &&other) : _token(std::move(other._token)), _value(std::move(other._value)) {}

   TToken tok() const { return _token; }
   std::string value() const { return _value; }
};

template <typename TToken>
std::ostream &operator<<(std::ostream &os, const token<TToken> &tok)
{
   os << std::string("[") << tok.tok() << std::string(",") << tok.value() << std::string("]");
   return os;
}

template <typename TToken>
class token_rule
{
   TToken _token;
   std::regex _regexp;
   bool _keep_value;

public:
   token_rule(TToken token, std::string regular_expression, bool keep_value) : _token(token), _keep_value(keep_value)
   {
      _regexp.assign("^(?:(?:" + regular_expression + ")(?:\\b|\\s*$))", std::regex_constants::ECMAScript);
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

class tokenizer_error : public std::domain_error
{
   int _line;
   int _column;

public:
   tokenizer_error(int line, int column, const char *message) : domain_error(message), _line(line), _column(column) {}
   tokenizer_error(int line, int column, const std::string &message) : domain_error(message), _line(line), _column(column) {}

   int line() { return _line; }
   int column() { return _column; }
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

   void parse(int line_number, const std::string &input, std::vector<token<TToken>> &tokens) const
   {
      auto line = input;
      int column = 0;
      if (_ignore_spaces)
         column += gasp::common::ltrim(line);
      if (line.length() == 0)
         return; // Skip empty lines
      while (line.length() > 0)
      {
         auto found_rule = false;
         for (auto rule : _rules)
         {
            auto [match, token, substring] = rule.match(line);
            if (match)
            {
               found_rule = true;
               tokens.push_back(token);
               line = line.substr(substring.length());
               column += substring.length();
               if (_ignore_spaces)
                  column += gasp::common::ltrim(line);
               break;
            }
         }
         if (!found_rule)
            throw tokenizer_error(line_number, column, "Invalid character sequence");
      }
   }
};
} // namespace gasp::common