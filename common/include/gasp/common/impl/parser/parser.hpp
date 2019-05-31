#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <algorithm>

#include <gasp/common/string.hpp>
#include <gasp/common/debug.hpp>

#include <gasp/common/impl/parser/parser_error.hpp>
#include <gasp/common/impl/parser/parser_context.hpp>
#include <gasp/common/impl/parser/parser_io.hpp>

namespace gasp::common
{

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

   static std::string match_token_and_get_value(parser_context<TTokenType> &context, TTokenType token_type)
   {
      if (!context.has_more_tokens())
         throw parser_error("No more tokens to parse.");

      auto token = context.peek_token();
      if (is_token(context, token_type))
      {
         GASP_DEBUG("common-parser", make_string("common_parser::match_token_and_get_value<", token_type,">") << std::endl);

         auto value = token.value();
         context.move_next_token();
         return value;
      }
      auto message = gasp::common::make_string("Unexpected token: was expecting '", token_type, "' but found '", token.type(), "'.");
      throw_parse_error_with_details(context, token.line(), token.column(), message);
   }

   static void match_token(parser_context<TTokenType> &context, TTokenType token_type)
   {
      if (!context.has_more_tokens())
         throw parser_error("No more tokens to parse.");

      auto token = context.peek_token();
      if (is_token(context, token_type))
      {
         GASP_DEBUG("common-parser", make_string("common_parser::match_token<", token_type,">") << std::endl);
         context.move_next_token();
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