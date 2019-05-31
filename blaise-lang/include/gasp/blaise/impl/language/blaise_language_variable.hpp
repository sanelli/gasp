#pragma once

#include <string>
#include <memory>
#include <stdexcept>

#include <gasp/blaise/tokens.hpp>
#include <gasp/common/tokenizer.hpp>

#include <gasp/blaise/impl/language/blaise_language_type.hpp>

namespace gasp::blaise::language {

enum class blaise_variable_type {
   VARIABLE,
   CONSTANT,
   PARAMETER
};

class blaise_generic_memory_location {
   std::string _name;
   blaise_language_type _type;
   blaise_variable_type _variable_type;
   public:
      blaise_generic_memory_location(blaise_variable_type variable_type,
                      const gasp::common::token<gasp::blaise::blaise_token_type>& identifier, 
                      const gasp::common::token<gasp::blaise::blaise_token_type>& type);

      std::string name() const;
      blaise_language_type type() const;
      blaise_variable_type variable_type() const;
};

class blaise_variable : public blaise_generic_memory_location {
   public:
   blaise_variable(const gasp::common::token<gasp::blaise::blaise_token_type>& identifier, 
                   const gasp::common::token<gasp::blaise::blaise_token_type>& type);

};

class blaise_constant : public blaise_generic_memory_location {
   public:
   blaise_constant(const gasp::common::token<gasp::blaise::blaise_token_type>& identifier, 
                   const gasp::common::token<gasp::blaise::blaise_token_type>& type);

};

class blaise_subroutine_parameter : public blaise_generic_memory_location {
   public:
   blaise_subroutine_parameter(const gasp::common::token<gasp::blaise::blaise_token_type>& identifier, 
                   const gasp::common::token<gasp::blaise::blaise_token_type>& type);

};

}