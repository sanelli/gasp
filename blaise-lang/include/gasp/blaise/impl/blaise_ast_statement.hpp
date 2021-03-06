#pragma once

#include <vector>
#include <memory>

#include <gasp/blaise/impl/blaise_ast_common.hpp>
#include <gasp/blaise/impl/blaise_ast_identifier.hpp>
#include <gasp/blaise/impl/blaise_ast_expression.hpp>
#include <gasp/blaise/impl/blaise_ast_subroutine.hpp>

namespace gasp::blaise::ast
{

// Need this forward declaration otherwise the compile will complain
class blaise_ast_subroutine;

enum class blaise_ast_statement_type : unsigned char
{
   EMPTY,
   COMPOUND,
   ASSIGNEMENT,
   SUBROUTINE_CALL,
   IF,
   FOR_LOOP,
   DO_WHILE_LOOP,
   WHILE_LOOP,
   REPEAT_UNTIL_LOOP,
   DELETE
};

class blaise_ast_statement : public blaise_ast
{
   blaise_ast_statement_type _type;

protected:
   blaise_ast_statement(const sanelli::token<gasp::blaise::blaise_token_type> &reference, blaise_ast_statement_type type);

public:
   blaise_ast_statement_type type();
};

class blaise_ast_statement_empty : public blaise_ast_statement
{
   blaise_ast_statement_type _type;

protected:
   blaise_ast_statement_empty(const sanelli::token<gasp::blaise::blaise_token_type> &reference);

   friend class sanelli::memory;
};
std::shared_ptr<ast::blaise_ast_statement_empty> make_empty_statement(const sanelli::token<gasp::blaise::blaise_token_type> &reference);

class blaise_ast_statement_compund : public blaise_ast_statement
{
   std::vector<std::shared_ptr<blaise_ast_statement>> _statements;
   blaise_ast_statement_compund(const sanelli::token<gasp::blaise::blaise_token_type> &reference);

public:
   void push_back(std::shared_ptr<blaise_ast_statement> statement);
   std::shared_ptr<blaise_ast_statement> get_statement(unsigned int) const;
   unsigned int get_statements_count() const;

   friend class sanelli::memory;
};
std::shared_ptr<ast::blaise_ast_statement_compund> make_compound_statement(const sanelli::token<gasp::blaise::blaise_token_type> &reference);

class blaise_ast_statement_assignment : public blaise_ast_statement
{
   std::shared_ptr<blaise_ast_identifier> _identifier;
   std::shared_ptr<blaise_ast_expression> _expression;
   blaise_ast_statement_assignment(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                   std::shared_ptr<blaise_ast_identifier> identifier,
                                   std::shared_ptr<blaise_ast_expression> expression);

public:
   std::shared_ptr<blaise_ast_identifier> identifier() const;
   std::shared_ptr<blaise_ast_expression> expression() const;

   friend class sanelli::memory;
};
std::shared_ptr<blaise_ast_statement> make_assignement_statement(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                                                 std::shared_ptr<blaise_ast_identifier> identifier,
                                                                 std::shared_ptr<blaise_ast_expression> expression);

class blaise_ast_statement_subroutine_call : public blaise_ast_statement
{
   std::shared_ptr<blaise_ast_subroutine> _subroutine;
   std::vector<std::shared_ptr<blaise_ast_expression>> _actual_parameters;
   blaise_ast_statement_subroutine_call(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                        std::shared_ptr<blaise_ast_subroutine> subroutine,
                                        const std::vector<std::shared_ptr<blaise_ast_expression>> &parameters);

public:
   std::shared_ptr<blaise_ast_subroutine> subroutine() const;
   unsigned int parameters_count() const;
   std::shared_ptr<blaise_ast_expression> actual_parameter(unsigned int index) const;

   friend class sanelli::memory;
};
std::shared_ptr<blaise_ast_statement> make_blaise_ast_statement_subroutine_call(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                                                                std::shared_ptr<blaise_ast_subroutine> subroutine,
                                                                                const std::vector<std::shared_ptr<blaise_ast_expression>> &parameters);

class blaise_ast_statement_if : public blaise_ast_statement
{
   std::shared_ptr<blaise_ast_expression> _condition;
   std::shared_ptr<blaise_ast_statement> _then_statement;
   std::shared_ptr<blaise_ast_statement> _else_statement;
   blaise_ast_statement_if(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                           std::shared_ptr<blaise_ast_expression> condition,
                           std::shared_ptr<blaise_ast_statement> then_statement,
                           std::shared_ptr<blaise_ast_statement> else_statement);

public:
   std::shared_ptr<blaise_ast_expression> condition() const;
   std::shared_ptr<blaise_ast_statement> then_statement() const;
   std::shared_ptr<blaise_ast_statement> else_statement() const;
   friend class sanelli::memory;
};
std::shared_ptr<blaise_ast_statement_if> make_blaise_ast_statement_if(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                                                      std::shared_ptr<blaise_ast_expression> condition,
                                                                      std::shared_ptr<blaise_ast_statement> then_statement,
                                                                      std::shared_ptr<blaise_ast_statement> else_statement);

class blaise_ast_statement_for_loop : public blaise_ast_statement
{
   std::shared_ptr<blaise_ast_identifier> _variable;
   std::shared_ptr<blaise_ast_expression> _from_expression;
   std::shared_ptr<blaise_ast_expression> _to_expression;
   std::shared_ptr<blaise_ast_expression> _step_expression;
   std::shared_ptr<blaise_ast_statement> _body;

