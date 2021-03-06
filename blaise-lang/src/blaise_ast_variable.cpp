#include <string>
#include <memory>

#include <gasp/blaise/ast.hpp>


#include <sanelli/sanelli.hpp>

using namespace sanelli;
using namespace gasp::blaise::ast;
using namespace gasp::blaise;

using namespace std;

gasp::blaise::ast::blaise_ast_generic_memory_location::blaise_ast_generic_memory_location(
    blaise_ast_variable_type variable_type,
    const token<blaise_token_type> &reference,
    const string &identifier,
    std::shared_ptr<blaise_ast_type> type)
    : blaise_ast(reference, blaise_token_type::IDENTIFIER),
      _variable_type(variable_type),
      _name(identifier),
      _type(type)
{
}

string blaise_ast_generic_memory_location::name() const { return _name; }
std::shared_ptr<blaise_ast_type> blaise_ast_generic_memory_location::type() const { return _type; }
blaise_ast_variable_type blaise_ast_generic_memory_location::variable_type() const { return _variable_type; }

gasp::blaise::ast::blaise_ast_variable::blaise_ast_variable(
    const token<blaise_token_type> &reference,
    const string &identifier,
    std::shared_ptr<blaise_ast_type> type)
    : blaise_ast_generic_memory_location(blaise_ast_variable_type::VARIABLE,
                                         reference, identifier, type) {}

gasp::blaise::ast::blaise_ast_subroutine_parameter::blaise_ast_subroutine_parameter(
    const token<blaise_token_type> &reference,
    const string &identifier,
    std::shared_ptr<blaise_ast_type> type)
    : blaise_ast_generic_memory_location(blaise_ast_variable_type::PARAMETER,
                                         reference, identifier, type) {}

gasp::blaise::ast::blaise_ast_constant::blaise_ast_constant(
    const token<blaise_token_type> &reference,
    const string &identifier,
    std::shared_ptr<blaise_ast_type> type)
    : blaise_ast_generic_memory_location(blaise_ast_variable_type::CONSTANT,
                                         reference, identifier, type) {}

void gasp::blaise::ast::blaise_ast_constant::literal_expression(std::shared_ptr<gasp::blaise::ast::blaise_ast_expression> literal) {
   _literal = literal;
}
std::shared_ptr<gasp::blaise::ast::blaise_ast_expression> gasp::blaise::ast::blaise_ast_constant::literal_expression() const {
   return _literal;
}

std::shared_ptr<blaise_ast_variable> gasp::blaise::ast::make_blaise_ast_variable(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                       const std::string &identifier,
                       std::shared_ptr<blaise_ast_type> type){
   return memory::make_shared<blaise_ast_variable>(reference, identifier, type);
                       }
std::shared_ptr<blaise_ast_constant> gasp::blaise::ast::make_blaise_ast_constant(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                       const std::string &identifier,
                       std::shared_ptr<blaise_ast_type> type){
   return memory::make_shared<blaise_ast_constant>(reference, identifier, type);
                       }
std::shared_ptr<blaise_ast_subroutine_parameter> gasp::blaise::ast::make_blaise_ast_subroutine_parameter(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                       const std::string &identifier,
                       std::shared_ptr<blaise_ast_type> type){
   return memory::make_shared<blaise_ast_subroutine_parameter>(reference, identifier, type);
                       }