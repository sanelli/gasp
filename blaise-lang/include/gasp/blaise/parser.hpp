#pragma once

#include <vector>

#include <gasp/common/tokenizer.hpp>
#include <gasp/common/parser.hpp>
#include <gasp/blaise/tokens.hpp>

namespace gasp::blaise
{

class blaise_parser_context : public gasp::common::parser_context<blaise_token>
{
public:
   blaise_parser_context() : gasp::common::parser_context<blaise_token>(){};
};

class blaise_parser : public gasp::common::parser<blaise_token>
{
   // TODO: Create a match identifier so that in the future I can 
   //       handle identifiers followed by array definition

   // TODO: Create a match number so I can handle multiple bases case

   static void parse_program(blaise_parser_context &context);
   static void parse_variables_declaration(blaise_parser_context &context);
   static void parse_variable_declaration(blaise_parser_context &context);
   static void parse_variable_names_list(blaise_parser_context &context, std::vector<std::string> &variable_names);
   static void parse_variable_type(blaise_parser_context &context);
   static void parse_statement(blaise_parser_context &context);
   static void parse_compound_statement(blaise_parser_context &context);
   static void parse_function_call_statement(blaise_parser_context &context);
   static void parse_function_call_parameters(blaise_parser_context &context);
   static void parse_assignamet_statement(blaise_parser_context &context);
   static inline void parse_expression(blaise_parser_context &context);
   static void parse_expression_helper(blaise_parser_context &context, unsigned int min_precedence);
   static void parse_expression_term(blaise_parser_context &context);

public:
   void parse(blaise_parser_context &context) const;
};

} // namespace gasp::blaise