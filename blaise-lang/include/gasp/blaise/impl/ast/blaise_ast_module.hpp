#pragma once

#include <string>
#include <memory>
#include <vector>

#include <gasp/blaise/tokens.hpp>
#include <gasp/common/tokenizer.hpp>

#include <gasp/blaise/impl/ast/blaise_ast_subroutine.hpp>

namespace gasp::blaise::ast {

enum class blaise_module_type {
   PROGRAM,
   MODULE
};

class blaise_module {
   blaise_module_type _type;
   std::string _name;
   std::vector<std::shared_ptr<blaise_subroutine>> _subroutines;
   std::weak_ptr<blaise_module> _self;

   public:
   blaise_module(const gasp::common::token<gasp::blaise::blaise_token_type>& identifier, blaise_module_type type);

   std::string name() const;
   blaise_module_type type() const;
   void self(std::weak_ptr<blaise_module> module);
   
   std::shared_ptr<blaise_subroutine> add_subroutine(const gasp::common::token<gasp::blaise::blaise_token_type>& identifier);

   std::shared_ptr<blaise_subroutine> get_subroutine(
      const gasp::common::token<gasp::blaise::blaise_token_type>& identifier,
      const std::vector<blaise_ast_type>& typaram_typespes) const;
   std::shared_ptr<blaise_subroutine> expect_exact_subroutine(
      const gasp::common::token<gasp::blaise::blaise_token_type>& identifier,
      const std::vector<blaise_ast_type>& typaram_typespes) const;
      
   unsigned int count_subroutine(
      const gasp::common::token<gasp::blaise::blaise_token_type>& identifier,
      const std::vector<blaise_ast_type>& param_types) const;
};

}