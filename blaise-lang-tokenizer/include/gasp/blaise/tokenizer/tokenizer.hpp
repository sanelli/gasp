#pragma once

#include <gasp/common/tokenizer.hpp>
#include <gasp/blaise/tokenizer/tokens.hpp>

namespace gasp::blaise
{

class blaise_tokenizer : public gasp::common::tokenizer<gasp::blaise::blaise_token_type> 
{

public:
   blaise_tokenizer();
};

} // namespace gasp::blaise