   blaise_ast_statement_for_loop(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                 std::shared_ptr<blaise_ast_identifier> variable,
                                 std::shared_ptr<blaise_ast_expression> from_expression,
                                 std::shared_ptr<blaise_ast_expression> to_expression,
                                 std::shared_ptr<blaise_ast_expression> step_expression,
                                 std::shared_ptr<blaise_ast_statement> body);

public:
   std::shared_ptr<blaise_ast_identifier> variable() const;
   std::shared_ptr<blaise_ast_expression> from_expression() const;
   std::shared_ptr<blaise_ast_expression> to_expression() const;
   std::shared_ptr<blaise_ast_expression> step_expression() const;
   std::shared_ptr<blaise_ast_statement> body() const;
   friend class sanelli::memory;
};
std::shared_ptr<blaise_ast_statement_for_loop> make_blaise_ast_statement_for_loop(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                                                                  std::shared_ptr<blaise_ast_identifier> variable,
                                                                                  std::shared_ptr<blaise_ast_expression> from_expression,
                                                                                  std::shared_ptr<blaise_ast_expression> to_expression,
                                                                                  std::shared_ptr<blaise_ast_expression> step_expression,
                                                                                  std::shared_ptr<blaise_ast_statement> body);

class blaise_ast_statement_generic_loop : public blaise_ast_statement
{
   std::shared_ptr<blaise_ast_expression> _condition;
   std::shared_ptr<blaise_ast_statement> _body;

protected:
   blaise_ast_statement_generic_loop(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                     blaise_ast_statement_type type,
                                     std::shared_ptr<blaise_ast_expression> condition,
                                     std::shared_ptr<blaise_ast_statement> body);

public:
   std::shared_ptr<blaise_ast_expression> condition() const;
   std::shared_ptr<blaise_ast_statement> body() const;
};

class blaise_ast_statement_while_loop : public blaise_ast_statement_generic_loop
{
   blaise_ast_statement_while_loop(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                   std::shared_ptr<blaise_ast_expression> condition,
                                   std::shared_ptr<blaise_ast_statement> body);

public:
   friend class sanelli::memory;
};
std::shared_ptr<blaise_ast_statement_while_loop> make_blaise_ast_statement_while_loop(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                                                                      std::shared_ptr<blaise_ast_expression> condition,
                                                                                      std::shared_ptr<blaise_ast_statement> body);

class blaise_ast_statement_dowhile_loop : public blaise_ast_statement_generic_loop
{
   blaise_ast_statement_dowhile_loop(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                     std::shared_ptr<blaise_ast_expression> condition,
                                     std::shared_ptr<blaise_ast_statement> body);

public:
   friend class sanelli::memory;
};
std::shared_ptr<blaise_ast_statement_dowhile_loop> make_blaise_ast_statement_dowhile_loop(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                                                                          std::shared_ptr<blaise_ast_expression> condition,
                                                                                          std::shared_ptr<blaise_ast_statement> body);

class blaise_ast_statement_repeatuntil_loop : public blaise_ast_statement_generic_loop
{
   blaise_ast_statement_repeatuntil_loop(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                         std::shared_ptr<blaise_ast_expression> condition,
                                         std::shared_ptr<blaise_ast_statement> body);

public:
   friend class sanelli::memory;
};
std::shared_ptr<blaise_ast_statement_repeatuntil_loop> make_blaise_ast_statement_repeatuntil_loop(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                                                                                  std::shared_ptr<blaise_ast_expression> condition,
                                                                                                  std::shared_ptr<blaise_ast_statement> body);

class blaise_ast_statement_delete : public blaise_ast_statement
{
   std::shared_ptr<blaise_ast_identifier> _identifier;
   blaise_ast_statement_delete(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                   std::shared_ptr<blaise_ast_identifier> identifier);

public:
   std::shared_ptr<blaise_ast_identifier> identifier() const;

   friend class sanelli::memory;
};
std::shared_ptr<blaise_ast_statement_delete> make_delete_statement(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                                                 std::shared_ptr<blaise_ast_identifier> identifier);

} // namespace gasp::blaise::ast