#pragma once

#include <string>
#include <memory>
#include <stdexcept>

#include <gasp/blaise/tokens.hpp>
#include <gasp/common/tokenizer.hpp>

#include <gasp/blaise/impl/ast/blaise_ast_common.hpp>
#include <gasp/blaise/impl/ast/blaise_ast_type.hpp>

namespace gasp::blaise::ast {

enum class blaise_ast_variable_type {
   VARIABLE,
   CONSTANT,
   PARAMETER
};

class blaise_ast_generic_memory_location : public blaise_ast {
   std::string _name;
   blaise_ast_type _type;
   blaise_ast_variable_type _variable_type;
   public:
      blaise_ast_generic_memory_location(blaise_ast_variable_type variable_type,
                      const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
                      const std::string& identifier, 
                     blaise_ast_type type);

      std::string name() const;
      blaise_ast_type type() const;
      blaise_ast_variable_type variable_type() const;
};

class blaise_ast_variable : public blaise_ast_generic_memory_location {
   public:
   blaise_ast_variable(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
                      const std::string& identifier, 
                     blaise_ast_type type);

};

class blaise_ast_constant : public blaise_ast_generic_memory_location {
   public:
   blaise_ast_constant(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
                      const std::string& identifier, 
                     blaise_ast_type type);

};

class blaise_ast_subroutine_parameter : public blaise_ast_generic_memory_location {
   public:
   blaise_ast_subroutine_parameter(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
                      const std::string& identifier, 
                     blaise_ast_type type);

};

}