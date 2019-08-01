#pragma once

#include <ostream>
#include <string>

#include <gasp/blaise/tokenizer/impl/blaise_token_type.hpp>

namespace gasp::blaise
{

std::ostream &operator<<(std::ostream &os, const blaise_token_type &token_type);

} // namespace gasp::blaise