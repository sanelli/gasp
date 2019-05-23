#pragma once

#include <string>
#include <vector>
#include <stdexcept>

namespace gasp::common
{

class parser_error : public std::domain_error
{
   int _line;
   int _column;

public:
   parser_error(int line, int column, const char *message) : domain_error(message), _line(line), _column(column) {}
   parser_error(int line, int column, const std::string &message) : domain_error(message), _line(line), _column(column) {}

   int line() { return _line; }
   int column() { return _column; }
};

template <typename TToken>
class parser_context
{
   std::vector<gasp::common::token<TToken>> _tokens;
   unsigned int _index;

public:
   void add_token(TToken token, std::string value, int line, int column) { return _tokens.emplace(token, value, line, column); }
   gasp::common::token<TToken> token(unsigned int index) { return _tokens.at(index); }
   gasp::common::token<TToken> peek() { return _tokens.at(_index); }
   unsigned int index() { return _index; }
   unsigned int next() { return ++_index; }
};

template <typename TToken>
class parser
{
protected:
   static bool is_token(TToken match_token, unsigned int &index, const std::vector<gasp::common::token<TToken>> &tokens)
   {
      auto token = tokens.at(index);
      return token.token_value() == match_token;
   }

   static std::string match_token(TToken match_token, unsigned int &index, const std::vector<gasp::common::token<TToken>> &tokens)
   {
      if (is_token(match_token, index, tokens))
      {
         auto value = tokens.at(index).value();
         ++index;
         return value;
      }

      throw parser_error(tokens.at(index).line(), tokens.at(index).column(), "Unexpected token");
   }
};
} // namespace gasp::common