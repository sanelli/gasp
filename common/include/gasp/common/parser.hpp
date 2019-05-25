#pragma once

#include <string>
#include <vector>
#include <stdexcept>

#include <gasp/common/string.hpp>
#include <gasp/common/debug.hpp>

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

   template <typename T>
   friend std::ostream &operator<<(std::ostream &os, const parser_context<T> &token);

protected:
   parser_context() { _index = 0; }

public:
   void push_back(gasp::common::token<TTokenType> token) { return _tokens.push_back(token); }
   gasp::common::token<TTokenType> inline token(unsigned int index) const { return _tokens.at(index); }
   gasp::common::token<TTokenType> inline peek_token() const
   {
      if (_index >= _tokens.size())
         throw parser_error("No more tokens to parse");
      return _tokens.at(_index);
   }
   unsigned int inline index() const { return _index; }
   unsigned int inline move_next_token() { return ++_index; }
   bool inline has_more_tokens(unsigned int lookahead = 0) const { return (_index + lookahead) < _tokens.size(); }
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
   static bool is_token(parser_context<TTokenType> &context, TTokenType token_type, unsigned long lookahead = 0)
   {
      if (!context.has_more_tokens(lookahead))
      {
         GASP_DEBUG("CAZZO" << std::endl);
         return false;
      }
      auto token = context.token(context.index() + lookahead);
      GASP_DEBUG(context.index() << std::endl);
      GASP_DEBUG(lookahead << std::endl);
      GASP_DEBUG(token_type << std::endl);
      GASP_DEBUG(token.type() << std::endl);

      return token.type() == token_type;
   }

   static bool is_token_and_match(parser_context<TTokenType> &context, TTokenType token_type)
   {
      if (is_token(context, token_type))
      {
         match_token(context, token_type);
         return true;
      }
      return false;
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
      throw parser_error(token.line(), token.column(), gasp::common::make_string("Unexpected token: was expecting '", token_type, "' but found '", token.type(), "'."));
   }
};
} // namespace gasp::common