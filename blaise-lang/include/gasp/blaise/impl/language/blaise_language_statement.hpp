#pragma once

#include <vector>
#include <memory>

#include <gasp/blaise/impl/language/blaise_language_variable.hpp>
#include <gasp/blaise/impl/language/blaise_language_expression.hpp>
#include <gasp/blaise/impl/language/blaise_language_subroutine.hpp>

namespace gasp::blaise::language {

// Need this forward declaration otherwise the compile will complain
class blaise_subroutine;

enum class blaise_statement_type : unsigned char{
   COMPOUND,
   ASSIGNEMENT,
   SUBROUTINE_CALL
};

class blaise_statement {
   blaise_statement_type _type;
   protected:
      blaise_statement(blaise_statement_type type);
   public:
      blaise_statement_type type();
};

class blaise_statement_compund : public blaise_statement {
   std::vector<std::shared_ptr<blaise_statement>> _statements;
   blaise_statement_compund();

   public:
      void push_back(std::shared_ptr<blaise_statement> statement);
      std::shared_ptr<blaise_statement> get_statement(unsigned int) const;
      unsigned int get_statements_count() const ;

   friend std::shared_ptr<blaise_statement> make_compound_statement();
};
std::shared_ptr<blaise_statement> make_compound_statement();

class blaise_statement_assignment : public blaise_statement {
   std::shared_ptr<blaise_generic_memory_location> _variable;
   std::shared_ptr<blaise_expression> _expression;
   blaise_statement_assignment(std::shared_ptr<blaise_generic_memory_location> variable, 
                  std::shared_ptr<blaise_expression> expression);
   public:
   std::shared_ptr<blaise_generic_memory_location> variable() const;
   std::shared_ptr<blaise_expression> expression() const;

   friend std::shared_ptr<blaise_statement> make_assignement_statement(std::shared_ptr<blaise_generic_memory_location> variable, 
                  std::shared_ptr<blaise_expression> expression);
};
std::shared_ptr<blaise_statement> make_assignement_statement(std::shared_ptr<blaise_generic_memory_location> variable, 
                  std::shared_ptr<blaise_expression> expression);

class blaise_statement_subroutine_call : public blaise_statement {
   std::shared_ptr<blaise_subroutine> _subroutine;
   std::vector<std::shared_ptr<blaise_expression>> _actual_parameters;
   blaise_statement_subroutine_call(std::shared_ptr<blaise_subroutine> subroutine,
            const std::vector<std::shared_ptr<blaise_expression>>& parameters);
   public:
   std::shared_ptr<blaise_subroutine> subroutine() const;
   unsigned int parameters_count() const;
   std::shared_ptr<blaise_expression> actual_parameter(unsigned int index) const;

   friend std::shared_ptr<blaise_statement> make_blaise_statement_subroutine_call(std::shared_ptr<blaise_subroutine> subroutine,
            const std::vector<std::shared_ptr<blaise_expression>>& parameters);
};
std::shared_ptr<blaise_statement> make_blaise_statement_subroutine_call(std::shared_ptr<blaise_subroutine> subroutine,
            const std::vector<std::shared_ptr<blaise_expression>>& parameters);

}