#pragma once

#include <string>
#include <regex>
#include <tuple>
#include <utility>
#include <vector>
#include <exception>
#include <iostream>
#include <map>
#include <stdexcept>

#include <gasp/common/string.hpp>
#include <gasp/common/exception.hpp>

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
   bool _keep_token;

public:
   token_rule(TToken token, std::string regular_expression, bool keep_value, bool keep_token) : _token(token), _keep_value(keep_value), _keep_token(keep_token)
   {
      _regexp.assign("^(?:(?:" + regular_expression + ")(?:\\b|\\s*$))", std::regex_constants::ECMAScript);
   }

   std::tuple<bool, token<TToken>, std::string, bool> match(std::string input) const
   {
      std::smatch match;
      bool found = std::regex_search(input, match, _regexp);
      if (found)
         return std::make_tuple<>(true, token(_token, _keep_value ? match[0] : std::string("")), match[0], _keep_token);
      return std::make_tuple<>(false, token<TToken>(), "", _keep_token);
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

   void set_ignore_spaces(bool ignore_spaces) { _ignore_spaces = ignore_spaces; }
   void ignore_spaces() { return _ignore_spaces; }

   void add(TToken token, const std::string &rule, bool keep_value = false, bool keep_token = true)
   {
      if (!keep_token && keep_value)
         throw gasp::common::gasp_error("Trying to add a token where keep_token = false but keep_value = true");
      _rules.emplace_back(token, rule, keep_value, keep_token);
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
            auto [match, token, substring, keep_token] = rule.match(line);
            if (match)
            {
               found_rule = true;
               if (keep_token)
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

template <typename TToken>
class token_provider_constructor
{
protected:
   std::map<TToken, std::tuple<std::string, std::string, bool, bool>> _token_values;
   std::vector<TToken> _tokens;

   void add_token(TToken token, std::string rule, std::string name, bool keep_value = false, bool keep_token = true)
   {
      _tokens.push_back(token);
      _token_values.insert(std::make_pair<>(token, std::make_tuple<>(rule, name, keep_value, keep_token)));
   }

public:
   token_provider_constructor() {}

   typename std::vector<TToken>::const_iterator cbegin() const { return _tokens.cbegin(); }
   typename std::vector<TToken>::const_iterator cend() const { return _tokens.cend(); }

   std::string rule(TToken token) const { return std::get<0>(_token_values.at(token)); }
   std::string name(TToken token) const { return std::get<1>(_token_values.at(token)); }
   bool keep_value(TToken token) const { return std::get<2>(_token_values.at(token)); }
   bool keep_token(TToken token) const { return std::get<3>(_token_values.at(token)); }
};

} // namespace gasp::common