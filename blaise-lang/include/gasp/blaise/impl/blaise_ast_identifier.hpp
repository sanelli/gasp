#pragma once

#include <memory>

#include <gasp/blaise/impl/blaise_ast_common.hpp>
#include <gasp/blaise/impl/blaise_ast_variable.hpp>
#include <gasp/blaise/impl/blaise_ast_expression.hpp>

namespace gasp::blaise::ast
{

enum class blaise_ast_identifier_type : unsigned char {
   VARIABLE,
   ARRAY
};

class blaise_ast_identifier : public blaise_ast {
   blaise_ast_identifier_type _type;
protected:
   blaise_ast_identifier(const sanelli::token<gasp::blaise::blaise_token_type>& reference,
                         blaise_ast_identifier_type type);
public:
   blaise_ast_identifier_type type() const;
};

class blaise_ast_variable_identifier : public blaise_ast_identifier {
   std::shared_ptr<blaise_ast_generic_memory_location> _variable;
   blaise_ast_variable_identifier(const sanelli::token<gasp::blaise::blaise_token_type>& reference,
                                 std::shared_ptr<blaise_ast_generic_memory_location> variable);
public:
   std::shared_ptr<blaise_ast_generic_memory_location> variable() const; 
   friend sanelli::memory;
};
std::shared_ptr<blaise_ast_variable_identifier> make_blaise_ast_variable_identifier(const sanelli::token<gasp::blaise::blaise_token_type>& reference,
                                 std::shared_ptr<blaise_ast_generic_memory_location> variable);

class blaise_ast_array_identifier : public blaise_ast_identifier {
   std::shared_ptr<blaise_ast_generic_memory_location> _variable;
   std::shared_ptr<blaise_ast_expression> _indexing_expression;
    blaise_ast_array_identifier(const sanelli::token<gasp::blaise::blaise_token_type>& reference,
                                 std::shared_ptr<blaise_ast_generic_memory_location> variable,
                                 std::shared_ptr<blaise_ast_expression> indexing_expression);
public:
   std::shared_ptr<blaise_ast_generic_memory_location> variable() const; 
   std::shared_ptr<blaise_ast_expression> indexing_expression() const;
   friend sanelli::memory;
};
std::shared_ptr<blaise_ast_array_identifier> make_blaise_ast_array_identifier(const sanelli::token<gasp::blaise::blaise_token_type>& reference,
                                 std::shared_ptr<blaise_ast_generic_memory_location> variable,
                                 std::shared_ptr<blaise_ast_expression> indexing_expression);

}