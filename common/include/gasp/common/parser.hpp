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

template <typename TTokenType>
class parser_context
{
   std::vector<gasp::common::token<TTokenType>> _tokens;
   unsigned int _index;

public:
   void add_token(TTokenType token, std::string value, int line, int column) { return _tokens.emplace(token, value, line, column); }
   gasp::common::token<TTokenType> token(unsigned int index) { return _tokens.at(index); }
   gasp::common::token<TTokenType> peek() { return _tokens.at(_index); }
   unsigned int index() { return _index; }
   unsigned int next() { return ++_index; }
};

template <typename TTokenType>
class parser
{
protected:
   static bool is_token(TTokenType token_type, unsigned int &index, const std::vector<gasp::common::token<TTokenType>> &tokens)
   {
      auto token = tokens.at(index);
      return token.type() == token_type;
   }

   static std::string match_token(TTokenType token_type, unsigned int &index, const std::vector<gasp::common::token<TTokenType>> &tokens)
   {
      if (is_token(token_type, index, tokens))
      {
         auto value = tokens.at(index).value();
         ++index;
         return value;
      }

      throw parser_error(tokens.at(index).line(), tokens.at(index).column(), "Unexpected token");
   }
};
} // namespace gasp::common