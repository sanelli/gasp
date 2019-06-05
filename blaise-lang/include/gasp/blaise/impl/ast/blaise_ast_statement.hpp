#pragma once

#include <vector>
#include <memory>

#include <gasp/blaise/impl/ast/blaise_ast_variable.hpp>
#include <gasp/blaise/impl/ast/blaise_ast_expression.hpp>
#include <gasp/blaise/impl/ast/blaise_ast_subroutine.hpp>

namespace gasp::blaise::ast {

// Need this forward declaration otherwise the compile will complain
class blaise_ast_subroutine;

enum class blaise_ast_statement_type : unsigned char{
   COMPOUND,
   ASSIGNEMENT,
   SUBROUTINE_CALL
};

class blaise_ast_statement {
   blaise_ast_statement_type _type;
   protected:
      blaise_ast_statement(blaise_ast_statement_type type);
   public:
      blaise_ast_statement_type type();
};

class blaise_ast_statement_compund : public blaise_ast_statement {
   std::vector<std::shared_ptr<blaise_ast_statement>> _statements;
   blaise_ast_statement_compund();

   public:
      void push_back(std::shared_ptr<blaise_ast_statement> statement);
      std::shared_ptr<blaise_ast_statement> get_statement(unsigned int) const;
      unsigned int get_statements_count() const ;

   friend std::shared_ptr<ast::blaise_ast_statement_compund> make_compound_statement();
};
std::shared_ptr<ast::blaise_ast_statement_compund> make_compound_statement();

class blaise_ast_statement_assignment : public blaise_ast_statement {
   std::shared_ptr<blaise_ast_generic_memory_location> _variable;
   std::shared_ptr<blaise_ast_expression> _expression;
   blaise_ast_statement_assignment(std::shared_ptr<blaise_ast_generic_memory_location> variable, 
                  std::shared_ptr<blaise_ast_expression> expression);
   public:
   std::shared_ptr<blaise_ast_generic_memory_location> variable() const;
   std::shared_ptr<blaise_ast_expression> expression() const;

   friend std::shared_ptr<blaise_ast_statement> make_assignement_statement(unsigned int line, unsigned int column,
                  std::shared_ptr<blaise_ast_generic_memory_location> variable, 
                  std::shared_ptr<blaise_ast_expression> expression);
};
std::shared_ptr<blaise_ast_statement> make_assignement_statement(unsigned int line, unsigned int column,
                  std::shared_ptr<blaise_ast_generic_memory_location> variable, 
                  std::shared_ptr<blaise_ast_expression> expression);

class blaise_ast_statement_subroutine_call : public blaise_ast_statement {
   std::shared_ptr<blaise_ast_subroutine> _subroutine;
   std::vector<std::shared_ptr<blaise_ast_expression>> _actual_parameters;
   blaise_ast_statement_subroutine_call(std::shared_ptr<blaise_ast_subroutine> subroutine,
            const std::vector<std::shared_ptr<blaise_ast_expression>>& parameters);
   public:
   std::shared_ptr<blaise_ast_subroutine> subroutine() const;
   unsigned int parameters_count() const;
   std::shared_ptr<blaise_ast_expression> actual_parameter(unsigned int index) const;

   friend std::shared_ptr<blaise_ast_statement> make_blaise_ast_statement_subroutine_call(std::shared_ptr<blaise_ast_subroutine> subroutine,
            const std::vector<std::shared_ptr<blaise_ast_expression>>& parameters);
};
std::shared_ptr<blaise_ast_statement> make_blaise_ast_statement_subroutine_call(std::shared_ptr<blaise_ast_subroutine> subroutine,
            const std::vector<std::shared_ptr<blaise_ast_expression>>& parameters);

}