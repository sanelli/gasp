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
blaise_ast_expression::blaise_ast_expression(blaise_ast_expression_type expression_type, blaise_ast_type result_type) 
   :  _expression_type(expression_type), _result_type(result_type){ }
blaise_ast_type blaise_ast_expression::result_type() const { return _result_type; }
blaise_ast_expression_type blaise_ast_expression::expression_type() const { return _expression_type; }


//
// SUBROUTINE CALL EXPRESSION
//
blaise_ast_expression_subroutine_call::blaise_ast_expression_subroutine_call(
      token<blaise_token_type> subroutine_name_token,
      shared_ptr<blaise_ast_subroutine> subroutine,
      const vector<shared_ptr<blaise_ast_expression>>& expressions)
         : blaise_ast_expression(blaise_ast_expression_type::FUNCTION_CALL, subroutine->return_type()),
         _subroutine(subroutine)
   {
      if(subroutine->return_type() == blaise_ast_type::VOID)
                  throw blaise_ast_error(subroutine_name_token.line(), subroutine_name_token.column(), make_string("Cannot use call procedure '",  subroutine_name_token.value(), " inside an expressions."));
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
blaise_ast_expression_binary::blaise_ast_expression_binary(shared_ptr<blaise_ast_expression> left,
               token<blaise_token_type> token_operator,
               shared_ptr<blaise_ast_expression> right
) : blaise_ast_expression(blaise_ast_expression_type::BINARY, blaise_ast_utility::get_resulting_type(token_operator, left->result_type(), right->result_type())),
    _operator(token_operator.type()), _left(left), _right(right) {

}
shared_ptr<blaise_ast_expression> blaise_ast_expression_binary::left() const { return _left; }
shared_ptr<blaise_ast_expression> blaise_ast_expression_binary::right() const { return _right; }
blaise_token_type blaise_ast_expression_binary::op() const { return _operator; }
std::shared_ptr<blaise_ast_expression_binary> gasp::blaise::ast::blaise_ast_expression_binary_factory(
               std::shared_ptr<blaise_ast_expression> left,
               gasp::common::token<gasp::blaise::blaise_token_type> token_operator,
               std::shared_ptr<blaise_ast_expression> right) {
   return make_shared<blaise_ast_expression_binary>(left, token_operator, right);
   }

//
// UNARY EXPRESSION
//
blaise_ast_expression_unary::blaise_ast_expression_unary(gasp::common::token<gasp::blaise::blaise_token_type> token_operator, 
      std::shared_ptr<blaise_ast_expression> operand)
   : blaise_ast_expression(blaise_ast_expression_type::UNARY, blaise_ast_utility::get_resulting_type(token_operator, operand->result_type())),
    _operator(token_operator.type()), _operand(operand) {

}
shared_ptr<blaise_ast_expression> blaise_ast_expression_unary::operand() const { return _operand; }
blaise_token_type blaise_ast_expression_unary::op() const { return _operator; }
std::shared_ptr<blaise_ast_expression_unary> gasp::blaise::ast::blaise_ast_expression_unary_factory(gasp::common::token<gasp::blaise::blaise_token_type> token_operator,
                        std::shared_ptr<blaise_ast_expression> operand) {
   return make_shared<blaise_ast_expression_unary>(token_operator, operand);
}

// VARIABLE
blaise_ast_expression_memory_location::blaise_ast_expression_memory_location(shared_ptr<blaise_ast_generic_memory_location> memory_location) 
   : blaise_ast_expression(blaise_ast_expression_type::MEMORY_LOCATION, memory_location->type()), _memory_location(memory_location) { 
   
}
std::shared_ptr<blaise_ast_generic_memory_location> blaise_ast_expression_memory_location::memory_location() const { return _memory_location; }
std::shared_ptr<blaise_ast_expression_memory_location> gasp::blaise::ast::blaise_ast_expression_memory_location_factory(const std::shared_ptr<blaise_ast_subroutine>& subroutine, token<blaise_token_type> token){
   switch(token.type()) {
      case blaise_token_type::IDENTIFIER:
      {
         auto variable = subroutine->get_memory_location(token);
         if(variable == nullptr)
            throw blaise_ast_error(token.line(), token.column(), make_string("Undefined variable/constant/parameter '",token.value(),"'"));
         return make_shared<blaise_ast_expression_memory_location>(variable);
      }
      default:
         throw blaise_ast_error(token.line(), token.column(), "Was expecting an identifier");
   }
}

//
// LITERAL
//
shared_ptr<blaise_ast_expression> gasp::blaise::ast::blaise_ast_expression_value_factory(token<blaise_token_type> token){
   switch(token.type()){
      case blaise_token_type::INTEGER_LITERAL:
         return make_shared<blaise_ast_expression_integer_value>(stoi(token.value()));
      case blaise_token_type::INTEGER_BASE_TWO_LITERAL:
         return make_shared<blaise_ast_expression_integer_value>(stoi(token.value().substr(2), nullptr, 2));
      case blaise_token_type::INTEGER_BASE_EIGHT_LITERAL:
         return make_shared<blaise_ast_expression_integer_value>(stoi(token.value().substr(2), nullptr, 8));
      case blaise_token_type::INTEGER_BASE_SIXTEEN_LITERAL:
         return make_shared<blaise_ast_expression_integer_value>(stoi(token.value().substr(2), nullptr, 16));
      case blaise_token_type::FLOAT_LITERAL:
         return make_shared<blaise_ast_expression_float_value>(stof(token.value()));
      case blaise_token_type::DOUBLE_LITERAL:
         return make_shared<blaise_ast_expression_double_value>(stod(token.value()));
      case blaise_token_type::CHAR_LITERAL:
         return make_shared<blaise_ast_expression_char_value>(token.value()[1]);
      case blaise_token_type::STRING_LITERAL:
         return make_shared<blaise_ast_expression_string_value>(token.value().substr(1, token.value().length()-2));
      case blaise_token_type::BOOLEAN_LITERAL:
         return make_shared<blaise_ast_expression_boolean_value>(token.value() == "true");
      default:
         throw blaise_ast_error(token.line(), token.column(), "Token is not an expression");
   }
}
