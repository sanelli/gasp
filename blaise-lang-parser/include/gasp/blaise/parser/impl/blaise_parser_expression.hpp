#pragma once

#include <memory>

#include <gasp/blaise/tokenizer/tokenizer.hpp>
#include <gasp/blaise/ast.hpp>
#include <gasp/blaise/parser/impl/blaise_parser_context.hpp>

namespace gasp::blaise {

std::shared_ptr<ast::blaise_ast_expression> make_blaise_ast_expression_value(blaise_parser_context &context, const sanelli::token<blaise_token_type>& token);

}