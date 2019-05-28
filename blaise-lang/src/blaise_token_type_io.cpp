#include <gasp/blaise/tokens.hpp>

using namespace gasp::blaise;
using namespace gasp::common;

std::ostream &gasp::blaise::operator<<(std::ostream &os, const blaise_token_type &token_type)
{
   return os << std::string(blaise_token_provider::name(token_type));
}