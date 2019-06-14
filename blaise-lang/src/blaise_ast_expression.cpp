#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>

#include <gasp/blaise/ast.hpp>
#include <gasp/blaise/tokens.hpp>
#include <gasp/common/tokenizer.hpp>

using namespace gasp::blaise::ast;
using namespace gasp::blaise;
using namespace gasp::common;
using namespace std;

//
// EXPRESSION
// 
blaise_ast_expression::blaise_ast_expression(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
      blaise_ast_expression_type expression_type, blaise_ast_type result_type) 
   : blaise_ast(reference), _expression_type(expression_type), _result_type(result_type){ }
blaise_ast_type blaise_ast_expression::result_type() const { return _result_type; }
blaise_ast_expression_type blaise_ast_expression::expression_type() const { return _expression_type; }


//
// SUBROUTINE CALL EXPRESSION
//
blaise_ast_expression_subroutine_call::blaise_ast_expression_subroutine_call(
      const token<blaise_token_type>& reference,
      shared_ptr<blaise_ast_subroutine> subroutine,
      const vector<shared_ptr<blaise_ast_expression>>& expressions)
         : blaise_ast_expression(reference, blaise_ast_expression_type::FUNCTION_CALL, subroutine->return_type()),
         _subroutine(subroutine)
   {
      if(subroutine->return_type() == blaise_ast_type::VOID)
                  throw blaise_ast_error(reference.line(), reference.column(), make_string("Cannot call procedure '", subroutine->signature_as_string(), " inside an expressions."));
      std::copy(expressions.begin(), expressions.end(), std::back_inserter(_expressions));
   }

typename vector<shared_ptr<blaise_ast_expression>>::const_iterator blaise_ast_expression_subroutine_call::begin_actual_parameters() const { return _expressions.cbegin(); }
typename vector<shared_ptr<blaise_ast_expression>>::const_iterator blaise_ast_expression_subroutine_call::end_actual_parameters() const { return _expressions.cend(); };
std::shared_ptr<blaise_ast_subroutine> blaise_ast_expression_subroutine_call::subroutine() const { return _subroutine; }
shared_ptr<blaise_ast_expression_subroutine_call> gasp::blaise::ast::blaise_ast_expression_subroutine_call_factory(
   token<blaise_token_type> subroutine_name_token,
   shared_ptr<blaise_ast_subroutine> subroutine,
   const vector<shared_ptr<blaise_ast_expression>>& expressions){
         return make_shared<blaise_ast_expression_subroutine_call>(subroutine_name_token, subroutine, expressions);
}

//
// BINARY EXPRESSION
//
blaise_ast_expression_binary::blaise_ast_expression_binary(
               const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
               shared_ptr<blaise_ast_expression> left,
               blaise_token_type op,
               shared_ptr<blaise_ast_expression> right
) : blaise_ast_expression(reference, blaise_ast_expression_type::BINARY, blaise_ast_utility::get_resulting_type(reference, op, left->result_type(), right->result_type())),
    _operator(op), _left(left), _right(right) {

}
shared_ptr<blaise_ast_expression> blaise_ast_expression_binary::left() const { return _left; }
shared_ptr<blaise_ast_expression> blaise_ast_expression_binary::right() const { return _right; }
blaise_token_type blaise_ast_expression_binary::op() const { return _operator; }
std::shared_ptr<blaise_ast_expression_binary> gasp::blaise::ast::blaise_ast_expression_binary_factory(
               std::shared_ptr<blaise_ast_expression> left,
               gasp::common::token<gasp::blaise::blaise_token_type> token_operator,
               std::shared_ptr<blaise_ast_expression> right) {
   return make_shared<blaise_ast_expression_binary>(token_operator, left, token_operator.type(), right);
   }

//
// UNARY EXPRESSION
//
blaise_ast_expression_unary::blaise_ast_expression_unary(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
      gasp::blaise::blaise_token_type op,
      std::shared_ptr<blaise_ast_expression> operand)
   : blaise_ast_expression(reference, blaise_ast_expression_type::UNARY, blaise_ast_utility::get_resulting_type(reference, op, operand->result_type())),
    _operator(op), _operand(operand) {

}
shared_ptr<blaise_ast_expression> blaise_ast_expression_unary::operand() const { return _operand; }
blaise_token_type blaise_ast_expression_unary::op() const { return _operator; }
std::shared_ptr<blaise_ast_expression_unary> gasp::blaise::ast::blaise_ast_expression_unary_factory(gasp::common::token<gasp::blaise::blaise_token_type> token_operator,
                        std::shared_ptr<blaise_ast_expression> operand) {
   return make_shared<blaise_ast_expression_unary>(token_operator, token_operator.type(), operand);
}

// MEMORY LOCATION
blaise_ast_expression_memory_location::blaise_ast_expression_memory_location(const gasp::common::token<gasp::blaise::blaise_token_type>& reference, shared_ptr<blaise_ast_generic_memory_location> memory_location) 
   : blaise_ast_expression(reference, blaise_ast_expression_type::MEMORY_LOCATION, memory_location->type()), _memory_location(memory_location) { 
   
}
std::shared_ptr<blaise_ast_generic_memory_location> blaise_ast_expression_memory_location::memory_location() const { return _memory_location; }
std::shared_ptr<blaise_ast_expression_memory_location> gasp::blaise::ast::blaise_ast_expression_memory_location_factory(const std::shared_ptr<blaise_ast_subroutine>& subroutine, token<blaise_token_type> token){
   switch(token.type()) {
      case blaise_token_type::IDENTIFIER:
      {
         auto variable = subroutine->get_memory_location(token.value());
         if(variable == nullptr)
            throw blaise_ast_error(token.line(), token.column(), make_string("Undefined variable/constant/parameter '",token.value(),"'"));
         return make_shared<blaise_ast_expression_memory_location>(token, variable);
      }
      default:
         throw blaise_ast_error(token.line(), token.column(), "Was expecting an identifier");
   }
}
