#pragma once

#include <vector>

#include <gasp/common/tokenizer.hpp>
#include <gasp/common/parser.hpp>
#include <gasp/blaise/tokens.hpp>

namespace gasp::blaise
{

class blaise_parser_context : public gasp::common::parser_context<blaise_token>
{
};

class blaise_parser : public gasp::common::parser<blaise_token>
{
   static void parse_program(blaise_parser_context &context);
   static void parse_variables_declaration(blaise_parser_context &context);
   static void parse_variable_declaration(blaise_parser_context &context);
   static void parse_variable_names_list(blaise_parser_context &context, std::vector<std::string>& variable_names);
   static void parse_variable_type(blaise_parser_context &context);

public:
   void parse(blaise_parser_context &context) const;
};

} // namespace gasp::blaise