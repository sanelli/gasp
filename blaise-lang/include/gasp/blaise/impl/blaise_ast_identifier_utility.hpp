#pragma once

#include <memory>
#include <gasp/blaise/impl/blaise_ast_identifier.hpp>

namespace gasp::blaise::ast
{
class blaise_ast_identifier_utility
{
public:
   static std::shared_ptr<blaise_ast_variable_identifier> as_variable_identifier(std::shared_ptr<blaise_ast_identifier> identifier);
   static std::shared_ptr<blaise_ast_array_identifier> as_array_identifier(std::shared_ptr<blaise_ast_identifier> identifier);
};

} // namespace gasp::blaise::ast
