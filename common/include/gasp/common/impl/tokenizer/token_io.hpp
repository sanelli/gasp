#pragma once

#include <string>
#include <ostream>

#include <gasp/common/impl/tokenizer/token.hpp>

namespace gasp::common
{

template <typename TTokenType>
std::ostream &operator<<(std::ostream &os, const token<TTokenType> &token)
{
   return os << std::string("[")
             << token.type() << std::string(",") << token.value() << std::string(",")
             << std::string("(") << token.line() << std::string(",") << token.column() << std::string(")")
             << std::string("]");
}

}