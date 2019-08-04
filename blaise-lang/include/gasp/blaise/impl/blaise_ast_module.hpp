#pragma once

#include <string>
#include <memory>
#include <vector>





#include <gasp/blaise/impl/blaise_ast_common.hpp>
#include <gasp/blaise/impl/blaise_ast_subroutine.hpp>

namespace gasp::blaise::ast {

enum class blaise_ast_module_type {
   PROGRAM,
   MODULE
};

class blaise_ast_module : public blaise_ast {
   blaise_ast_module_type _type;
   std::string _name;
   std::vector<std::shared_ptr<blaise_ast_subroutine>> _subroutines;
   std::weak_ptr<blaise_ast_module> _self;

   blaise_ast_module(const sanelli::token<gasp::blaise::blaise_token_type>& reference, const std::string& module_name, blaise_ast_module_type type);

public:
   std::string name() const;
   blaise_ast_module_type type() const;
   void self(std::weak_ptr<blaise_ast_module> module);
   
   std::shared_ptr<blaise_ast_subroutine> add_subroutine(const sanelli::token<gasp::blaise::blaise_token_type>& identifier);

   std::shared_ptr<blaise_ast_subroutine> get_subroutine(
      const sanelli::token<gasp::blaise::blaise_token_type>& identifier,
      const std::vector<std::shared_ptr<blaise_ast_type>>& param_types) const;
   std::shared_ptr<blaise_ast_subroutine> expect_exact_subroutine(
      const sanelli::token<gasp::blaise::blaise_token_type>& identifier,
      const std::vector<std::shared_ptr<blaise_ast_type>>& param_types) const;
      
   unsigned int count_subroutine(
      const sanelli::token<gasp::blaise::blaise_token_type>& identifier,
      const std::vector<std::shared_ptr<blaise_ast_type>>& param_types) const;

   friend sanelli::memory;
};

 std::shared_ptr<blaise_ast_module> make_blaise_ast_module(const sanelli::token<gasp::blaise::blaise_token_type>& reference, const std::string& module_name, blaise_ast_module_type type);


}