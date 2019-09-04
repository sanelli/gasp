#include <memory>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <map>
#include <string>

#include <gasp/blaise/ast.hpp>
#include <gasp/torricelly/torricelly.hpp>
#include <gasp/blaise-to-torricelly/blaise-to-torricelly.hpp>
#include <gasp/common/internal_error.hpp>

#include <sanelli/sanelli.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::blaise;

void blaise_to_torricelly::translator::translate_expression(std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine,
                                                            const std::map<std::string, unsigned int> &variables_mapping,
                                                            std::shared_ptr<gasp::blaise::ast::blaise_ast_expression> expression,
                                                            unsigned int &max_stack_size) const
{
   switch (expression->expression_type())
   {
   case ast::blaise_ast_expression_type::BINARY:
      break;
   case ast::blaise_ast_expression_type::CAST:
      break;
   case ast::blaise_ast_expression_type::FUNCTION_CALL:
      break;
   case ast::blaise_ast_expression_type::IDENTIFIER:
      break;
   case ast::blaise_ast_expression_type::LITERAL_BOOLEAN:
      break;
   case ast::blaise_ast_expression_type::LITERAL_CHAR:
      break;
   case ast::blaise_ast_expression_type::LITERAL_DOUBLE:
      break;
   case ast::blaise_ast_expression_type::LITERAL_FLOAT:
      break;
   case ast::blaise_ast_expression_type::LITERAL_INTEGER:
      break;
   case ast::blaise_ast_expression_type::LITERAL_STRING:
      break;
   case ast::blaise_ast_expression_type::TERNARY:
      break;
   case ast::blaise_ast_expression_type::UNARY:
      break;
   default:
      throw blaise_to_torricelly_internal_error("Unknown or unsupported expression type found during translation");
   }
}