#pragma once

#include <stdexcept>

#include <sanelli/sanelli.hpp>
#include <gasp/common/impl/parser/parser_context.hpp>

namespace gasp::common
{

template <typename TTokenType>
std::ostream &operator<<(std::ostream &os, const parser_context<TTokenType> &context)
{
   return sanelli::operator<<(os, context._tokens);
}

}