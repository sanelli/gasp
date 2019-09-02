#pragma once

#include <string>
#include <memory>
#include <bitset>
#include <type_traits>
#include <algorithm>

#include <gasp/blaise/impl/blaise_ast_variable.hpp>
#include <gasp/blaise/impl/blaise_ast_type.hpp>
#include <gasp/blaise/impl/blaise_ast_module.hpp>
#include <gasp/blaise/impl/blaise_ast_statement.hpp>

namespace gasp::blaise::ast {

class blaise_ast_module;

enum class blaise_ast_subroutine_flags : unsigned char {
   NATIVE = 1 << 0
};

class blaise_ast_subroutine {
   std::string _name;
   std::weak_ptr<blaise_ast_module> _module;
   std::shared_ptr<blaise_ast_type> _return_type;
   std::bitset<8 * sizeof(typename std::underlying_type<blaise_ast_subroutine_flags>::type)> _flags;
   std::vector<std::shared_ptr<blaise_ast_subroutine_parameter>> _parameters;
   std::vector<std::shared_ptr<blaise_ast_constant>> _constants;
   std::vector<std::shared_ptr<blaise_ast_variable>> _variables;
   std::vector<std::shared_ptr<blaise_ast_statement>> _statements;

protected:
   template<typename TType>
   inline std::shared_ptr<TType> get_memory_from_vector(const std::vector<std::shared_ptr<TType>>& values,
            const std::string& identifier) const {
      auto it = std::begin(values);
      auto end = std::end(values);
      while (it != end)
      {
         auto variable = *it;
         if (variable->name() == identifier)
         {
            return variable;
         }
         ++it;
      }
      return nullptr;
   }

   blaise_ast_subroutine(std::weak_ptr<blaise_ast_module> module, const std::string& name);

public:
   std::string name() const;
   std::shared_ptr<blaise_ast_type> return_type() const;
   void return_type(std::shared_ptr<blaise_ast_type> type);
   std::weak_ptr<blaise_ast_module> module() const;

   bool is(blaise_ast_subroutine_flags flag) const;
   void set(blaise_ast_subroutine_flags flag);
   void reset(blaise_ast_subroutine_flags flag);

   // Signature
   // Check if the 
   bool signature_match_exactly(const std::string name, const std::vector<std::shared_ptr<blaise_ast_type>>& param_types) const;
   bool signature_match_with_cast(const std::string name, const std::vector<std::shared_ptr<blaise_ast_type>>& param_types) const;
   std::string signature_as_string() const;
   unsigned long get_arity() const;
   std::shared_ptr<blaise_ast_type> get_parameter_type(unsigned long index) const;

   // Memory location management
   std::shared_ptr<blaise_ast_subroutine_parameter> get_parameter(const std::string& identifier) const;
   std::shared_ptr<blaise_ast_constant> get_constant(const std::string& identifier) const;
   std::shared_ptr<blaise_ast_variable> get_variable(const std::string& identifier) const;
   std::shared_ptr<blaise_ast_generic_memory_location> get_memory_location(const std::string& identifier) const;

   unsigned int count_parameters() const;
   std::shared_ptr<blaise_ast_subroutine_parameter> get_parameter(unsigned int index) const;
   unsigned int count_constants() const;
   std::shared_ptr<blaise_ast_constant> get_constant(unsigned int index) const;
   unsigned int count_variables() const;
   std::shared_ptr<blaise_ast_variable> get_variable(unsigned int index) const;

   std::shared_ptr<blaise_ast_subroutine_parameter> add_parameter(const sanelli::token<gasp::blaise::blaise_token_type>& reference,
                     const std::string& identifier, 
                     std::shared_ptr<blaise_ast_type> type);
   std::shared_ptr<blaise_ast_constant> add_constant(const sanelli::token<gasp::blaise::blaise_token_type>& reference,
                     const std::string& identifier, 
                     std::shared_ptr<blaise_ast_type> type);
   std::shared_ptr<blaise_ast_variable> add_variable(const sanelli::token<gasp::blaise::blaise_token_type>& reference, 
                     const std::string& identifier, 
                     std::shared_ptr<blaise_ast_type> type);

   // Statements
   void push_back(std::shared_ptr<blaise_ast_statement> statement);
   std::shared_ptr<blaise_ast_statement> get_statement(unsigned int) const;
   unsigned int get_statements_count() const ;

   friend sanelli::memory;
};

std::shared_ptr<blaise_ast_subroutine> make_blaise_ast_subroutine(std::weak_ptr<blaise_ast_module> module, const std::string& name);

}