#include <gasp/blaise/tokenizer/tokenizer.hpp>

using namespace gasp::blaise;


std::ostream &gasp::blaise::operator<<(std::ostream &os, const blaise_token_type &token_type)
{
   return os << std::string(blaise_token_type_provider::name(token_type));
}