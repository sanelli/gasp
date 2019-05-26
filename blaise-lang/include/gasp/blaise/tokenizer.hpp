#pragma once

#include <vector>
#include <istream>

#include <gasp/common/tokenizer.hpp>
#include <gasp/blaise/tokens.hpp>

namespace gasp::blaise
{

class blaise_tokenizer : public gasp::common::tokenizer<gasp::blaise::blaise_token_type> 
{

public:
   blaise_tokenizer();
};

} // namespace gasp::blaise