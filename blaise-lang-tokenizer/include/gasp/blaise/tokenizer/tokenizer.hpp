#pragma once

#include <sanelli/sanelli.hpp>
#include <gasp/blaise/tokenizer/tokens.hpp>

namespace gasp::blaise
{

class blaise_tokenizer : public sanelli::tokenizer<gasp::blaise::blaise_token_type> 
{

public:
   blaise_tokenizer();
};

} // namespace gasp::blaise