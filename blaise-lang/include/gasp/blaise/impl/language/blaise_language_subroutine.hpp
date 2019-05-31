#pragma once

#include <string>
#include <memory>
#include <bitset>
#include <type_traits>
#include <algorithm>

#include <gasp/blaise/tokens.hpp>
#include <gasp/common/tokenizer.hpp>

#include <gasp/blaise/impl/language/blaise_language_variable.hpp>
#include <gasp/blaise/impl/language/blaise_language_type.hpp>
#include <gasp/blaise/impl/language/blaise_language_module.hpp>

namespace gasp::blaise::language {

class blaise_module;

enum class blaise_subroutine_flags : unsigned char {
   NATIVE = 1 << 0
};

class blaise_subroutine {
   std::string _name;
   std::weak_ptr<blaise_module> _module;
   blaise_language_type _return_type;
   std::bitset<8 * sizeof(typename std::underlying_type<blaise_subroutine_flags>::type)> _flags;
   std::vector<std::shared_ptr<blaise_subroutine_parameter>> _parameters;
   std::vector<std::shared_ptr<blaise_constant>> _constants;
   std::vector<std::shared_ptr<blaise_variable>> _variables;
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
   blaise_subroutine(std::weak_ptr<blaise_module> module, const std::string& name);

   std::string name() const;
   blaise_language_type return_type() const;
   void return_type(const gasp::common::token<gasp::blaise::blaise_token_type> &type);
   std::weak_ptr<blaise_module> module() const;

   bool is(blaise_subroutine_flags flag) const;
   void set(blaise_subroutine_flags flag);
   void reset(blaise_subroutine_flags flag);

   // Signature
   bool signature_match(const std::string name, const std::vector<language::blaise_language_type>& param_types) const;
   std::string signature_as_string() const;

   // Memory location management
   std::shared_ptr<blaise_subroutine_parameter> get_parameter(const gasp::common::token<gasp::blaise::blaise_token_type>& identifier) const;
   std::shared_ptr<blaise_constant> get_constant(const gasp::common::token<gasp::blaise::blaise_token_type>& identifier) const;
   std::shared_ptr<blaise_variable> get_variable(const gasp::common::token<gasp::blaise::blaise_token_type>& identifier) const;
   std::shared_ptr<blaise_generic_memory_location> get_memory_location(const gasp::common::token<gasp::blaise::blaise_token_type>& identifier) const;

   std::shared_ptr<blaise_subroutine_parameter> add_parameter(const gasp::common::token<gasp::blaise::blaise_token_type>& identifier, 
                     const gasp::common::token<gasp::blaise::blaise_token_type>& type);
   std::shared_ptr<blaise_constant> add_constant(const gasp::common::token<gasp::blaise::blaise_token_type>& identifier, 
                     const gasp::common::token<gasp::blaise::blaise_token_type>& type);
   std::shared_ptr<blaise_variable> add_variable(const gasp::common::token<gasp::blaise::blaise_token_type>& identifier, 
                     const gasp::common::token<gasp::blaise::blaise_token_type>& type);
};

}