#pragma once

#include <ostream>
#include <gasp/blaise/impl/language/blaise_language_expression_type.hpp>

namespace gasp::blaise::language {

std::ostream &operator<<(std::ostream &os, const token<TTokenType> &token)
{
   return os << std::string("[")
             << token.type() << std::string(",") << token.value() << std::string(",")
             << std::string("(") << token.line() << std::string(",") << token.column() << std::string(")")
             << std::string("]");
}


}