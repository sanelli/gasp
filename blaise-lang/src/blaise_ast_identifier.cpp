#include <memory>

#include <gasp/blaise/tokenizer/tokenizer.hpp>
#include <gasp/blaise/ast.hpp>
#include <sanelli/sanelli.hpp>

using namespace sanelli;
using namespace std;
using namespace gasp;

using namespace gasp::blaise;
using namespace gasp::blaise::ast;

blaise_ast_identifier::blaise_ast_identifier(const sanelli::token<gasp::blaise::blaise_token_type>& reference,
                         blaise_ast_identifier_type type) 
                         : blaise_ast(reference), _type(type)
                         {}
blaise_ast_identifier_type blaise_ast_identifier::type() const { return _type; }

blaise_ast_variable_identifier::blaise_ast_variable_identifier(const sanelli::token<gasp::blaise::blaise_token_type>& reference,
                                 std::shared_ptr<blaise_ast_generic_memory_location> variable)
                                 : blaise_ast_identifier(reference, blaise_ast_identifier_type::VARIABLE), 
                                 _variable(variable) 
                                 {}
std::shared_ptr<blaise_ast_generic_memory_location> blaise_ast_variable_identifier::variable() const { return _variable; }
std::shared_ptr<blaise_ast_variable_identifier> ast::make_blaise_ast_variable_identifier(const sanelli::token<gasp::blaise::blaise_token_type>& reference,
                                 std::shared_ptr<blaise_ast_generic_memory_location> variable){
   return memory::make_shared<blaise_ast_variable_identifier>(reference, variable);
}

 blaise_ast_array_identifier::blaise_ast_array_identifier(const sanelli::token<gasp::blaise::blaise_token_type>& reference,
                                 std::shared_ptr<blaise_ast_generic_memory_location> variable,
                                 std::vector<std::shared_ptr<blaise_ast_expression>>& indexing) 
                                 : blaise_ast_identifier(reference, blaise_ast_identifier_type::ARRAY), 
                                 _variable(variable),
                                 _indexing(indexing)
                                 {}
std::shared_ptr<blaise_ast_generic_memory_location> blaise_ast_array_identifier::variable() const { return _variable; }
unsigned int blaise_ast_array_identifier::count_indexes() const { return _indexing.size(); }
std::shared_ptr<blaise_ast_expression> blaise_ast_array_identifier::indexing(unsigned int index) const { return _indexing.at(index); }
std::shared_ptr<blaise_ast_array_identifier> ast::make_blaise_ast_array_identifier(const sanelli::token<gasp::blaise::blaise_token_type>& reference,
                                 std::shared_ptr<blaise_ast_generic_memory_location> variable,
                                 std::vector<std::shared_ptr<blaise_ast_expression>>& indexing){
   return memory::make_shared<blaise_ast_array_identifier>(reference, variable, indexing);
}