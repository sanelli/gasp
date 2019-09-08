#include <memory>

#include <gasp/blaise/ast.hpp>
#include <sanelli/sanelli.hpp>

using namespace sanelli;
using namespace std;

using namespace gasp;
using namespace gasp::blaise;
using namespace gasp::blaise::ast;

std::shared_ptr<blaise_ast_variable_identifier> blaise_ast_identifier_utility::as_variable_identifier(std::shared_ptr<blaise_ast_identifier> identifier)
{
   return std::static_pointer_cast<blaise_ast_variable_identifier>(identifier);
}

std::shared_ptr<blaise_ast_array_identifier> blaise_ast_identifier_utility::as_array_identifier(std::shared_ptr<blaise_ast_identifier> identifier)
{
   return std::static_pointer_cast<blaise_ast_array_identifier>(identifier);
}