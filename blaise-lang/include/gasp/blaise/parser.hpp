#pragma once

#include <vector>

#include <gasp/common/tokenizer.hpp>
#include <gasp/blaise/tokens.hpp>

namespace gasp::blaise
{

class blaise_parser
{
public:
   void parse(const std::vector<gasp::common::token<gasp::blaise::blaise_token>> &tokens);
};

} // namespace gasp::blaise