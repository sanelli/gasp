#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>

#include <gasp/blaise/ast.hpp>
#include <gasp/blaise/tokenizer/tokens.hpp>
#include <gasp/blaise/tokenizer/tokenizer.hpp>
#include <sanelli/sanelli.hpp>


using namespace sanelli;
using namespace gasp::blaise::ast;
using namespace gasp::blaise;
using namespace gasp::common;
using namespace std;

//
// EXPRESSION
// 
blaise_ast_expression::blaise_ast_expression(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
      blaise_ast_expression_type expression_type, std::shared_ptr<blaise_ast_type> result_type) 
   : blaise_ast(reference), _expression_type(expression_type), _result_type(result_type){ }
std::shared_ptr<blaise_ast_type> blaise_ast_expression::result_type() const { return _result_type; }
blaise_ast_expression_type blaise_ast_expression::expression_type() const { return _expression_type; }

//
// CAST EXPRESSION
//
blaise_ast_expression_cast::blaise_ast_expression_cast(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
      std::shared_ptr<blaise_ast_type> target_type,
      std::shared_ptr<blaise_ast_expression> operand
   ) : blaise_ast_expression(reference, blaise_ast_expression_type::CAST, target_type), _operand(operand) {}
std::shared_ptr<blaise_ast_expression> blaise_ast_expression_cast::operand() const{
   return _operand;
}

std::shared_ptr<blaise_ast_expression_cast> gasp::blaise::ast::make_blaise_ast_expression_cast(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
      std::shared_ptr<blaise_ast_type> target_type,
      std::shared_ptr<blaise_ast_expression> operand) {
   return memory::make_shared<blaise_ast_expression_cast>(reference, target_type, operand);
}

shared_ptr<blaise_ast_expression> gasp::blaise::ast::introduce_cast_if_required(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
      std::shared_ptr<blaise_ast_type> target_type,
      shared_ptr<blaise_ast_expression> expression){
   if(!expression->result_type()->equals(target_type)) {
         expression = ast::make_blaise_ast_expression_cast(reference, target_type, expression);
   }
   return expression;
}

std::shared_ptr<ast::blaise_ast_expression> ast::cast_to_boolean( 
   const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
   std::shared_ptr<ast::blaise_ast_expression> expression){
   auto bool_type = ast::make_plain_type(ast::blaise_ast_system_type::BOOLEAN);
   if(!expression->result_type()->equals(bool_type)){
      if(!ast::blaise_ast_utility::can_auto_cast(bool_type, expression->result_type()))
         throw ast::blaise_ast_error(reference.line(), reference.column(), sanelli::make_string("Expression cannot be converted into a boolean expression."));
      else
         expression = ast::introduce_cast_if_required(reference, bool_type, expression);
   }
   return expression;
}

void gasp::blaise::ast::introduce_cast_if_required(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
      std::shared_ptr<blaise_ast_subroutine> subroutine,
      std::vector<std::shared_ptr<blaise_ast_expression>>& expressions) {

      if(subroutine->get_arity() != expressions.size())
         throw std::logic_error(sanelli::make_string("Number of parameters for subroutine '", subroutine->signature_as_string(),"' do not match the number of expressions (", expressions.size(),")"));

      for(auto index=0; index < expressions.size(); ++index){
         auto expected_type = subroutine->get_parameter_type(index);
         expressions.at(index) = introduce_cast_if_required(reference, expected_type, expressions.at(index));
      }
}

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
      if(ast::make_plain_type(blaise_ast_system_type::VOID)->equals(subroutine->return_type()))
            throw blaise_ast_error(reference.line(), reference.column(), sanelli::make_string("Cannot call procedure '", subroutine->signature_as_string(), " inside an expressions."));
      std::copy(expressions.begin(), expressions.end(), std::back_inserter(_expressions));
   }

