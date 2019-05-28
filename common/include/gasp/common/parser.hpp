#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <algorithm>

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
   gasp::common::token<TTokenType> inline peek_token(unsigned int lookahead = 0) const
   {
      if (!has_more_tokens(lookahead))
         throw parser_error("No more tokens to parse");
      return _tokens.at(_index + lookahead);
   }
   unsigned int inline index() const { return _index; }
   unsigned int inline move_next_token() { return ++_index; }
   bool inline has_more_tokens(unsigned int lookahead = 0) const { return (_index + lookahead) < _tokens.size(); }

   std::string context_token_to_string(unsigned int length = 0, signed int first = -1) const {
      std::stringstream stream;
      auto start = first == -1 ? _index : (unsigned) first;
      auto last = length == 0 ? _tokens.size() : std::min(start + length + 1, (unsigned int)_tokens.size());
      for(unsigned int index = start; index < length; ++index)
         stream << index << ":" << token(index) << (index == length-1 ? "" : ", ");
      return stream.str();
   }
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
         return false;
      }
      auto token = context.token(context.index() + lookahead);
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

   //TODO: Remove returned value (most likely it is not really necessary)
   static std::string match_token(parser_context<TTokenType> &context, TTokenType token_type)
   {
      if (!context.has_more_tokens())
         throw parser_error("No more tokens to parse.");

      auto token = context.peek_token();
      if (is_token(context, token_type))
      {
         GASP_DEBUG("common-parser", make_string("common_parser::match_token<", token_type,">") << std::endl);

         auto value = token.value();
         context.move_next_token();
         return value;
      }
      auto message = gasp::common::make_string("Unexpected token: was expecting '", token_type, "' but found '", token.type(), "'.");
      throw_parse_error_with_details(context, token.line(), token.column(), message);
   }

   [[noreturn]]
   static inline void throw_parse_error_with_details(const parser_context<TTokenType> &context, const unsigned int line, const unsigned int column, const std::string& message, unsigned int display_token = 5){
      GASP_DEBUG("common-parser", make_string("common_parser::parser_error<", context.context_token_to_string(display_token), ">") << std::endl);
      throw parser_error(line, column, message);
   }

   [[noreturn]]
   static inline void throw_parse_error_with_details(const parser_context<TTokenType> &context, const std::string& message, unsigned int display_token = 5){
      GASP_DEBUG("common-parser", make_string("common_parser::parser_error<", context.context_token_to_string(display_token), ">") << std::endl);
      throw parser_error(0,0,message);
   }
};
} // namespace gasp::common