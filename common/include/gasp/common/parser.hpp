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
   parser_error(const char *message) : domain_error(message), _line(0), _column(0) {}
   parser_error(std::string &message) : domain_error(message), _line(0), _column(0) {}
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

   template<typename T>
   friend std::ostream &operator<<(std::ostream &os, const parser_context<T> &token);

public:
   void push_back(gasp::common::token<TTokenType> token) { return _tokens.push_back(token); }
   gasp::common::token<TTokenType> token(unsigned int index) { return _tokens.at(index); }
   gasp::common::token<TTokenType> peek_token() { return _tokens.at(_index); }
   unsigned int index() { return _index; }
   unsigned int move_next_token() { return ++_index; }
   bool has_more_tokens() { return _index < _tokens.size(); }
};

template <typename TTokenType>
std::ostream &operator<<(std::ostream &os, const parser_context<TTokenType> &context)
{
   return os << context._tokens;
}

template <typename TTokenType>
class parser
{
protected:
   static bool is_token(parser_context<TTokenType> &context, TTokenType token_type)
   {
      auto token = context.peek_token();
      return token.type() == token_type;
   }

   static std::string match_token(parser_context<TTokenType> &context, TTokenType token_type)
   {
      if (!context.has_more_tokens())
         throw parser_error("No more tokens to parse.");

      auto token = context.peek_token();
      if (is_token(context, token_type))
      {
         auto value = token.value();
         context.move_next_token();
         return value;
      }
      throw parser_error(token.line(), token.column(), "Unexpected token");
   }
};
} // namespace gasp::common