typename vector<shared_ptr<blaise_ast_expression>>::const_iterator blaise_ast_expression_subroutine_call::begin_actual_parameters() const { return _expressions.cbegin(); }
typename vector<shared_ptr<blaise_ast_expression>>::const_iterator blaise_ast_expression_subroutine_call::end_actual_parameters() const { return _expressions.cend(); };
std::shared_ptr<blaise_ast_subroutine> blaise_ast_expression_subroutine_call::subroutine() const { return _subroutine; }
shared_ptr<blaise_ast_expression_subroutine_call> gasp::blaise::ast::make_blaise_ast_expression_subroutine_call(
   token<blaise_token_type> subroutine_name_token,
   shared_ptr<blaise_ast_subroutine> subroutine,
   const vector<shared_ptr<blaise_ast_expression>>& expressions){
         return memory::make_shared<blaise_ast_expression_subroutine_call>(subroutine_name_token, subroutine, expressions);
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
std::shared_ptr<blaise_ast_expression_binary> gasp::blaise::ast::make_blaise_ast_expression_binary(
               std::shared_ptr<blaise_ast_expression> left,
               gasp::common::token<gasp::blaise::blaise_token_type> token_operator,
               std::shared_ptr<blaise_ast_expression> right) {
   return memory::make_shared<blaise_ast_expression_binary>(token_operator, left, token_operator.type(), right);
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
std::shared_ptr<blaise_ast_expression_unary> gasp::blaise::ast::make_blaise_ast_expression_unary(gasp::common::token<gasp::blaise::blaise_token_type> token_operator,
                        std::shared_ptr<blaise_ast_expression> operand) {
   return memory::make_shared<blaise_ast_expression_unary>(token_operator, token_operator.type(), operand);
}

// MEMORY ACCESS
blaise_ast_expression_generic_memory_access::blaise_ast_expression_generic_memory_access(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
                  blaise_ast_expression_memory_access_type memory_access_type,
                  std::shared_ptr<blaise_ast_type> result_type) 
                  : blaise_ast_expression(reference, blaise_ast_expression_type::IDENTIFIER, result_type), 
                  _memory_access_type(memory_access_type){

   }
blaise_ast_expression_memory_access_type blaise_ast_expression_generic_memory_access::memory_access_type() const { return _memory_access_type; }

blaise_ast_expression_memory_access::blaise_ast_expression_memory_access(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
                     std::shared_ptr<blaise_ast_generic_memory_location> memory_location) 
   : blaise_ast_expression_generic_memory_access(reference, blaise_ast_expression_memory_access_type::MEMORY_LOCATION, memory_location->type()),
       _memory_location(memory_location) { 
   
}
std::shared_ptr<blaise_ast_generic_memory_location> blaise_ast_expression_memory_access::memory_location() const { return _memory_location; }
std::shared_ptr<blaise_ast_expression_memory_access> gasp::blaise::ast::make_blaise_ast_expression_memory_access(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
      std::shared_ptr<blaise_ast_generic_memory_location> memory_location){
  return memory::make_shared<blaise_ast_expression_memory_access>(reference, memory_location);
}

blaise_ast_expression_array_access::blaise_ast_expression_array_access(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
                     std::shared_ptr<blaise_ast_generic_memory_location> array,
                     std::shared_ptr<blaise_ast_expression> indexing)
   : blaise_ast_expression_generic_memory_access(reference, blaise_ast_expression_memory_access_type::MEMORY_LOCATION, ast::blaise_ast_utility::as_array_type(array->type())->inner_type()), 
      _array(array), _indexing(indexing) { 
      }
std::shared_ptr<blaise_ast_generic_memory_location> blaise_ast_expression_array_access::array() const { return _array;}
std::shared_ptr<blaise_ast_expression> blaise_ast_expression_array_access::indexing() const { return _indexing; }
std::shared_ptr<blaise_ast_expression_array_access> gasp::blaise::ast::make_blaise_ast_expression_array_access(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
                     std::shared_ptr<blaise_ast_generic_memory_location> array,
                     std::shared_ptr<blaise_ast_expression> indexing){
  return memory::make_shared<blaise_ast_expression_array_access>(reference, array, indexing);
}
// TERNARY
blaise_ast_ternary_cast::blaise_ast_ternary_cast(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
   std::shared_ptr<blaise_ast_expression> condition,
   std::shared_ptr<blaise_ast_expression> then_expression,
   std::shared_ptr<blaise_ast_expression> else_expression
) : blaise_ast_expression(reference, blaise_ast_expression_type::TERNARY, blaise_ast_utility::get_common_type(reference, then_expression->result_type(), else_expression->result_type())),
   _condition(condition), _then_expression(then_expression), _else_expression(else_expression) 
{
   if(!condition->result_type()->equals(make_plain_type(blaise_ast_system_type::BOOLEAN)))
      throw blaise_ast_error(reference.line(), reference.column(), sanelli::make_string("Ternary condition must be a boolean expression."));
}
std::shared_ptr<blaise_ast_expression> blaise_ast_ternary_cast::condition() const { return _condition; }
std::shared_ptr<blaise_ast_expression> blaise_ast_ternary_cast::then_expression() const { return _then_expression; }
std::shared_ptr<blaise_ast_expression> blaise_ast_ternary_cast::else_expression() const { return _else_expression; }

std::shared_ptr<blaise_ast_ternary_cast> ast::make_blaise_ast_ternary_cast(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
   std::shared_ptr<blaise_ast_expression> condition,
   std::shared_ptr<blaise_ast_expression> then_expression,
   std::shared_ptr<blaise_ast_expression> else_expression
   )
{
   return memory::make_shared<blaise_ast_ternary_cast>(reference, condition, then_expression, else_expression);
}