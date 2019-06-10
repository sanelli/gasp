#pragma once

#include <memory>

#include <gasp/blaise/tokens.hpp>
#include <gasp/blaise/tokenizer.hpp>
#include <gasp/common/tokenizer.hpp>
#include <gasp/blaise/ast.hpp>

#include <gasp/blaise/impl/parser/blaise_parser_context.hpp>


namespace gasp::blaise {

std::shared_ptr<ast::blaise_ast_expression> blaise_ast_expression_value_factory(blaise_parser_context &context, const gasp::common::token<blaise_token_type>& token);

}