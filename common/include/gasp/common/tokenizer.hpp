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
#include <istream>

#include <gasp/common/debug.hpp>
#include <gasp/common/string.hpp>
#include <gasp/common/exception.hpp>

namespace gasp::common
{

template <typename TTokenType>
class token
{
   TTokenType _type;
   std::string _value;
   unsigned int _line;
   unsigned int _column;

public:
   token() : _type(static_cast<TTokenType>(0)), _value(""), _line(0), _column(0) {}
   token(TTokenType token, unsigned int line, unsigned int column) : _type(token), _value(""), _line(line), _column(column) {}
   token(TTokenType token, std::string value, unsigned int line, unsigned int column) : _type(token), _value(value), _line(line), _column(column) {}
   token(const token<TTokenType> &other) : _type(other._type), _value(other._value), _line(other._line), _column(other._column) {}
   token(token<TTokenType> &&other) : _type(std::move(other._type)), _value(std::move(other._value)), _line(std::move(other._line)), _column(std::move(other._column)) {}

   token<TTokenType>& operator=(const token<TTokenType>& other) {
      _type = other._type;
      _value = other._value;
      _line = other._line;
      _column = other._column;
      return *this;
   }

   token<TTokenType>& operator=(token<TTokenType>&& other) {
      _type = std::move(other._type);
      _value = std::move(other._value);
      _line = std::move(other._line);
      _column = std::move(other._column);
      return *this;
   }


   TTokenType type() const { return _type; }
   std::string value() const { return _value; }
   unsigned int line() const { return _line; }
   unsigned int column() const { return _column; }
};

template <typename TTokenType>
std::ostream &operator<<(std::ostream &os, const token<TTokenType> &token)
{
   return os << std::string("[")
             << token.type() << std::string(",") << token.value() << std::string(",")
             << std::string("(") << token.line() << std::string(",") << token.column() << std::string(")")
             << std::string("]");
}

template <typename TTokenType>
class token_rule
{
   TTokenType _token_type;
   std::regex _regexp;
   bool _keep_value;
   bool _keep_token;
   std::string _plain_regexp;

public:
   token_rule(TTokenType token_type, std::string regular_expression, bool keep_value, bool keep_token) 
   : _token_type(token_type), _keep_value(keep_value), _keep_token(keep_token), _plain_regexp(regular_expression)
   {
      _regexp.assign("^(?:(?:" + regular_expression + ")(?:\\b|\\s*$|\\s*))", std::regex_constants::ECMAScript);
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
            if(ignore_spaces){
               gasp::common::ltrim(value);
               gasp::common::rtrim(value);
            }
            return std::make_tuple<>(true, token(_token_type, value, line, column), match[0], _keep_token);
         }
      return std::make_tuple<>(false, token<TTokenType>(), "", _keep_token);
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

template <typename TTokenType>
class tokenizer
{
   bool _ignore_spaces;
   std::vector<token_rule<TTokenType>> _rules;

public:
   tokenizer(bool ingore_spaces = true) : _ignore_spaces(ingore_spaces) {}

   void set_ignore_spaces(bool ignore_spaces) { _ignore_spaces = ignore_spaces; }
   void ignore_spaces() { return _ignore_spaces; }

   void add(TTokenType token, const std::string &rule, bool keep_value = false, bool keep_token = true)
   {
      if (!keep_token && keep_value)
         throw gasp::common::gasp_error("Trying to add a token where keep_token = false but keep_value = true");
      _rules.emplace_back(token, rule, keep_value, keep_token);
   }

   template <typename TTokenContainer>
   void tokenize(const std::string &input, int line_number, TTokenContainer &tokens) const
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
                  column += gasp::common::ltrim(line);
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

template <typename TTokenType>
class token_provider_constructor
{
protected:
   std::map<TTokenType, std::tuple<std::string, std::string, bool, bool>> _token_values;
   std::vector<TTokenType> _tokens;

   void add_token(TTokenType token, std::string rule, std::string name, bool keep_value = false, bool keep_token = true)
   {
      _tokens.push_back(token);
      _token_values.insert(std::make_pair<>(token, std::make_tuple<>(rule, name, keep_value, keep_token)));
   }

public:
   token_provider_constructor() {}

   typename std::vector<TTokenType>::const_iterator cbegin() const { return _tokens.cbegin(); }
   typename std::vector<TTokenType>::const_iterator cend() const { return _tokens.cend(); }

   std::string rule(TTokenType token) const { return std::get<0>(_token_values.at(token)); }
   std::string name(TTokenType token) const { return std::get<1>(_token_values.at(token)); }
   bool keep_value(TTokenType token) const { return std::get<2>(_token_values.at(token)); }
   bool keep_token(TTokenType token) const { return std::get<3>(_token_values.at(token)); }
};

} // namespace gasp::common