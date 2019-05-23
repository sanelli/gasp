#pragma once

#include <vector>

#include <gasp/common/tokenizer.hpp>
#include <gasp/common/parser.hpp>
#include <gasp/blaise/tokens.hpp>

namespace gasp::blaise
{

class blaise_parser : public gasp::common::parser<blaise_token>
{
   static void parse_program(unsigned int& index, const std::vector<gasp::common::token<gasp::blaise::blaise_token>> &tokens);
   static void parse_variables(unsigned int& index, const std::vector<gasp::common::token<gasp::blaise::blaise_token>> &tokens);
public:
   void parse(const std::vector<gasp::common::token<gasp::blaise::blaise_token>> &tokens) const;
};

} // namespace gasp::blaise