#pragma once

#include <vector>
#include <istream>

#include <gasp/common/tokenizer.hpp>
#include <gasp/blaise/tokens.hpp>

namespace gasp::blaise
{

class blaise_tokenizer
{
   gasp::common::tokenizer<gasp::blaise::blaise_token> _tokenizer;

public:
   blaise_tokenizer();
   void parse(std::istream &input, std::vector<gasp::common::token<gasp::blaise::blaise_token>>& tokens) const;
};

} // namespace gasp::blaise