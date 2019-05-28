#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <ostream>

#include <gasp/common/debug.hpp>
#include <gasp/common/impl/parser/parser_error.hpp>

namespace gasp::common{

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

}