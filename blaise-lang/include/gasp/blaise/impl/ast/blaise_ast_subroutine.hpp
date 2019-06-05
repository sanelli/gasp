#pragma once

#include <string>
#include <memory>
#include <bitset>
#include <type_traits>
#include <algorithm>

#include <gasp/blaise/tokens.hpp>
#include <gasp/common/tokenizer.hpp>

#include <gasp/blaise/impl/ast/blaise_ast_variable.hpp>
#include <gasp/blaise/impl/ast/blaise_ast_type.hpp>
#include <gasp/blaise/impl/ast/blaise_ast_module.hpp>
#include <gasp/blaise/impl/ast/blaise_ast_statement.hpp>

namespace gasp::blaise::ast {

class blaise_ast_module;

enum class blaise_ast_subroutine_flags : unsigned char {
   NATIVE = 1 << 0
};

class blaise_ast_subroutine {
   std::string _name;
   std::weak_ptr<blaise_ast_module> _module;
   blaise_ast_type _return_type;
   std::bitset<8 * sizeof(typename std::underlying_type<blaise_ast_subroutine_flags>::type)> _flags;
   std::vector<std::shared_ptr<blaise_ast_subroutine_parameter>> _parameters;
   std::vector<std::shared_ptr<blaise_ast_constant>> _constants;
   std::vector<std::shared_ptr<blaise_ast_variable>> _variables;
   std::vector<std::shared_ptr<blaise_ast_statement>> _statements;

protected:
   template<typename TType>
   inline std::shared_ptr<TType> get_memory_from_vector(std::vector<std::shared_ptr<TType>> values,
            const gasp::common::token<gasp::blaise::blaise_token_type>& identifier) const {
      auto it = std::begin(values);
      auto end = std::end(values);
      while (it != end)
      {
         auto variable = *it;
         if (variable->name() == identifier.value())
         {
            return variable;
         }
         ++it;
      }
      return nullptr;
   }

public:
   blaise_ast_subroutine(std::weak_ptr<blaise_ast_module> module, const std::string& name);

   std::string name() const;
   blaise_ast_type return_type() const;
   void return_type(const gasp::common::token<gasp::blaise::blaise_token_type> &type);
   std::weak_ptr<blaise_ast_module> module() const;

   bool is(blaise_ast_subroutine_flags flag) const;
   void set(blaise_ast_subroutine_flags flag);
   void reset(blaise_ast_subroutine_flags flag);

   // Signature
   // Check if the 
   bool signature_match_exactly(const std::string name, const std::vector<ast::blaise_ast_type>& param_types) const;
   bool signature_match_with_cast(const std::string name, const std::vector<ast::blaise_ast_type>& param_types) const;
   std::string signature_as_string() const;

   // Memory location management
   std::shared_ptr<blaise_ast_subroutine_parameter> get_parameter(const gasp::common::token<gasp::blaise::blaise_token_type>& identifier) const;
   std::shared_ptr<blaise_ast_constant> get_constant(const gasp::common::token<gasp::blaise::blaise_token_type>& identifier) const;
   std::shared_ptr<blaise_ast_variable> get_variable(const gasp::common::token<gasp::blaise::blaise_token_type>& identifier) const;
   std::shared_ptr<blaise_ast_generic_memory_location> get_memory_location(const gasp::common::token<gasp::blaise::blaise_token_type>& identifier) const;

   std::shared_ptr<blaise_ast_subroutine_parameter> add_parameter(const gasp::common::token<gasp::blaise::blaise_token_type>& identifier, 
                     const gasp::common::token<gasp::blaise::blaise_token_type>& type);
   std::shared_ptr<blaise_ast_constant> add_constant(const gasp::common::token<gasp::blaise::blaise_token_type>& identifier, 
                     const gasp::common::token<gasp::blaise::blaise_token_type>& type);
   std::shared_ptr<blaise_ast_variable> add_variable(const gasp::common::token<gasp::blaise::blaise_token_type>& identifier, 
                     const gasp::common::token<gasp::blaise::blaise_token_type>& type);

   // Statements
   void push_back(std::shared_ptr<blaise_ast_statement> statement);
   std::shared_ptr<blaise_ast_statement> get_statement(unsigned int) const;
   unsigned int get_statements_count() const ;
};

}