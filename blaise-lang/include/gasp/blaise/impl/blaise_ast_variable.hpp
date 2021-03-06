#pragma once

#include <string>
#include <memory>
#include <stdexcept>

#include <gasp/blaise/tokenizer/tokenizer.hpp>
#include <gasp/blaise/impl/blaise_ast_common.hpp>
#include <gasp/blaise/impl/blaise_ast_type.hpp>

namespace gasp::blaise::ast
{

enum class blaise_ast_variable_type
{
   VARIABLE,
   CONSTANT,
   PARAMETER,
};

class blaise_ast_generic_memory_location : public blaise_ast
{
   std::string _name;
   std::shared_ptr<blaise_ast_type> _type;
   blaise_ast_variable_type _variable_type;

protected:
   blaise_ast_generic_memory_location(blaise_ast_variable_type variable_type,
                                      const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                      const std::string &identifier,
                                      std::shared_ptr<blaise_ast_type> type);

public:
   std::string name() const;
   std::shared_ptr<blaise_ast_type> type() const;
   blaise_ast_variable_type variable_type() const;
};

class blaise_ast_variable : public blaise_ast_generic_memory_location
{
   friend sanelli::memory;
   blaise_ast_variable(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                       const std::string &identifier,
                       std::shared_ptr<blaise_ast_type> type);
};
std::shared_ptr<blaise_ast_variable> make_blaise_ast_variable(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                       const std::string &identifier,
                       std::shared_ptr<blaise_ast_type> type);


class blaise_ast_expression; // Forward declaration

class blaise_ast_constant : public blaise_ast_generic_memory_location
{
   std::shared_ptr<gasp::blaise::ast::blaise_ast_expression> _literal;

   friend sanelli::memory;
   blaise_ast_constant(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                       const std::string &identifier,
                       std::shared_ptr<blaise_ast_type> type);
   public:
      void literal_expression(std::shared_ptr<gasp::blaise::ast::blaise_ast_expression> literal);
      std::shared_ptr<gasp::blaise::ast::blaise_ast_expression> literal_expression() const;
};
std::shared_ptr<blaise_ast_constant> make_blaise_ast_constant(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                       const std::string &identifier,
                       std::shared_ptr<blaise_ast_type> type);

class blaise_ast_subroutine_parameter : public blaise_ast_generic_memory_location
{
   friend sanelli::memory;
   blaise_ast_subroutine_parameter(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                   const std::string &identifier,
                                   std::shared_ptr<blaise_ast_type> type);
};
std::shared_ptr<blaise_ast_subroutine_parameter> make_blaise_ast_subroutine_parameter(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                       const std::string &identifier,
                       std::shared_ptr<blaise_ast_type> type);

} // namespace gasp::blaise::ast